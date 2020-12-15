extern crate clang;

use anyhow::Result;
use log::*;
use tempfile::NamedTempFile;

pub struct Token {
  pub token_kind: clang::token::TokenKind,
  pub cursor_kind: clang::EntityKind,
  pub cursor_typekind: Option<clang::TypeKind>,
  pub line: usize,
  pub column: usize,
  pub size: usize,
}

pub fn tokenize(file: NamedTempFile) -> Result<Vec<Token>> {
  let filename = file.path().to_str().unwrap();
  let clang = clang::Clang::new().unwrap();
  let index = clang::Index::new(&clang, false, false);
  debug!("tokenizing file: {}", filename);
  let translation_unit = index
    .parser(filename.clone())
    .detailed_preprocessing_record(true)
    /* TODO: Read clang path from vim. */
    .arguments(&[
      // Pre
      "-isystem/home/jeaye/downloads/tmp/clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04/include",
      "-isystem/home/jeaye/downloads/tmp/clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04/include/c++/v1",
      "-x", "c++",
      "-std=c++17",

      /* TODO: Allow custom flags. */
      // Middle
      "-I.",
      "-Iinclude",

      // Post
      "-isystem/usr/local/include",
      "-isystem/opt/local/include",
      "-isystem/usr/include",
      "-isystem/home/jeaye/downloads/tmp/clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04/lib/clang/7.0.0/include",
      "-resource-dir=/home/jeaye/downloads/tmp/clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04/lib/clang/7.0.0",
      "-w",
      "-fcolor-diagnostics"
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
