#include <Grid.h>
#include <Logger.h>
#include <Ircis.h>
#include <getopt.h>
#include <cxxopts.hpp>

#include <iostream>
#include <string>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  cxxopts::Options options("IRCIS", "2D grid based esoteric programming language");
  options.add_options()
    ("i,input", "Input file list", cxxopts::value<std::vector<std::string>>())
    ("s,speed", "Set the speed for generated animation", cxxopts::value<int>()->default_value("15"))
    ("x,startx", "Starting x position", cxxopts::value<int>()->default_value("0"))
    ("y,starty", "Starting y position", cxxopts::value<int>()->default_value("0"))
    ("d,direction", "Specify the starting direction", cxxopts::value<std::string>()->default_value("E"))
    ("h,help", "Print usage help")
    ;
  options.parse_positional({"input"});
  options.positional_help("Input files to parse");

  auto result = options.parse(argc, argv);

  if (result.count("help") || !result.count("input")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }

  int startx = result["startx"].as<int>();
  int starty = result["starty"].as<int>();
  char direction = result["direction"].as<std::string>().front();
  int speed = result["speed"].as<int>();
  std::vector<std::string> input_files = result["input"].as<std::vector<std::string>>();

  for (std::string& file_name: input_files) {
    Ircis::Ircis ircis(file_name, startx, starty, direction);
    ircis.set_generate_html("CSS");
    ircis.set_animation_speed(speed);
    while (ircis.update());
  }

  Ircis::Logger::log_line_dbg("Program has finished running");

  return 0;
}
