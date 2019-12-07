#pragma once

#include <DirVec.h>

#include <string>
#include <vector>
#include <fstream>

namespace Ircis {
  class HTMLVizBase {
  public:
    HTMLVizBase(std::string file_name, const std::vector<std::string> &lines, const std::vector<MovementData>& paths)
      : output_file_(std::ofstream(file_name)), lines_(lines), paths_(paths) { }
    virtual ~HTMLVizBase() = default;

    virtual void generate_html() = 0;
  protected:
    std::ofstream output_file_;
    std::vector<std::string> lines_;
    std::vector<MovementData> paths_;
  };

  class HTMLVizCSS : public HTMLVizBase {
  public:
    HTMLVizCSS(std::string file_name, const std::vector<std::string> &lines, const std::vector<MovementData>& paths) : HTMLVizBase(file_name, lines, paths) { };
    ~HTMLVizCSS() = default;
    void generate_html() override;
  };

  class HTMLVizJS : public HTMLVizBase {
  public:
    HTMLVizJS(std::string file_name, const std::vector<std::string> &lines, const std::vector<MovementData>& paths) : HTMLVizBase(file_name, lines, paths) { };
    ~HTMLVizJS() = default;
    void generate_html() override;
  };
}
