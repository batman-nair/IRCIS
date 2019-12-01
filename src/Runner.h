#pragma once

#include <CharMaps.h>
#include <DataType.h>
#include <RunnerStack.h>
#include <DirVec.h>
#include <Logger.h>
#include <Grid.h>

#include <memory>
#include <stack>
#include <queue>
#include <unordered_map>

namespace Ircis {
  enum Mode {
	     NONE = 0,
	     STACK,
	     STACK_POP,
	     STACK_PUSH
  };

  // String of characters that end the mode
  static std::unordered_map<Mode, std::string>
  mode_end_chars = {
		    { NONE, "" },
		    { STACK, { CH_STACK } },
		    { STACK_PUSH, { CH_DOT, CH_SPC } },
		    { STACK_POP, { CH_DOT, CH_SPC } }
  };

  bool is_mode_end_char(Mode mode, char current_char);

  struct RunnerInfo {
    DirVec position;
    RunnerStack st;
    std::vector<DirVec> path;
  };

  class Runner {
  public:

    Runner(int id, DirVec init_pos, std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log,
	   std::shared_ptr<std::queue<RunnerInfo> > new_runners_list)
      : id_(id), position_(init_pos), log_(log), grid_(grid), new_runners_list_(new_runners_list) {
      Logger::log_line("Created Runner at position ", position_);
      mode_ = Mode::NONE;
      stack_mode_ = false;
      integer_mode_ = false;
    }
    Runner(int id, DirVec init_pos, std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log,
	   std::shared_ptr<std::queue<RunnerInfo> > new_runners_list, RunnerStack st)
      : Runner(id, init_pos, grid, log, new_runners_list) {
      st_ = st;
    }
    Runner(int id, DirVec init_pos, std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log,
	   std::shared_ptr<std::queue<RunnerInfo> > new_runners_list, RunnerStack st,
	   std::vector<DirVec> path)
      : Runner(id, init_pos, grid, log, new_runners_list, st) {
      path_ = path;
    }

    Runner(int id, std::shared_ptr<Grid> grid, std::shared_ptr<Logger> log,
	   std::shared_ptr<std::queue<RunnerInfo> > new_runners_list)
      : Runner(id, {}, grid, log, new_runners_list) {
    }

    // Update the Runner movement
    // Returns true if the Runner is still alive
    bool step();
    int get_id() { return id_; }
    void run_debug();

    std::vector<DirVec> get_path() {
      return path_;
    }

  private:
    // Return Ture if character has been processed
    bool process_char(char current_char);
    bool process_integer_buffer();
    bool process_mode_buffer();
    bool process_split();

    template <typename T>
    std::string get_string_from_vars(T val);
    template <typename T, typename... Types>
    std::string get_string_from_vars(T val, Types... vars);
    template <typename... Types>
    void set_error(Types... vars);

    // Wrapper over Logger functions with Runner name prints
    template <typename ...Types>
    void log_line(Types... vars) {
      Logger::log_line("Runner ", get_id(), ": ", vars...);
    }
    template <typename ...Types>
    void err_line(Types... vars) {
      Logger::err_line("Runner ", get_id(), ": ", vars...);
    }

    int id_ = 0;
    DirVec position_;
    std::vector<DirVec> path_;

    std::shared_ptr<Logger> log_;
    std::shared_ptr<Grid> grid_;
    std::shared_ptr<std::queue<RunnerInfo> > new_runners_list_;

    RunnerStack st_;
    Mode mode_;
    bool integer_mode_;
    std::string mode_buffer_;
    std::string integer_mode_buffer_;

    bool stack_mode_;
    std::string err_str_;
    std::string processed_chars_;
  };
}
