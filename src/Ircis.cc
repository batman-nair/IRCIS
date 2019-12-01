#include <Ircis.h>

#include <HTMLViz.h>

namespace Ircis {
  bool Ircis::update() {
      bool keep_moving = false;
      for (auto it = runner_list_.begin(); it != runner_list_.end(); ++it) {
	if (it->step()) {
	  keep_moving = true;
	}
	else {
	  Logger::log_line("Runner ", it->get_id(), " died.");
	  it->run_debug();
	  paths_.push_back(it->get_path());
	  runner_list_.erase(it--); /* iter-- so the loop proceeds properly */
	}
      }

      while (!new_runners_list_->empty()) {
	Logger::log_line("Adding new Runner");
	keep_moving = true;
	auto new_runner_info = new_runners_list_->front();
	runner_list_.emplace_back(runner_id_++, new_runner_info.position, grid_, log_, new_runners_list_, new_runner_info.st, new_runner_info.path);
	Logger::log_line("Added new runner to list, Runner ", runner_id_);
	new_runners_list_->pop();
      }

      if (!keep_moving) {
	log_->print_line(" ");
	Logger::log_line_dbg("Ircis has finished running!");

	if (generate_html_) {
	  HTMLViz html_viz = HTMLViz("output.html", grid_->get_lines(), paths_);
	  html_viz.generate_html();
	}

      }
      return keep_moving;
  }
}
