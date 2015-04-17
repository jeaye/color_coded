#include <jest/jest.hpp>

#include "config.hpp"

int main()
{
    jest::worker const j{};
    return j();
}
