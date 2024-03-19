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
  SECTION("CalcThenWorks1"){
    auto executor = par::Executor{4};
    auto return_function = [](){ return 42; };
    auto inout_function = [](int i){ return ++i; };
    auto work = par::Calc<int()>{return_function};
    auto work2 = work.then<int>(executor, inout_function);
    auto work3 = work2.then<int>(executor, inout_function);
    executor.run(work.make_task());
    executor.wait_for(work3.make_task());
    CHECK(work.is_finished());
    CHECK(work2.is_finished());
    CHECK(work3.is_finished());
    CHECK(work3.result() == 44);
  }
  
  SECTION("CalcThenWorks2"){
    auto executor = par::Executor{4};
    auto return_function = [](){ return 42; };
    int result = 0;
    auto inout_function = [](int i){ return ++i; };
    auto void_function = [&result](int i){ result = i + 1; };
    auto work = par::Calc<int()>{return_function};
    auto work2 = work.then<int>(executor, inout_function);
    auto work3 = work2.then(executor, void_function);
    executor.run(work.make_task());
    executor.wait_for(work3.make_task());
    CHECK(work.is_finished());
    CHECK(work2.is_finished());
    CHECK(work3.is_finished());
    CHECK(result == 44);
  }

  SECTION("LoadTest"){
    auto executor = par::Executor{4};
    auto return_function = [](){ return 42; };
    auto inout_function = [](int i){ return ++i; };
    std::vector<par::Calc<int()>> initial_works;
    std::vector<par::Calc<int()>> final_works;
    for (size_t i = 0; i < 10; i++){
      auto work = par::Calc<int()>{return_function};
      initial_works.push_back(work);
      auto previous_work = work;
      for(size_t j = 0; j < 5; ++j){
        auto work_then = previous_work.then<int>(executor, inout_function);
        previous_work = work_then;
        if(j >= 4){
          final_works.push_back(work_then);
        }
      }
    }
    for (auto& work : initial_works){
      executor.run(work.make_task());
    }
    for (auto& work : final_works){
      executor.wait_for(work.make_task());
      CHECK(work.is_finished());
    }
    // check work is finished
    for(auto& work : initial_works){
      CHECK(work.is_finished());
    }
    for(auto& work : final_works){
      CHECK(work.is_finished());
    }
    // check results
    for(auto& work : final_works){
      CHECK(work.result() == 1042);
    }
  }

}

} // namespace