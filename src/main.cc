#include <Grid.h>
#include <Logger.h>
#include <Runner.h>

#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Required source file name argument" << std::endl;
    std::cout << "Usage: ptrain <file_name>" << std::endl;
    return 1;
  }

  std::string file_name = argv[1];

  PTrain::Grid grid(file_name);
  PTrain::Logger log;

  PTrain::Runner runner(std::make_shared<PTrain::Grid>(grid), std::make_shared<PTrain::Logger>(log));

  while (runner.step());
  std::cout << std::endl;

  return 0;
}
