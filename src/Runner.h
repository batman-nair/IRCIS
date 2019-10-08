#pragma once

#include <DirVec.h>
#include <DataType.h>
#include <Logger.h>
#include <Grid.h>

#include <memory>
#include <stack>
#include <queue>
#include <unordered_map>

namespace PTrain {
  enum Mode {
	     NONE = 0,
	     STACK
  };

  // String of characters that end the mode
  static std::unordered_map<Mode, std::string>
  mode_end_chars = {
		    { NONE, "" },
		    { STACK, "\"" }
  };

  bool is_mode_end_char(Mode mode, char current_char);

  class Runner {
  public:

    Runner(DirVec init_pos, std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log,
	   std::shared_ptr<std::queue<DirVec> > new_runners_list_)
      : position_(init_pos), log_(log), grid_(grid), new_runners_list_(new_runners_list_) {
      DBG("Created Runner at position " << position_);
      mode_ = Mode::NONE;
      stack_mode_ = false;
      integer_mode_ = false;
    }

    Runner(std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log,
	   std::shared_ptr<std::queue<DirVec> > new_runners_list_)
      : Runner({}, grid, log, new_runners_list_) {
    }


    // Update the Runner movement
    // Returns true if the Runner is still alive
    bool step();

  private:
    // Return Ture if character has been processed
    bool process_char(char current_char);
    bool process_integer_buffer();
    bool process_mode_buffer();

    DirVec position_;

    std::shared_ptr<Logger> log_;
    std::shared_ptr<Grid> grid_;
    std::shared_ptr<std::queue<DirVec> > new_runners_list_;

    std::stack<Data> st_;
    Mode mode_;
    bool integer_mode_;
    std::string mode_buffer_;
    std::string integer_mode_buffer_;

    bool stack_mode_;
  };
}
