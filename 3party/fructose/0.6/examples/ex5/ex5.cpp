// Copyright (c) 2007 Andrew Peter Marlow.
// All rights reserved.

#include "fructose/test_base.h"
#include <stdexcept>
#include <vector>

struct timestest : 
  public fructose::test_base<timestest> {
    void array_bounds(const std::string& test_name) {
      std::vector<int> v;
      v.push_back(1234);
      fructose_assert_exception(v.at(2), std::out_of_range);
    };
};

int main(int argc, char* argv[]) {
  timestest tests;
  tests.add_test("array_bounds", &timestest::array_bounds);
  return tests.run(argc, argv);
}
