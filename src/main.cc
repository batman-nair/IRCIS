#include <Grid.h>

#include <iostream>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Required source file name argument" << std::endl;
    std::cout << "Usage: ddrunner <file_name>" << std::endl;
    return 1;
  }

  std::string file_name = argv[1];

  Grid grid(file_name);

  for (const auto &line : grid.get_lines()) {
    std::cout << line << std::endl;
  }

  return 0;
}
