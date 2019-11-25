#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <fstream>

template <typename T>
void DBG_slave(T val) {
  std::cout << val << std::endl;
}
template <typename T, typename... Types>
void DBG_slave(T val, Types... vars) {
  std::cout << val;
  DBG_slave(vars...);
}

#ifndef NDEBUG
#define DBG(...) do { DBG_slave( "", ## __VA_ARGS__ ); } while (0)
#define PN(x) (#x) << ": " << x << " "
#else
#define DBG(...) do {} while (0)
#define PN(x)
#endif

namespace Ircis {
  class Logger {
  public:
    Logger(std::string file_name, bool quiet_mode=false)
      : output_file_(std::make_shared<std::fstream>(file_name)) {
      set_quiet_mode(quiet_mode);
    }
    Logger(std::shared_ptr<std::ostream> log, bool quiet_mode=false)
      : output_file_(log) {
      set_quiet_mode(quiet_mode);
    }

    template <class T>
    void print(T val) {
      if (!quiet_mode_)
	std::cout << val;
      *output_file_ << val;
    }
    void print_line() {
      if (!quiet_mode_)
	std::cout << std::endl;
      *output_file_ << std::endl;
    }
    template <class T>
    void print_line(T val) {
      if (!quiet_mode_)
	std::cout << val << std::endl;
      *output_file_ << val << std::endl;
    }

  private:
    std::shared_ptr<std::ostream> output_file_;

    static bool quiet_mode_;
    static void set_quiet_mode(bool quiet_mode);

  public:
    template <typename... Types>
    static void log_line_dbg(Types... vars) {
      log_line(vars...);
      if (!quiet_mode_)
	DBG("DEBUG: ", vars...);
    }
    template <typename... Types>
    static void err_line_dbg(Types... vars) {
      err_line(vars...);
      if (!quiet_mode_)
	DBG("ERROR: ", vars...);
    }
    template <typename... Types>
    static void log_line(Types... vars) {
      log_file_ << "DEBUG: ";
      log_vars_slave(vars...);
      log_file_ << std::endl;
    }
    template <typename... Types>
    static void err_line(Types... vars) {
      log_file_ << "ERROR: ";
      log_vars_slave(vars...);
      log_file_ << std::endl;
    }

  private:
    static std::ofstream log_file_; // Initialized in cc
    template <typename T>
    static void log_vars_slave(T val) {
      log_file_ << val;
    }
    template <typename T, typename... Types>
    static void log_vars_slave(T val, Types... vars) {
      log_file_ << val;
      log_vars_slave(vars...);
    }
  };

}
