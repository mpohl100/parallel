#include <catch2/catch_all.hpp>

#include "par/Executor.h"
#include "par/Mail.h"

#include <functional>
#include <iostream>
#include <vector>

namespace {

struct TestAddressee : public par::Addressee {
  TestAddressee(std::function<void()> on_received)
      : _on_received{on_received} {}
  void receive() override { _on_received(); }
  std::function<void()> _on_received;
};

TEST_CASE("Mail", "[mail]") {

  SECTION("MailWorksSynchronously") {
    bool received = false;
    const auto on_received = [&received]() { received = true; };
    auto mailbox = par::MailBox{};
    {
      auto addressee =
          std::make_shared<TestAddressee>(std::function<void()>{on_received});
      auto orchestrator = par::Orchestrator{addressee};
      orchestrator.expect(mailbox);
      mailbox.arrived();
      CHECK(received);
      received = false;
    }
    // Orchestrator is destructed, so nothing should be received
    mailbox.arrived();
    CHECK(!received);
  }
  SECTION("MailWorksAsynchronously") {
    bool received = false;
    const auto on_received = [&received]() { received = true; };
    auto executor = std::make_shared<par::Executor>(1);
    auto mailbox = par::MailBox{executor};
    {
      auto addressee =
          std::make_shared<TestAddressee>(std::function<void()>{on_received});
      auto orchestrator = par::Orchestrator{addressee};
      orchestrator.expect(mailbox);
      mailbox.arrived();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      CHECK(received);
      received = false;
    }
    // Orchestrator is destructed, so nothing should be received
    mailbox.arrived();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    CHECK(!received);
  }
}

} // namespace
