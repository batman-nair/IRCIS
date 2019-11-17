#include <Grid.h>
#include <Logger.h>
#include <Ircis.h>

#include <iostream>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Required source file name argument" << std::endl;
    std::cout << "Usage: ircis <file_name>" << std::endl;
    return 1;
  }

  std::string file_name = argv[1];

  Ircis::Ircis ircis(file_name);
  while (ircis.update());

  Ircis::Logger::log_line_dbg("Program has finished running");

  return 0;
}
