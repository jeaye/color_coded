struct foo
{
  void work()
  { }

  char const * name;
  int age;
};

template <typename T>
class bar
{
  public:
    bar()
    { }

    virtual T spam() const
    { return T{}; }
};

/* Oh, excellent. */
int main()
{
  foo f;
  f.name = "jeaye";
  f.age = 22;
  f.work();
}
