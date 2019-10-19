#include <Grid.h>

#include <Logger.h>
#include <fstream>

namespace PTrain {

  Grid::Grid(std::string file_name) {
    read_file(file_name);
    equalize_lines();
  }

  void Grid::read_file(std::string file_name) {
    std::ifstream src_file(file_name);
    Logger::log_line("Reading file: ", file_name);
    lines_.clear();
    width_ = 0;

    std::string line;
    while (std::getline(src_file, line)) {
      lines_.push_back(line);
      width_ = std::max(width_, line.length());
    }
    height_ = lines_.size();
    Logger::log_line("Source Grid created of height ", height_, " and width ", width_);
  }

  void Grid::equalize_lines() {
    Logger::log_line("Source code: ");
    for (auto& line: lines_) {
      if (line.length() < width_) {
	line.resize(width_, '.');
      }
      Logger::log_line(" ", line);
    }
  }
}
