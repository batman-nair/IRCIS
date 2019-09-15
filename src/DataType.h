#pragma once

namespace PTrain {
  struct DataType {

  DataType(int value, bool is_integer)
    : value(value), is_integer(is_integer) { }

    int value;
    bool is_integer;
  };

  typedef DataType Data;
}
