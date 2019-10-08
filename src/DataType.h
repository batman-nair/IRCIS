#pragma once

#include <string>

namespace PTrain {
  // Structure to store values taken from grid in stack
  struct DataType {

  DataType(int value, bool is_integer=false)
    : value(value), is_integer(is_integer) { }

    int value;
    bool is_integer;

    // Return string equivalent of the data stored
    std::string to_string() const;
  };

  typedef DataType Data;
}
