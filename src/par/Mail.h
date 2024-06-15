#pragma once

#include <memory>

namespace par {

struct AddresseeImpl {
  virtual void receive() = 0;
};

struct MailBox {
  void arrived() {
    // remove deleted addressees
    auto it =
        std::remove_if(_addressees.begin(), _addressees.end(),
                       [](const std::shared_ptr<AddresseeImpl> &addressee) {
                         if (addressee.use_count() == 1) {
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
  void add(std::shared_ptr<AddresseeImpl> addressee) {
    _addressees.push_back(addressee);
  }

private:
  std::vector<std::shared_ptr<AddresseeImpl>> _addressees;
};

struct Addressee {
  Addressee(std::shared_ptr<AddresseeImpl> addressee_impl)
      : _addressee_impl{addressee_impl} {}
  Addressee() = default;
  Addressee(Addressee &&) = default;
  Addressee &operator=(Addressee &&) = default;
  Addressee(const Addressee &) = default;
  Addressee &operator=(const Addressee &) = default;

  virtual ~Addressee() = default;

  std::shared_ptr<AddresseeImpl> get() const { return _addressee_impl; }
  void expect(MailBox &mail_box) { mail_box.add(_addressee_impl); }
  
private:
  std::shared_ptr<AddresseeImpl> _addressee_impl = nullptr;
};

} // namespace par