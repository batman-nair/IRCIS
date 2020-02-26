#include <Grid.h>
#include <Logger.h>
#include <Ircis.h>
#include <getopt.h>

#include <iostream>
#include <string>
#include <stdlib.h>

static int parseInt(std::string str, std::string descr) {
  int to;
  try {
    to = std::stoi(str);
  } catch (std::invalid_argument e) {
		std::cerr << "Invalid value for " << descr << ": \"" << str << "\". "
		      << "Expected an integer value." << std::endl;
		exit(EXIT_FAILURE);
	}
  return to;
}
static char parseDir(std::string str) {
  char to = std::toupper( str.front() ) ;
  std::string valid = "NSEW";
  if (valid.find(to) == std::string::npos) {
		std::cerr << "Invalid value for direction: \"" << str << "\". "
		      << "Expected N, S, E or W." << std::endl;
		exit(EXIT_FAILURE);
	}
  return to;
}


int main(int argc, char *argv[]) {
  intmax_t startx;
  intmax_t starty;
  char direction;
  /* Default values */
  startx = 0;
  starty = 0;
  direction = 'E';

  static struct option const long_options[] =
  {
    {"startx", required_argument, NULL, 'x'},
    {"starty", required_argument, NULL, 'y'},
    {"direction", required_argument, NULL, 'd'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  while (true)
  {
    const auto opt = getopt_long(argc, argv, "x:y:d:", long_options, &option_index);

    if (-1 == opt)
      break;

    switch (opt)
    {
      case 'x':
        //startx = std::stoi(optarg);
        startx = parseInt(optarg, "startx");
        break;
      case 'y':
        starty = parseInt(optarg, "starty");
        break;
      case 'd':
        direction = parseDir(optarg);
        break;
      case 0:
        break;
    }
  }

  if (optind < argc)
  {
    while (optind < argc) 
    {
      std::string file_name = argv[optind++];
      Ircis::Ircis ircis(file_name, startx, starty, direction);
      ircis.set_generate_html("CSS");
      while (ircis.update());
    }
  }
  else 
  {
    std::cout << "Required source file name argument" << std::endl;
    std::cout << "Usage: ircis <file_name> <option(s)>\n"
      << "Options:\n"
      << "\t-x,--startx num\t\tStarting x position (Default: 0)\n"
      << "\t-y,--starty num\t\tStarting y position (Default: 0)\n"
      << "\t-d,--direction [NSEW]\tSpecify the starting direction (Default: E)"
      << std::endl;
    exit(EXIT_FAILURE);
  }


  Ircis::Logger::log_line_dbg("Program has finished running");

  return 0;
}
