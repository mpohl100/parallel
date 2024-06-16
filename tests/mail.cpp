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
    struct TestOrchestrator : public par::Addressee {
      TestOrchestrator(std::function<void()> on_received) : _on_received{on_received} {}
      void receive() override { _on_received(); }
      std::function<void()> _on_received;
    };
    auto mailbox = par::MailBox{};
    {
      auto my_Orchestrator = std::make_shared<TestOrchestrator>(std::function<void()>{on_received});
      auto Orchestrator = par::Orchestrator{my_Orchestrator};
      Orchestrator.expect(mailbox);
      mailbox.arrived();
      CHECK(received);
      received = false;
    }
    // Orchestrator is destructed, so nothing should be received
    mailbox.arrived();
    CHECK(!received);
  }
}

} // namespace
