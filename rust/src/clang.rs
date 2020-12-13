extern crate clang;

pub struct Token {
  pub token_kind: clang::token::TokenKind,
  pub cursor_kind: clang::EntityKind,
  pub cursor_typekind: Option<clang::TypeKind>,
  pub line: usize,
  pub column: usize,
  pub size: usize,
}

pub fn tokenize(filename: &str) -> Vec<Token> {
  let clang = clang::Clang::new().unwrap();
  let index = clang::Index::new(&clang, false, false);
  let translation_unit = index
    .parser(filename.clone())
    .parse()
    .expect("failed to parse");

  let diags = translation_unit.get_diagnostics();
  /* TODO: Improve logging. */
  println!("diags: {:?}", diags);

  let tokens = translation_unit
    .get_entity()
    .get_range()
    .unwrap()
    .tokenize();
  let cursors = translation_unit.annotate(&tokens);

  //for (&tok, &cur) in tokens.iter().zip(cursors.iter()) {
  //  println!(
  //    "\n-----\ntoken: {:?}\ncursor kind: {:#?}\ncursor type: {:#?}\ncursor typekind: {:#?}\n",
  //    tok.get_spelling(),
  //    cur.map(|t| t.get_kind()),
  //    cur.map(|t| t.get_type()),
  //    cur.map(|t| t.get_type().map(|t| t.get_kind())),
  //  );
  //}

  /* TODO: Parallelize? */
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
    .collect()
}
