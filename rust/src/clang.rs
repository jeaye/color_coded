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
  clang_include_cpp: String,
  clang_include_lib: String,
  clang_resource: String,
}

impl Config {
  pub fn new() -> Self {
    let clang_dir = std::env::args().nth(0).unwrap();
    Self {
      clang_include: format!("-isystem{}/include", clang_dir),
      clang_include_cpp: format!("-isystem{}/include/c++/v1", clang_dir),
      clang_include_lib: format!("-isystem{}/lib/clang/7.0.0/include", clang_dir),
      clang_resource: format!("-resource-dir={}/lib/clang/7.0.0", clang_dir),
    }
  }
}

pub fn tokenize(config: Arc<Config>, file: NamedTempFile) -> Result<Vec<Token>> {
  let filename = file.path().to_str().unwrap();
  let clang = clang::Clang::new().unwrap();
  let index = clang::Index::new(&clang, false, false);
  debug!("tokenizing file: {}", filename);
  let translation_unit = index
    .parser(filename.clone())
    .detailed_preprocessing_record(true)
    /* TODO: Avoid all of these allocations. */
    .arguments(&[
      // Pre
      config.clang_include.clone(),
      config.clang_include_cpp.clone(),
      "-x".to_owned(),
      "c++".to_owned(),
      "-std=c++17".to_owned(),
      /* TODO: Allow custom flags. */
      // Middle
      "-I.".to_owned(),
      "-Iinclude".to_owned(),
      // Post
      "-isystem/usr/local/include".to_owned(),
      "-isystem/opt/local/include".to_owned(),
      "-isystem/usr/include".to_owned(),
      config.clang_include_lib.clone(),
      config.clang_resource.clone(),
      "-w".to_owned(),
      "-fcolor-diagnostics".to_owned(),
    ])
    .parse()?;

  let diags = translation_unit.get_diagnostics();
  debug!("tokenize diags: {:?}", diags);

  let tokens = translation_unit
    .get_entity()
    .get_range()
    .unwrap()
    .tokenize();
  let cursors = translation_unit.annotate(&tokens);

  for (&tok, &cur) in tokens.iter().zip(cursors.iter()) {
    debug!(
      "\n-----\ntoken: {:?}\ncursor kind: {:#?}\ncursor type: {:#?}\ncursor typekind: {:#?}\n",
      tok.get_spelling(),
      cur.map(|t| t.get_kind()),
      cur.map(|t| t.get_type()),
      cur.map(|t| t.get_type().map(|t| t.get_kind())),
    );
  }

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
