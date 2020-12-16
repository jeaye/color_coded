extern crate clang;

use anyhow::Result;
use log::*;
use std::sync::Arc;
use tempfile::NamedTempFile;

pub struct Token {
  pub token_kind: clang::token::TokenKind,
  pub cursor_kind: clang::EntityKind,
  pub cursor_typekind: Option<clang::TypeKind>,
  pub line: usize,
  pub column: usize,
  pub size: usize,
}

pub struct Config {
  clang_include: String,
  clang_include_cxx: String,
  clang_include_lib: String,
  clang_resource: String,
}

impl Config {
  pub fn new() -> Self {
    let clang_dir = std::env::args().nth(1).unwrap();
    debug!("clang directory: {}", clang_dir);
    Self {
      clang_include: format!("-isystem{}/include", clang_dir),
      clang_include_cxx: format!("-isystem{}/include/c++/v1", clang_dir),
      clang_include_lib: format!("-isystem{}/lib/clang/7.0.0/include", clang_dir),
      clang_resource: format!("-resource-dir={}/lib/clang/7.0.0", clang_dir),
    }
  }

  pub fn build_arguments(&self, file_type: String) -> Vec<&str> {
    let mut args = vec![
      &self.clang_include[..],
      #[cfg(target_os = "macos")]
      "-isystem/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include",
      "-x",
      match &file_type[..] {
        "c" => "c",
        "c++" => "c++",
        "objective-c" => "objective-c",
        "objective-c++" => "objective-c++",
        _ => "",
      },
    ];
    let cxx_pre = [
      &self.clang_include_cxx[..],
      #[cfg(target_os = "macos")]
      "-isystem/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1",
    ];
    match &file_type[..] {
      "c++" => args.extend(cxx_pre.iter().chain(&["-std=c++17"]).copied()),
      "objective-c++" => args.extend(cxx_pre.iter().copied()),
      _ => {}
    };

    /* TODO: Allow custom flags. */
    let middle = ["-I.", "-Iinclude"];
    args.extend(middle.iter());

    let universal_post = [
      "-isystem/usr/local/include",
      "-isystem/opt/local/include",
      "-isystem/usr/include",
      &self.clang_include_lib,
      &self.clang_resource,
      #[cfg(target_os = "macos")]
      "-isystem/System/Library/Frameworks",
      #[cfg(target_os = "macos")]
      "-isystem/Library/Frameworks",
      "-w",
      /* See https://github.com/jeaye/color_coded/issues/104 */
      "-fcolor-diagnostics",
    ];
    args.extend(universal_post.iter());

    debug!("compilation args: {:?}", args);

    args
  }
}

pub fn tokenize(config: Arc<Config>, file: NamedTempFile, file_type: String) -> Result<Vec<Token>> {
  let filename = file.path().to_str().unwrap();
  let clang = clang::Clang::new().unwrap();
  let index = clang::Index::new(&clang, false, false);
  debug!("tokenizing file: {}", filename);
  let translation_unit = index
    .parser(filename.clone())
    .detailed_preprocessing_record(true)
    .arguments(&config.build_arguments(file_type))
    .parse()?;

  let diags = translation_unit.get_diagnostics();
  debug!("tokenize diags: {:?}", diags);

  let tokens = translation_unit
    .get_entity()
    .get_range()
    .unwrap()
    .tokenize();
  let cursors = translation_unit.annotate(&tokens);

  //for (&tok, &cur) in tokens.iter().zip(cursors.iter()) {
  //  debug!(
  //    "\n-----\ntoken: {:?}\ncursor kind: {:#?}\ncursor type: {:#?}\ncursor typekind: {:#?}\n",
  //    tok.get_spelling(),
  //    cur.map(|t| t.get_kind()),
  //    cur.map(|t| t.get_type()),
  //    cur.map(|t| t.get_type().map(|t| t.get_kind())),
  //  );
  //}

  /* TODO: Parallelize? */
  Ok(
    tokens
      .iter()
      .zip(cursors.iter())
      .filter_map(|(&tok, &cur)| match cur {
        Some(cur) => {
          let location = cur.get_location().unwrap().get_file_location();

          Some(Token {
            token_kind: tok.get_kind(),
            cursor_kind: cur.get_kind(),
            cursor_typekind: cur.get_type().map(|t| t.get_kind()),
            line: location.line as usize,
            column: location.column as usize,
            size: tok.get_spelling().len(),
          })
        }
        None => None,
      })
      .collect(),
  )
}
