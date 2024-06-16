#pragma once

#include <memory>

namespace par {

struct Addressee {
  virtual void receive() = 0;
};

struct MailBox {
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
    // bubble through
    for (auto &addressee : _addressees) {
      addressee->receive();
    }
  }
  void add(std::shared_ptr<Addressee> addressee) {
    _addressees.push_back(addressee);
  }

private:
  std::vector<std::shared_ptr<Addressee>> _addressees;
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