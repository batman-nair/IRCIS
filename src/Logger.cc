#include <Logger.h>

namespace Ircis {
  std::ofstream Logger::log_file = std::ofstream("debug.log");
}
