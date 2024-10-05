#pragma once

#include "par/Task.h"
#include "par/Calc.h"

#include <vector>

namespace par{

struct TaskGraph{
    TaskGraph() = default;
    TaskGraph(const TaskGraph &) = default;
    TaskGraph(TaskGraph &&) = default;
    TaskGraph &operator=(const TaskGraph &) = default;
    TaskGraph &operator=(TaskGraph &&) = default;
    
    void add_task(Task task){
        _tasks.front().succeeds(task);
        _tasks.push_back(task);
    }
private:
    Task create_dummy_finish_task(){
        return Calc<void()>{[]() -> void {}}.make_task();
    }

    std::vector<Task> _tasks = {create_dummy_finish_task()};
};
}

}