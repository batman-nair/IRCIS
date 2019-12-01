#pragma once

#include <DirVec.h>

#include <string>
#include <vector>
#include <fstream>

namespace Ircis {
  typedef std::vector<DirVec> path;

  class HTMLViz {
  public:
  HTMLViz(std::string file_name, const std::vector<std::string> &lines, const std::vector<path>& paths)
    : output_file_(std::ofstream(file_name)), lines_(lines), paths_(paths) { }

    void generate_html();
  private:
    std::ofstream output_file_;
    std::vector<std::string> lines_;
    std::vector<path> paths_;

  };
}
