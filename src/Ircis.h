#pragma once

#include <Logger.h>
#include <Grid.h>
#include <Runner.h>

#include <string>
#include <queue>
#include <memory>
#include <sstream>


namespace Ircis {
  class Ircis {
  public:
    Ircis(std::string input_file, bool quiet_mode=false)
      : Ircis(input_file, "output.log", quiet_mode) { }
    Ircis(std::string input_file, std::string output_file, bool quiet_mode=false)
      :runner_id_(0),
       log_(std::make_shared<Logger>(output_file, quiet_mode)),
       grid_(std::make_shared<Grid>(input_file)),
       global_var_map_(std::make_shared<variable_map_t>()),
       input_file_name_(input_file),
       new_runners_list_(std::make_shared<std::queue<RunnerInfo> >()) {
      runner_list_.emplace_back(runner_id_++, grid_, log_, global_var_map_, new_runners_list_);
      paths_.emplace_back();
    }
    Ircis(std::string input_file, std::shared_ptr<std::ostream> output_stream, bool quiet_mode=false)
      :runner_id_(0),
       log_(std::make_shared<Logger>(output_stream, quiet_mode)),
       grid_(std::make_shared<Grid>(input_file)),
       global_var_map_(std::make_shared<variable_map_t>()),
       input_file_name_(input_file),
       new_runners_list_(std::make_shared<std::queue<RunnerInfo> >()) {
      runner_list_.emplace_back(runner_id_++, grid_, log_, global_var_map_, new_runners_list_);
      paths_.emplace_back();
    }

    // Steps all runner instances
    // Returns false when all runners are dead
    // (woah that sounds dark)
    bool update();

    void set_generate_html(std::string type="CSS") {
      output_log_stream_ = std::make_shared<std::ostringstream>();
      log_->set_output_stream(output_log_stream_);
      generate_html_ = true;
      html_method_ = type;
    }

  private:
    int runner_id_;
    std::shared_ptr<Logger> log_;
    std::shared_ptr<Grid> grid_;
    std::shared_ptr<variable_map_t> global_var_map_;
    std::vector<Runner> runner_list_;

    std::string input_file_name_ = "";
    std::string html_method_ = "CSS";
    bool generate_html_ = false;
    std::vector<MovementData> paths_;
    std::vector<std::pair<unsigned int, std::string>> time_output_data_;
    std::shared_ptr<std::ostringstream> output_log_stream_;

    std::shared_ptr<std::queue<RunnerInfo> > new_runners_list_;
  };
}
