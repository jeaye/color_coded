use crate::highlight;

type NeovimNamespace = i64;

#[derive(Debug)]
pub struct Buffer {
  pub name: String,
  pub group: highlight::Group,
  pub nvim_namespace_id: NeovimNamespace,
  //pub nvim_buffer: neovim_lib::Buffer,
  pub new_begin: usize,
  pub new_end: usize,
  pub old_begin: usize,
  pub old_end: usize,
}

impl Buffer {
  pub fn new(name: &str, group: highlight::Group) -> Self {
    Self {
      name: name.to_owned(),
      group,
      nvim_namespace_id: 0,
      //nvim_namespace_id: nvim.create_namespace(name).unwrap(),
      //nvim_buffer:
      new_begin: 0,
      new_end: 0,
      old_begin: 0,
      old_end: 0,
    }
  }
}
