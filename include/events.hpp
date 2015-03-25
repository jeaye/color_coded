#pragma once

#include "core.hpp"

namespace color_coded
{
  namespace event
  {
    inline bool pull(std::string const &file)
    {
      auto const pulled(core::queue().pull());
      if(pulled.second)
      {
        auto &buf(core::buffers()[pulled.first.name]);
        buf.group = std::move(pulled.first.group);
        return file == pulled.first.name;
      }
      return false;
    }

    inline void push(std::string const &file, std::string const &data)
    {
      if(pull(file))
      { vim::apply(core::buffers()[file]); }
      core::queue().push({ file, data });
    }

    inline void moved(std::string const &file, std::size_t const line,
                      std::size_t const lines, std::size_t const height)
    {
      auto &buf(core::buffers()[file]);
      buf.line = line;
      buf.lines = lines;
      buf.height = height;

      if(pull(file)) /* Pulled new info. */
      { vim::apply(buf); }
      else /* See if the move requires another application. */
      { vim::try_apply(buf); }
    }

    inline void enter(std::string const &file, std::string const &data)
    {
      auto &buf(core::buffers()[file]);
      if(buf.group.size())
      { vim::apply(buf); }
      core::queue().push({ file, data });
    }

    inline void destroy(std::string const &file)
    { core::buffers().erase(file); }

    inline std::string last_error()
    { return core::last_error(); }
  }
}
