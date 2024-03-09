#include <catch2/catch_all.hpp>

#include "par/ContinuableWork.h"
#include "par/parallel.h"


#include <iostream>
#include <vector>

namespace {

TEST_CASE("ContinuableTask", "[continuable_task]") {

  SECTION("ContinuableTaskThenExecutesAllFunctionsOfTheChain") {
    auto executor = par::Executor{4};
    auto return_function = [](){ return 3; };
    auto work = par::Calc1<int()>{return_function};
    auto void_function = [](){};
    auto work2 = par::Calc<void()>{void_function};
    auto arg_function = [](int x){ };
    auto work3 = par::Calc1<void(int)>{arg_function};
    auto inout_function = [](int x){ return x; };
    auto work4 = par::Calc2<int(int)>{inout_function};
  }

}

} // namespace