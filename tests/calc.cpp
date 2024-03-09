#include <catch2/catch_all.hpp>

#include "par/Calc.h"
#include "par/parallel.h"


#include <iostream>
#include <vector>

namespace {

TEST_CASE("Calc", "[calc]") {

  SECTION("CalcCanBeInstantiated") {
    auto executor = par::Executor{4};
    auto return_function = [](){ return 3; };
    auto work = par::Calc<int()>{return_function};
    auto void_function = [](){};
    auto work2 = par::Calc<void()>{void_function};
    auto arg_function = [](int x){ };
    auto work3 = par::Calc<void(int)>{arg_function};
    auto inout_function = [](int x){ return x; };
    auto work4 = par::Calc<int(int)>{inout_function};
  }

}

} // namespace