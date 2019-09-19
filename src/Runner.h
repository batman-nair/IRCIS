#pragma once

#include <DirVec.h>
#include <DataType.h>
#include <Logger.h>
#include <Grid.h>

#include <memory>
#include <stack>
#include <queue>

namespace PTrain {

  class Runner {
  public:
    Runner(std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log,
	   std::shared_ptr<std::queue<DirVec> > new_runners_list_)
      : position_(), log_(log), grid_(grid), new_runners_list_(new_runners_list_) {
      DBG("Created Runner at position " << position_);
      stack_mode_ = false;
      integer_mode_ = false;
    }

    Runner(DirVec init_pos, std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log)
      : position_(init_pos), log_(log), grid_(grid) {
      DBG("Created Runner at position " << position_);
      stack_mode_ = false;
      integer_mode_ = false;

    }

    // Update the Runner movement
    // Returns true if the Runner is still alive
    bool step();

  private:
    DirVec position_;

    std::shared_ptr<Logger> log_;
    std::shared_ptr<Grid> grid_;
    std::shared_ptr<std::queue<DirVec> > new_runners_list_;

    std::stack<Data> st_;

    bool stack_mode_;
    bool integer_mode_;
  };
}
