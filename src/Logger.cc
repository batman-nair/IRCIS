#include <Logger.h>

namespace Ircis {
  std::ofstream Logger::log_file_ = std::ofstream("debug.log");
  bool Logger::quiet_mode_ = false;

  void Logger::set_quiet_mode(bool quiet_mode) {
    quiet_mode_ = quiet_mode;
  }
}
