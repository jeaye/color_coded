struct foo
{
  void work()
  { }

  char const * name;
  int age;
};

/* Oh, excellent. */
int main()
{
  foo f;
  f.name = "jeaye";
  f.age = 22;
  f.work();
}
