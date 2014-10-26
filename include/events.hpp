#pragma once

#include "core.hpp"

namespace color_coded
{
  namespace event
  {
    bool pull(std::string const &file)
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

    void push(std::string const &file, std::string const &data)
    {
      if(pull(file))
      { vim::apply(core::buffers()[file]); }
      core::queue().push({ file, data });
    }

    void moved(std::string const &file, std::size_t const line,
                           std::size_t const lines, std::size_t const height)
    {
      auto &buf(core::buffers()[file]);
      if(pull(file))
      { vim::apply(buf); }
      else
      {
        buf.line = line;
        buf.lines = lines;
        buf.height = height;
        vim::try_apply(buf);
      }
    }

    void enter(std::string const &file, std::string const &data)
    {
      auto &buf(core::buffers()[file]);
      if(buf.group.size())
      { vim::apply(buf); }
      core::queue().push({ file, data });
    }

    void destroy(std::string const &file)
    { core::buffers().erase(file); }

    std::string last_error()
    { return core::last_error(); }
  }
}
