#include <iostream>
#include <string>

struct foo
{
  void work()
  { }

  char const * name{};
  int age{};
  static long count;
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

enum class kitten
{
  meow,
  purr
};

/* Oh, excellent. */
int main()
{
  foo f;
  f.name = "jeaye";
  f.age = 22;
  f.work();
  auto const k(kitten::meow);
  std::cout << static_cast<int>(k) << std::endl;
}
