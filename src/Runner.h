#pragma once

#include <DirVec.h>
#include <DataType.h>
#include <Logger.h>
#include <Grid.h>

#include <memory>
#include <stack>

namespace PTrain {

  class Runner {
  public:
    Runner(std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log)
      : position(), log(log), grid(grid) {
      DBG("Created Runner at position " << position);
      stack_mode = false;
      integer_mode = false;
    }

    Runner(DirVec init_pos, std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log)
      : position(init_pos), log(log), grid(grid) {
      DBG("Created Runner at position " << position);
      stack_mode = false;
      integer_mode = false;

    }

    // Update the Runner movement
    // Returns true if the Runner is still alive
    bool step();

  private:
    DirVec position;

    std::shared_ptr<Logger> log;
    std::shared_ptr<Grid> grid;

    std::stack<Data> st;

    bool stack_mode;
    bool integer_mode;
  };
}
