extern crate neovim_lib;
extern crate simplelog;

mod clang;
mod highlight;
mod vim;

use crate::vim::handler;
use anyhow::Result;
use log::*;
use neovim_lib::NeovimApi;
use tempfile::NamedTempFile;
use tokio::sync::mpsc;
use vim::buffer::Buffer;

pub struct App {
  nvim: neovim_lib::Neovim,
  event_receiver: mpsc::UnboundedReceiver<handler::Event>,
  log_file_path: String,
}

impl App {
  pub fn new(runtime_handle: tokio::runtime::Handle) -> Self {
    let log_file_path = match Self::initialize_logging() {
      Ok(path) => path,
      Err(e) => panic!("unable to initialize logging: {}", e),
    };

    let (event_sender, event_receiver) = mpsc::unbounded_channel();
    let mut session = neovim_lib::Session::new_parent().unwrap();
    /* TODO: Document this design. */
    session.start_event_loop_handler(handler::Handler::new(event_sender, runtime_handle));
    let nvim = neovim_lib::Neovim::new(session);

    Self {
      nvim,
      event_receiver,
      log_file_path,
    }
  }

  pub async fn run_event_loop(&mut self) {
    debug!("starting main event loop");
    loop {
      let event = self.event_receiver.recv().await;
      let result = match event {
        Some(handler::Event::Apply { ref buffer }) => self.apply(buffer),
        Some(handler::Event::OpenLog) => self.open_log(),
        None => {
          break;
        }
      };

      if let Err(e) = result {
        error!("error running event {:?}: {}", event, e);
      }
    }
  }

  fn apply(&mut self, buffer: &Buffer) -> Result<()> {
    debug!("applying buffer highlighting: {:#?}", buffer);
    let nvim_buf = neovim_lib::neovim_api::Buffer::new(neovim_lib::Value::from(buffer.number));
    /* TODO: Cache this. */
    let namespace = self.nvim.create_namespace(&buffer.name)?;

    /* TODO: Only clear the highlighted range? */
    nvim_buf.clear_namespace(&mut self.nvim, namespace, 0, -1)?;

    for highlight in &buffer.group.highlights {
      if (highlight.line as i64) < buffer.window_start_line
        || (highlight.line as i64) > buffer.window_end_line
      {
        continue;
      }

      nvim_buf.add_highlight(
        &mut self.nvim,
        namespace,
        highlight.r#type,
        highlight.line as i64,
        highlight.column as i64,
        (highlight.column + highlight.size) as i64,
      )?;
    }

    Ok(())
  }

  fn open_log(&mut self) -> Result<()> {
    /* TODO: Focus existing tab, if there is one. */
    /* TODO: Jump to bottom of log. */
    /* TODO: See if the buffer can be made to `tail -f` reliably. */
    self
      .nvim
      .command(&format!("tabnew {}", self.log_file_path))?;

    Ok(())
  }

  fn initialize_logging() -> Result<String, Box<dyn std::error::Error>> {
    /* TODO: Consider replacing simplelog with something better. */
    use simplelog::*;

    /* TODO: Read this from Vim config instead. */
    let log_level_filter = match std::env::var("COLOR_CODED_LOG_LEVEL")
      .unwrap_or("trace".to_owned())
      .to_lowercase()
      .as_ref()
    {
      "trace" => LevelFilter::Trace,
      "debug" => LevelFilter::Debug,
      "info" => LevelFilter::Info,
      "warn" => LevelFilter::Warn,
      "error" => LevelFilter::Error,
      _ => LevelFilter::Off,
    };

    let default_log_file = NamedTempFile::new()?;
    let log_file_path = default_log_file.path().to_str().unwrap().to_owned();
    let config = ConfigBuilder::new().set_time_to_local(true).build();

    CombinedLogger::init(vec![
      //TermLogger::new(log_level_filter, config, TerminalMode::Mixed),
      WriteLogger::new(log_level_filter, config, default_log_file),
    ])?;

    debug!("logging initialized: {}", log_file_path);

    Ok(log_file_path)
  }
}

fn main() {
  let runtime = tokio::runtime::Builder::new_multi_thread().build().unwrap();
  let handle = runtime.handle().clone();

  runtime.block_on(async move {
    let mut app = App::new(handle);
    app.run_event_loop().await;
    debug!("exiting main");
  })
}
