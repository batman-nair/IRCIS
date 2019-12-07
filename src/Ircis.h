#pragma once

#include <Logger.h>
#include <Grid.h>
#include <Runner.h>

#include <string>
#include <queue>
#include <memory>

namespace Ircis {
  class Ircis {
  public:
    Ircis(std::string input_file, bool quiet_mode=false)
      : Ircis(input_file, "output.log", quiet_mode) { }
    Ircis(std::string input_file, std::string output_file, bool quiet_mode=false)
      :runner_id_(0),
       log_(std::make_shared<Logger>(output_file, quiet_mode)),
       grid_(std::make_shared<Grid>(input_file)),
       new_runners_list_(std::make_shared<std::queue<RunnerInfo> >()) {
      runner_list_.emplace_back(runner_id_++, grid_, log_, new_runners_list_);
      paths_.emplace_back();
    }
    Ircis(std::string input_file, std::shared_ptr<std::ostream> output_stream, bool quiet_mode=false)
      :runner_id_(0),
       log_(std::make_shared<Logger>(output_stream, quiet_mode)),
       grid_(std::make_shared<Grid>(input_file)),
       new_runners_list_(std::make_shared<std::queue<RunnerInfo> >()) {
      runner_list_.emplace_back(runner_id_++, grid_, log_, new_runners_list_);
      paths_.emplace_back();
    }

    // Steps all runner instances
    // Returns false when all runners are dead
    // (woah that sounds dark)
    bool update();

    void set_generate_html(std::string type="CSS") {
      generate_html_ = true;
      html_method_ = type;
    }

  private:
    int runner_id_;
    std::shared_ptr<Logger> log_;
    std::shared_ptr<Grid> grid_;
    std::vector<Runner> runner_list_;

    std::string html_method_ = "CSS";
    bool generate_html_ = false;
    std::vector<MovementData> paths_;

    std::shared_ptr<std::queue<RunnerInfo> > new_runners_list_;
  };
}
