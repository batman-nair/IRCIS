#pragma once

#include <Logger.h>
#include <Grid.h>
#include <Runner.h>

#include <string>
#include <queue>
#include <memory>

namespace PTrain {
  class PTrain {
  public:
    PTrain(std::string file_name)
      :grid_(std::make_shared<Grid>(file_name)), log_(std::make_shared<Logger>()),
      new_runners_list_(std::make_shared<std::queue<DirVec> >()) {
      runner_list_.emplace_back(grid_, log_, new_runners_list_);
    }

    PTrain(std::unique_ptr<Grid> grid, std::unique_ptr<Logger> log)
      : grid_(std::move(grid)), log_(std::move(log)) {
      runner_list_.emplace_back(grid_, log_, new_runners_list_);
    }

    // Steps all runner instances
    // Returns false when all runners are dead
    // (woah that sounds dark)
    bool update();

  private:
    std::shared_ptr<Grid> grid_;
    std::shared_ptr<Logger> log_;
    std::vector<Runner> runner_list_;

    std::shared_ptr<std::queue<DirVec> > new_runners_list_;
  };
}
