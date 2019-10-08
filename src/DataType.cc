#include <DataType.h>

#include <ostream>

namespace PTrain {
  std::string DataType::to_string() const {
    return is_integer? std::to_string(value) : std::string(1, (char)value);
  }

  std::ostream& operator<<(std::ostream& os, const Data& data) {
    return os << data.to_string();
  }
}
