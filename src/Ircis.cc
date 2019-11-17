#include <Ircis.h>

namespace Ircis {
  bool Ircis::update() {
      bool keep_moving = false;
      for (auto it = runner_list_.begin(); it != runner_list_.end(); ++it) {
	if (it->step()) {
	  Logger::log_line("Runner ", it->get_id(), " updated.");
	  keep_moving = true;
	}
	else {
	  Logger::log_line("Runner ", it->get_id(), " died.");
	  it->run_debug();
	  runner_list_.erase(it--); /* iter-- so the loop proceeds properly */
	}
      }

      while (!new_runners_list_->empty()) {
	Logger::log_line("Adding new Runner");
	keep_moving = true;
	auto new_runner_info = new_runners_list_->front();
	Logger::log_line("Got new runner info", runner_id_);
	runner_list_.emplace_back(runner_id_++, new_runner_info.position, grid_, log_, new_runners_list_, new_runner_info.st);
	Logger::log_line("Added new runner to list", runner_id_);
	new_runners_list_->pop();
	Logger::log_line("New runner added by ");
      }

      if (!keep_moving) {
	log_->print_line(" ");
	Logger::log_line_dbg("Ircis has finished running!");
      }
      return keep_moving;
  }
}
