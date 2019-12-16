#pragma once

#include <DirVec.h>

#include <string>
#include <vector>
#include <fstream>

namespace Ircis {
  class HTMLVizBase {
  public:
    HTMLVizBase(std::string file_name, const std::vector<std::string> &lines, const std::vector<MovementData>& paths, const std::vector<std::pair<unsigned int, std::string>> time_output_data)
      : output_file_(std::ofstream(file_name)), lines_(lines), paths_(paths), time_output_data_(time_output_data) { }
    virtual ~HTMLVizBase() = default;

    virtual void generate_html() = 0;
  protected:
    std::ofstream output_file_;
    std::vector<std::string> lines_;
    std::vector<MovementData> paths_;
    std::vector<std::pair<unsigned int, std::string>> time_output_data_;
  };

  class HTMLVizCSS : public HTMLVizBase {
  public:
    HTMLVizCSS(std::string file_name, const std::vector<std::string> &lines, const std::vector<MovementData>& paths, const std::vector<std::pair<unsigned int, std::string>> time_output_data)
      : HTMLVizBase(file_name, lines, paths, time_output_data) { };
    ~HTMLVizCSS() = default;
    void generate_html() override;
  };

  class HTMLVizJS : public HTMLVizBase {
  public:
    HTMLVizJS(std::string file_name, const std::vector<std::string> &lines, const std::vector<MovementData>& paths, const std::vector<std::pair<unsigned int, std::string>> time_output_data)
      : HTMLVizBase(file_name, lines, paths, time_output_data) { };
    ~HTMLVizJS() = default;
    void generate_html() override;
  };
}
