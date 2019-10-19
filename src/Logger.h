#pragma once

#include <iostream>
#include <string>
#include <fstream>

#ifndef NDEBUG
#define DBG(x) std::cout << x;
#define PN(x) (#x) << ": " << x << " "
#else
#define DBG(x) ;
#define PN(x)
#endif

namespace PTrain {
  class Logger {
  public:
    Logger(std::string file_name) : output_file(file_name) { }

    template <class T>
    void print(T val) {
      std::cout << val;
      output_file << val;
    }
    void print_line() {
      std::cout << std::endl;
      output_file << std::endl;
    }
    template <class T>
    void print_line(T val) {
      std::cout << val << std::endl;
      output_file << val << std::endl;
    }

  private:
    std::ofstream output_file;


  public:
    template <typename T>
    static void log_vars_slave(T val) {
      DBG(val);
      log_file << val;
    }
    template <typename T, typename... Types>
    static void log_vars_slave(T val, Types... vars) {
      DBG(val);
      log_file << val;
      log_vars_slave(vars...);
    }
    template <typename... Types>
    static void log_line(Types... vars) {
      DBG("DEBUG: ");
      log_file << "DEBUG: ";
      log_vars_slave(vars...);
      log_file << std::endl;
      DBG(std::endl);
    }
    template <typename... Types>
    static void err_line(Types... vars) {
      log_file << "ERROR: ";
      log_vars_slave(vars...);
      log_file << std::endl;
    }

  private:
    static std::ofstream log_file;
  };

}
