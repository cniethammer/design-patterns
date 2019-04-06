#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "string_utils.h"

using namespace string_utils;

/* tests for join */
TEST_CASE("join(vec({1,2,3,3}), ', ') returns '1, 2, 3, 3'", "[join]") {
  std::string str = join(std::vector<int>({1, 2, 3, 3}), ", ");
  REQUIRE(str == "1, 2, 3, 3");
}

TEST_CASE("join(str({'path1','path2'}), ':') returns 'path1:path2'", "[join]") {
  std::string str = join(std::vector<std::string>({"path1", "path2"}), ":");
  REQUIRE(str == "path1:path2");
}

/* tests for string_to_type */
TEST_CASE("string_to_type<int>('100') returns 100", "[string_to_type]") {
  REQUIRE(string_to_type<int>("100") == 100);
}

TEST_CASE("string_to_type<double>('3.14') returns 3.14", "[string_to_type]") {
  REQUIRE(string_to_type<double>("3.14") == 3.14);
}

TEST_CASE("string_to_type<string>(' Hello ') returns 'Hello'",
          "[string_to_type]") {
  REQUIRE(string_to_type<std::string>(" Hello ") == std::string("Hello"));
}

/* tests for split */
TEST_CASE("split<string>(empty string, delimiter) returns ['']", "[split]") {
  auto vec = split<std::string>("", "test");
  REQUIRE(vec.size() == 1);
  REQUIRE(vec[0] == std::string());
}

TEST_CASE("split<string>(str, str) returns ['', '']", "[split]") {
  auto vec = split<std::string>("test", "test");
  REQUIRE(vec.size() == 2);
  REQUIRE(vec[0] == std::string());
  REQUIRE(vec[1] == std::string());
}

TEST_CASE("split<string>(Hello World!) returns ['Hello', 'World']", "[split]") {
  auto vec = split<std::string>("Hello World!");
  REQUIRE(vec.size() == 2);
  REQUIRE(vec[0] == std::string("Hello"));
  REQUIRE(vec[1] == std::string("World!"));
}

TEST_CASE("split<int>(empty string, delimiter) returns [0]", "[split]") {
  auto vec = split<int>("", ",");
  REQUIRE(vec.size() == 1);
  REQUIRE(vec[0] == 0);
}

TEST_CASE("split<int>(' 1,2,3 ',',') returns [1,2,3]", "[split]") {
  auto vec = split<int>(" 1,2,3 ", ",");
  REQUIRE(vec[0] == 1);
  REQUIRE(vec[1] == 2);
  REQUIRE(vec[2] == 3);
  REQUIRE(vec.size() == 3);
}
