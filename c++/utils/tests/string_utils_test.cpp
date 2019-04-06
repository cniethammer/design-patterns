#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "string_utils.h"

using namespace string_utils;


/* tests for join */
TEST_CASE("join(vec({1,2,3}), ', ') returns '1, 2, 3'", "[join]") {
  std::string str = join(std::vector<int>({1, 2, 3}), ", ");
  REQUIRE(str == "1, 2, 3");
}

TEST_CASE("join(str({'path1','path2'}), ':') returns 'path1:path2'", "[join]") {
  std::string str = join(std::vector<std::string>({"path1", "path2"}), ":");
  REQUIRE(str == "path1:path2");
}

