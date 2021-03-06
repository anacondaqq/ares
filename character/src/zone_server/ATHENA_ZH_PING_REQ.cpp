#include "state.hpp"
#include "../server.hpp"

void ares::character::zone_server::packet_handler<ares::packet::current<ares::packet::ATHENA_ZH_PING_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_ZH_PING_REQ: begin");
  session_.emplace_and_send<packet::current<packet::ATHENA_HZ_PING_ACK>>();
  SPDLOG_TRACE(log(), "ATHENA_ZH_PING_REQ: end");
}
