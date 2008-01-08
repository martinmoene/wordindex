// Copyright (c) 2007 Andrew Peter Marlow.
// All rights reserved.

#include "fructose/test_base.h"
#include <cmath>

struct simpletest : 
  public fructose::test_base<simpletest> {
    void floating(const std::string& test_name) {
      double mypi = 4.0 * std::atan(1.0);
      fructose_assert_double_eq(M_PI, mypi);
    }
};

int main(int argc, char* argv[]) {
  simpletest tests;
  tests.add_test("float", &simpletest::floating);
  return tests.run(argc, argv);
}

