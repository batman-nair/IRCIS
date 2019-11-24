#include <gtest/gtest.h>
#include <Ircis.h>
#include <fstream>

namespace Ircis {
  class SystemTests : public ::testing::Test {
  protected:
    void SetUp() override {

    }

    // void TearDown() override {}
    Ircis hello_world_{"hello_world.txt", "hello_world_test.log", true};

  };

  TEST_F(SystemTests, HelloWorld) {
    while (hello_world_.update());
    std::ifstream t("hello_world_test.log");
    std::stringstream buffer;
    buffer << t.rdbuf();
    ASSERT_EQ("Hello World!\n \n", buffer.str());
  }
}
