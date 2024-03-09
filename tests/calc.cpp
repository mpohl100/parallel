#include <catch2/catch_all.hpp>

#include "par/Calc.h"
#include "par/Executor.h"


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
  SECTION("CalcThenWorks"){
    auto executor = par::Executor{4};
    auto return_function = [](){ return 42; };
    auto inout_function = [](int i){ return ++i; };
    auto work = par::Calc<int()>{return_function};
    auto work2 = work.then<int>(executor, inout_function);
    executor.run(work.make_task());
    executor.wait_for(work2.make_task());
    CHECK(work.is_finished());
    CHECK(work2.is_finished());
    CHECK(work2.result() == 43);
  }

}

} // namespace