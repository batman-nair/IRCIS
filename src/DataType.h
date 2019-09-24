#pragma once

namespace PTrain {
  struct DataType {

  DataType(int value, bool is_integer=false)
    : value(value), is_integer(is_integer) { }

    int value;
    bool is_integer;
  };

  typedef DataType Data;
}
