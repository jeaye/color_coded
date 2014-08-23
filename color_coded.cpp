#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "ruby.h"

#include <juble/juble.hpp>

namespace color_coded
{
  void eval(std::string const &str)
  { rb_eval_string(str.c_str()); }

  void work(std::string const &line)
  {
    std::vector<int> nums;
    std::stringstream ss{ line };
    int tmp{};
    while(ss >> tmp)
    { nums.push_back(tmp); }
    if(nums.empty())
    { return; }

    auto const it(std::max_element(std::begin(nums), std::end(nums)));
    size_t const index(std::distance(std::begin(nums), it));
    size_t offset{ 1 };
    for(size_t i{}; i < index; ++i)
    { offset += std::to_string(nums[i]).size(); }
    offset += index; /* spaces */

    std::string const col{ std::to_string(offset) };
    std::string const len{ std::to_string(std::to_string(*it).size()) };
    eval("VIM::command(\"call clearmatches()\")");
    eval("VIM::command(\"call matchaddpos('keyword', [[1, " + col + ", " + len  + "]], -1)\")");
  }
}

extern "C" void Init_color_coded()
{
  script::registrar::add(script::func(&color_coded::work, "cc_work"));
}
