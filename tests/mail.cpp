#include <catch2/catch_all.hpp>

#include "par/Executor.h"
#include "par/Mail.h"

#include <iostream>
#include <functional>
#include <vector>

namespace {

TEST_CASE("Mail", "[mail]") {

  SECTION("MailCanBeSetup") {
    bool received = false;
    const auto on_received = [&received]() { received = true; };
    struct TestAddressee : public par::AddresseeImpl {
      TestAddressee(std::function<void()> on_received) : _on_received{on_received} {}
      void receive() override { _on_received(); }
      std::function<void()> _on_received;
    };
    auto mailbox = par::MailBox{};
    {
      auto my_addressee = std::make_shared<TestAddressee>(std::function<void()>{on_received});
      auto addressee = par::Addressee{my_addressee};
      addressee.expect(mailbox);
      mailbox.arrived();
      CHECK(received);
      received = false;
    }
    // addressee is destructed, so nothing should be received
    mailbox.arrived();
    CHECK(!received);
  }
}

} // namespace
