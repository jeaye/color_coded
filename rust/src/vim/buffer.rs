use crate::highlight;

#[derive(Debug)]
pub struct Buffer {
  pub name: String,
  pub number: i64,
  pub group: highlight::Group,
  pub new_begin: usize,
  pub new_end: usize,
  pub old_begin: usize,
  pub old_end: usize,
}

impl Buffer {
  pub fn new(name: &str, number: i64, group: highlight::Group) -> Self {
    Self {
      name: name.to_owned(),
      number,
      group,
      new_begin: 0,
      new_end: 0,
      old_begin: 0,
      old_end: 0,
    }
  }
}
