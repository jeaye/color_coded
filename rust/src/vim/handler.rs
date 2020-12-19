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
  value.as_i64().ok_or(anyhow!("cannot parse usize"))
}

#[derive(Debug)]
pub enum Event {
  Apply { buffer: Buffer },
  OpenLog,
}

struct CachedCompilation {
  data_hash: u64,
  group: highlight::Group,
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
    if args.len() != 6 {
      return Err(anyhow!("invalid args to recompile: {:?}", args));
    }

    let filename = parse_string(&args[0])?;
    let file_type = parse_string(&args[1])?;
    let data = parse_string(&args[2])?;
    let window_start_line = parse_i64(&args[3])?;
    let window_end_line = parse_i64(&args[4])?;
    let buffer_number = parse_i64(&args[5])?;

    let data_hash;
    {
      use std::hash::{Hash, Hasher};
      let mut hasher = DefaultHasher::new();
      data.hash(&mut hasher);
      data_hash = hasher.finish();
    }

    {
      let cache = handler.compilation_cache.lock().await;
      match cache.get(&buffer_number) {
        Some(cached_compilation) => {
          if cached_compilation.data_hash == data_hash {
            debug!("skipping recompilation; data is unchanged");
            return Ok(None);
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
        entry.data_hash = data_hash;
        entry.group = group.clone();
      } else {
        cache.insert(
          buffer_number,
          CachedCompilation {
            data_hash,
            group: group.clone(),
          },
        );
      }
    }

    Ok(Some(Event::Apply {
      buffer: Buffer::new(
        &filename,
        buffer_number,
        window_start_line,
        window_end_line,
        group,
      ),
    }))
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
