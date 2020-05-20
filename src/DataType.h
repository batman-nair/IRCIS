#pragma once

#include <string>

namespace Ircis {

  // integer exponentiation
  inline int power(int a, int n) {
    int res = 1;
    while (n) {
      if (n & 1)
        res *= a;
      a *= a;
      n >>= 1;
    }
    return res;
  }

  // Structure to store values taken from grid in stack
  struct DataType {
    DataType() : value(0), is_integer(true) { }

    DataType(int value, bool is_integer=false)
      : value(value), is_integer(is_integer) { }

    int value;
    bool is_integer;

    // Return string equivalent of the data stored
    std::string to_string() const;

    DataType operator + (const DataType& num) {
      return DataType(value + num.value, true);
    }
    DataType operator - (const DataType& num) {
      return DataType(value - num.value, true);
    }
    DataType operator * (const DataType& num) {
      return DataType(value * num.value, true);
    }
    DataType operator / (const DataType& num) {
      return DataType(value / num.value, true);
    }
    DataType operator % (const DataType& num) {
      return DataType(value % num.value, true);
    }
    DataType operator ^ (const DataType& num) {
      return DataType(power(value, num.value), true);
    }
    DataType operator & (const DataType& num) {
      return DataType(value & num.value, true);
    }
    DataType operator | (const DataType& num) {
      return DataType(value | num.value, true);
    }
    DataType V (const DataType& num) {
      return DataType(value ^ num.value, true);
    }
    DataType operator < (const DataType& num) {
      return DataType(value << num.value, true);
    }
    DataType operator > (const DataType& num) {
      return DataType(value >> num.value, true);
    }
  };

  typedef DataType Data;

  std::ostream& operator<<(std::ostream& os, const Data& data);
}
