#include <DataType.h>

#include <ostream>

namespace PTrain {
  char DataType::getc() const {
    return value + (is_integer? '0':0);
  }

  std::ostream& operator<<(std::ostream& os, const Data& data) {
    return os << data.getc();
  }
}
