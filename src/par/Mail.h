#pragma once

#include "Executor.h"
#include "Calc.h"

#include <memory>

namespace par {

struct Addressee {
  virtual void receive() = 0;
};

struct MailBox {
  MailBox(std::shared_ptr<Executor> executor) : _executor{executor} {}
  MailBox() = default;
  MailBox(MailBox &&) = default;
  MailBox &operator=(MailBox &&) = default;
  MailBox(const MailBox &) = default;
  MailBox &operator=(const MailBox &) = default;
  void arrived() {
    // remove deleted Orchestrators
    auto it =
        std::remove_if(_addressees.begin(), _addressees.end(),
                       [](const std::shared_ptr<Addressee> &Orchestrator) {
                         if (Orchestrator.use_count() == 1) {
                           return true;
                         }
                         return false;
                       });
    _addressees = {_addressees.begin(), it};

    // synchronous updates
    if(!_executor) {
      for (auto &addressee : _addressees) {
        addressee->receive();
      }
      return;
    }
    // asynchronous updates
    for (auto &addressee : _addressees) {
      const auto func = [addressee](){ addressee->receive(); };
      const auto calc = Calc<void()>{func};
      _executor->run(calc.make_task());
    }
  }
  void add(std::shared_ptr<Addressee> addressee) {
    _addressees.push_back(addressee);
  }

private:
  std::vector<std::shared_ptr<Addressee>> _addressees;
  std::shared_ptr<Executor> _executor = nullptr;
};

struct Orchestrator {
  Orchestrator(std::shared_ptr<Addressee> Orchestrator_impl)
      : _addressee{Orchestrator_impl} {}
  Orchestrator() = default;
  Orchestrator(Orchestrator &&) = default;
  Orchestrator &operator=(Orchestrator &&) = default;
  Orchestrator(const Orchestrator &) = default;
  Orchestrator &operator=(const Orchestrator &) = default;

  virtual ~Orchestrator() = default;

  std::shared_ptr<Addressee> get() const { return _addressee; }
  void expect(MailBox &mail_box) { mail_box.add(_addressee); }
  
private:
  std::shared_ptr<Addressee> _addressee = nullptr;
};

} // namespace par