#include <catch2/catch_all.hpp>

#include "par/TaskGraph.h"
#include "par/Executor.h"
#include "par/Calc.h"

namespace {

TEST_CASE("TaskGraph", "[taskgraph]") {

  SECTION("EmptyTaskGraphFinishes") {
    auto task_graph = par::TaskGraph{};
    auto executor = par::Executor{4};
    executor.run(task_graph);
    executor.wait_for(task_graph);
    CHECK(true);
  }
  SECTION("TaskGraphWithOneTaskFinishes"){
    auto task_graph = par::TaskGraph{};
    auto executor = par::Executor{4};
    bool finished = false;
    auto task = par::Calculation{[&finished]() -> void {
      finished = true;
    }}.make_task();
    task_graph.add_task(task);
    executor.run(task_graph);
    executor.wait_for(task_graph);
    CHECK(finished);
  }
  SECTION("TaskGraphFinishesChainOfCalcs"){
    auto executor = par::Executor{4};
    auto first_calc = par::Calc<int()>{[]() -> int {
      return 42;
    }};
    auto second_calc = first_calc.then<int>(executor, [](int x) -> int {
      return x + 1;
    });
    auto third_calc = second_calc.then<int>(executor, [](int x) -> int {
      return x + 1;
    });
    
    auto task_graph = par::TaskGraph{};
    task_graph.add_task(first_calc.make_task());
    task_graph.add_task(second_calc.make_task());
    task_graph.add_task(third_calc.make_task());
    executor.run(task_graph);
    executor.wait_for(task_graph);
    CHECK(third_calc.result() == 44);
  }
}

} // namespace
