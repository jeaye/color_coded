#pragma once

namespace color_coded
{
  namespace env
  { struct tag{}; }

  template <typename T>
  struct environment;
}

#include "env/impl.hpp"
