#include <gtest/gtest.h>
#include <Grid.h>

namespace Ircis {
  class GridTest : public ::testing::Test {
  protected:
    void SetUp() override {

    }

    // void TearDown() override {}

    Grid file_test_{"hello_world.txt"};
  };

  TEST_F(GridTest, NullTest) {
    try {
      Grid test("nonexistent_text_file.txt");
      FAIL() << "File read error not thrown";
    } catch (Grid::GridFileNotFoundException& ex) {
      ASSERT_STREQ("File nonexistent_text_file.txt not found", ex.what());
    } catch (...) {
      FAIL() << "Random exception occured while reading file";
    }
  }

  TEST_F(GridTest, AccessTest) {
    EXPECT_EQ('v', file_test_.get(0, 0));
    EXPECT_EQ('W', file_test_.get(8, 1));
    EXPECT_EQ('.', file_test_.get(24, 0));
    EXPECT_EQ('.', file_test_.get(24, 3));
    EXPECT_EQ(false, file_test_.is_inside(24, 4));
    EXPECT_EQ(true, file_test_.is_inside(2, 2));
    try {
      file_test_.get(24, 4);
      FAIL() << "Grid getter doesn't have bound checking";
    } catch (Grid::GridOutOfBoundsException& ex) {
      EXPECT_STREQ("Point (24, 4) is outside grid", ex.what());
    } catch (...) {
      FAIL() << "Random exception occured while calling getter";
    }
  }
}
