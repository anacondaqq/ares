#include "state.hpp"

#include "../server.hpp"
#include "../session.hpp"

ares::account::client::state::state(ares::account::server& serv,
                                    ares::account::session& sess) :
  server_(serv),
  session_(sess) {
  }

ares::account::client::state::state(const mono::state& mono_state) :
  server_(mono_state.server_),
  session_(mono_state.session_),
  client_exe_hash(mono_state.client_exe_hash) {
}

void ares::account::client::state::on_connect() {
  session_.close_abruptly();
}

void ares::account::client::state::on_connection_reset() {
  session_.close_abruptly();
}

void ares::account::client::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::account::client::state::on_eof() {
}

void ares::account::client::state::on_socket_error() {
}

void ares::account::client::state::defuse_asio() {
}

auto ares::account::client::state::allocate(const uint16_t packet_id) -> packet::alloc_info {
  switch (packet_id) {
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for client::state session while allocating", packet_id);
      packet::alloc_info ai;
      ai.expected_packet_sz = 0;
      ai.buf = nullptr;
      ai.buf_sz = 0;
      ai.deallocator = [] (void*) {};
      ai.PacketLength_offset = 0;
      return std::move(ai);
    }
  }
}

void ares::account::client::state::dispatch_packet(void* buf, std::function<void(void*)> /*deallocator*/) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf);
  switch (*packet_id) {
  default:
    {
      log()->error("Unexpected packet_id {:#x} for mono::state session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
}

auto ares::account::client::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::account::client::state::conf() const -> const config& {
  return server_.conf();
}
