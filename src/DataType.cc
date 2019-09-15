#include <DataType.h>

#include <ostream>

namespace PTrain {
  std::ostream& operator<<(std::ostream& os, const Data& data) {
    if (data.is_integer)
      return os << data.value;
    else
      return os << (char)data.value;
  }
}
