#pragma once

#include <DirVec.h>

#include <string>
#include <vector>
#include <fstream>

namespace Ircis {
  class HTMLVizBase {
  public:
    HTMLVizBase(std::string file_name, const std::vector<std::string> &lines, const std::vector<MovementData>& paths, const std::vector<std::pair<unsigned int, std::string>> time_output_data, std::string input_file_name)
      : output_file_(std::ofstream(file_name)), lines_(lines), paths_(paths), time_output_data_(time_output_data), input_file_name_(input_file_name) { }
    virtual ~HTMLVizBase() = default;

    virtual void generate_html() = 0;
    void set_animation_speed(unsigned int animation_speed) { animation_speed_ = animation_speed; }
    void use_emoji_for_html_chars() { emoji_for_html_chars_ = true; }
  protected:
    std::ofstream output_file_;
    std::vector<std::string> lines_;
    std::vector<MovementData> paths_;
    std::vector<std::pair<unsigned int, std::string>> time_output_data_;
    std::string input_file_name_;
    unsigned int animation_speed_ = 15;
    bool emoji_for_html_chars_ = false;
  };

  class HTMLVizCSS : public HTMLVizBase {
  public:
    HTMLVizCSS(std::string file_name, const std::vector<std::string> &lines, const std::vector<MovementData>& paths, const std::vector<std::pair<unsigned int, std::string>> time_output_data, std::string input_file_name)
      : HTMLVizBase(file_name, lines, paths, time_output_data, input_file_name) { };
    ~HTMLVizCSS() = default;
    void generate_html() override;
  };

  class HTMLVizJS : public HTMLVizBase {
  public:
    HTMLVizJS(std::string file_name, const std::vector<std::string> &lines, const std::vector<MovementData>& paths, const std::vector<std::pair<unsigned int, std::string>> time_output_data, std::string input_file_name)
      : HTMLVizBase(file_name, lines, paths, time_output_data, input_file_name) { };
    ~HTMLVizJS() = default;
    void generate_html() override;
  };
}
