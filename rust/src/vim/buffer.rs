use crate::highlight;

pub type BufferNumber = i64;

#[derive(Debug)]
pub struct Buffer {
  pub number: BufferNumber,
  pub group: highlight::Group,
  pub range_start_line: i64,
  pub range_end_line: i64,
}

impl Buffer {
  pub fn new(
    number: i64,
    range_start_line: i64,
    range_end_line: i64,
    group: highlight::Group,
  ) -> Self {
    Self {
      number,
      group,
      range_start_line,
      range_end_line,
    }
  }
}
