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
    DBG("Reading file: " << file_name);
    lines_.clear();
    length_ = 0;

    std::string line;
    while (std::getline(src_file, line)) {
      lines_.push_back(line);
      length_ = std::max(length_, line.length());
    }
    width_ = lines_.size();
    DBG("Source Grid created of length " << length_ << " and width " << width_);
  }

  void Grid::equalize_lines() {
    DBG("Source code: ");
    for (auto& line: lines_) {
      if (line.length() < length_) {
	line.resize(length_, '.');
      }
      DBG(" " << line);
    }
  }
}
