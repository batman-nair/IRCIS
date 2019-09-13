#include <Grid.h>
#include <Logger.h>

#include <iostream>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Required source file name argument" << std::endl;
    std::cout << "Usage: ddrunner <file_name>" << std::endl;
    return 1;
  }

  std::string file_name = argv[1];

  PTrain::Grid grid(file_name);
  PTrain::Logger log;

  for (const auto &line : grid.get_lines()) {
    log.printl(line);
  }


  return 0;
}
