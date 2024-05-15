#include <catch2/catch_all.hpp>

#include "par/Poll.h"


#include <iostream>
#include <vector>

namespace {

TEST_CASE("Poll", "[poll]") {

  SECTION("PollFinishes") {
    auto do_finally = std::function<void(int)>([](int poll_result){
        std::cout << "Poll finished with result: " << poll_result << std::endl;
        CHECK(poll_result == 42);
    });
    auto executor = par::Executor{4};
    auto func_to_wait_for = std::function<int()>([]() -> int {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 42;
    });
    auto poll = par::Poll{executor, par::Calc<int()>{func_to_wait_for}, do_finally};
    poll.poll();
    auto fail_finally = std::function<void(int)>([](int fail_result){
        std::cout << "Fail finished with result: " << fail_result << std::endl;
        CHECK(fail_result == 31);
        CHECK(false);
    });
    auto func_to_never_finish = std::function<int()>([]() -> int {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        return 31;
    });
    auto poll_fail = par::Poll{executor, par::Calc<int()>{func_to_never_finish}, fail_finally};
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
  
}
  
}   // namespace