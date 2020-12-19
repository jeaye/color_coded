use crate::highlight;

pub type BufferNumber = i64;

#[derive(Debug)]
pub struct Buffer {
  pub name: String,
  pub number: BufferNumber,
  pub group: highlight::Group,
  pub window_start_line: i64,
  pub window_end_line: i64,
}

impl Buffer {
  pub fn new(
    name: &str,
    number: i64,
    window_start_line: i64,
    window_end_line: i64,
    group: highlight::Group,
  ) -> Self {
    Self {
      name: name.to_owned(),
      number,
      group,
      window_start_line,
      window_end_line,
    }
  }
}
