use crate::highlight;
use crate::vim::buffer::Buffer;
use anyhow::{anyhow, Result};
use log::*;
use neovim_lib::Value;
use std::collections::hash_map::DefaultHasher;
use std::collections::HashMap;
use std::io::Write;
use std::sync::Arc;
use tempfile::NamedTempFile;
use tokio::sync::mpsc;
use tokio::sync::Mutex;

pub fn parse_string(value: &neovim_lib::Value) -> Result<String> {
  value
    .as_str()
    .ok_or(anyhow!("cannot parse error"))
    .map(|s| String::from(s))
}

pub fn parse_i64(value: &neovim_lib::Value) -> Result<i64> {
  value.as_i64().ok_or(anyhow!("cannot parse i64"))
}

#[derive(Debug)]
pub enum Event {
  Apply { buffer: Buffer },
  OpenLog,
}

struct CachedCompilation {
  data_hash: u64,
  group: highlight::Group,
  range_start_line: i64,
  range_end_line: i64,
}

#[derive(Clone)]
pub struct Handler {
  runtime_handle: tokio::runtime::Handle,
  event_sender: Arc<Mutex<mpsc::UnboundedSender<Event>>>,
  clang_config: Arc<crate::clang::Config>,
  compilation_cache: Arc<Mutex<HashMap<crate::vim::buffer::BufferNumber, CachedCompilation>>>,
}

impl Handler {
  pub fn new(
    event_sender: mpsc::UnboundedSender<Event>,
    runtime_handle: tokio::runtime::Handle,
  ) -> Self {
    Self {
      runtime_handle,
      event_sender: Arc::new(Mutex::new(event_sender)),
      clang_config: Arc::new(crate::clang::Config::new()),
      compilation_cache: Arc::new(Mutex::new(HashMap::new())),
    }
  }

  async fn recompile(handler: &mut Self, args: &Vec<Value>) -> Result<Option<Event>> {
    if args.len() != 5 {
      return Err(anyhow!("invalid args to recompile: {:?}", args));
    }

    let file_type = parse_string(&args[0])?;
    let data = parse_string(&args[1])?;
    let range_start_line = parse_i64(&args[2])?;
    let range_end_line = parse_i64(&args[3])?;
    let buffer_number = parse_i64(&args[4])?;

    let data_hash;
    {
      use std::hash::{Hash, Hasher};
      let mut hasher = DefaultHasher::new();
      data.hash(&mut hasher);
      data_hash = hasher.finish();
    }

    {
      let mut cache = handler.compilation_cache.lock().await;
      match cache.get_mut(&buffer_number) {
        Some(cached_compilation) => {
          if cached_compilation.data_hash == data_hash {
            debug!("skipping recompilation; data is unchanged");
            return Ok(None);
          } else {
            /* Mark the current hash as this one so no parallel tasks do duplicate work. */
            cached_compilation.data_hash = data_hash;
          }
        }
        None => {}
      }
    }

    let clang_config = handler.clang_config.clone();
    let tokens = handler
      .runtime_handle
      .spawn_blocking(move || {
        let mut temp_file = NamedTempFile::new()?;
        temp_file.write_all(data.as_bytes())?;
        crate::clang::tokenize(clang_config, temp_file, file_type)
      })
      .await??;

    let group = highlight::Group::new(tokens);

    {
      let mut cache = handler.compilation_cache.lock().await;
      if let Some(mut entry) = cache.get_mut(&buffer_number) {
        entry.group = group.clone();
      } else {
        cache.insert(
          buffer_number,
          CachedCompilation {
            data_hash,
            group: group.clone(),
            range_start_line,
            range_end_line,
          },
        );
      }
    }

    Ok(Some(Event::Apply {
      buffer: Buffer::new(buffer_number, range_start_line, range_end_line, group),
    }))
  }

  async fn move_and_apply(handler: &mut Self, args: &Vec<Value>) -> Result<Option<Event>> {
    if args.len() != 3 {
      return Err(anyhow!("invalid args to recompile: {:?}", args));
    }

    let range_start_line = parse_i64(&args[0])?;
    let range_end_line = parse_i64(&args[1])?;
    let buffer_number = parse_i64(&args[2])?;

    let mut cache = handler.compilation_cache.lock().await;
    match cache.get_mut(&buffer_number) {
      Some(cached_compilation) => {
        let (new_range_start, new_range_end) =
          if range_start_line < cached_compilation.range_start_line {
            (range_start_line, cached_compilation.range_start_line)
          } else if range_end_line > cached_compilation.range_end_line {
            (cached_compilation.range_end_line, range_end_line)
          } else {
            return Ok(None);
          };

        cached_compilation.range_start_line = new_range_start;
        cached_compilation.range_end_line = new_range_end;

        /* TODO: Don't clear when applying. */
        Ok(Some(Event::Apply {
          buffer: Buffer::new(
            buffer_number,
            new_range_start,
            new_range_end,
            cached_compilation.group.clone(),
          ),
        }))
      }
      None => Ok(None),
    }
  }
}

impl neovim_lib::Handler for Handler {
  fn handle_notify(&mut self, name: &str, args: Vec<Value>) {
    debug!("handler event: {}", name);
    match name {
      /* TODO: Queue. */
      "enter_buffer" | "recompile" => {
        let mut handler = self.clone();
        self.runtime_handle.spawn(async move {
          match Self::recompile(&mut handler, &args).await {
            Ok(Some(event)) => {
              let event_sender = handler.event_sender.lock().await;
              if let Err(reason) = event_sender.send(event) {
                // TODO: Improve
                error!("failed to send {}", reason);
              }
            }
            Ok(None) => {}
            Err(e) => error!("failed to handle: {}", e),
          }
        });
      }
      "move" => {
        let mut handler = self.clone();
        self.runtime_handle.spawn(async move {
          match Self::move_and_apply(&mut handler, &args).await {
            Ok(Some(event)) => {
              let event_sender = handler.event_sender.lock().await;
              if let Err(reason) = event_sender.send(event) {
                // TODO: Improve
                error!("failed to send {}", reason);
              }
            }
            Ok(None) => {}
            Err(e) => error!("failed to handle: {}", e),
          }
        });
      }
      "open_log" => {
        let event_sender = self.event_sender.clone();
        self.runtime_handle.spawn(async move {
          let event_sender = event_sender.lock().await;
          if let Err(reason) = event_sender.send(Event::OpenLog) {
            // TODO: Improve
            error!("failed to send {}", reason);
          }
        });
      }
      _ => {
        debug!("unmatched event: {}", name);
      }
    }
  }
}

impl neovim_lib::RequestHandler for Handler {
  fn handle_request(&mut self, _name: &str, _args: Vec<Value>) -> Result<Value, Value> {
    Err(Value::from("not implemented"))
  }
}
