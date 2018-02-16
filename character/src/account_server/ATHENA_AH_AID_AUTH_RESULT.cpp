#include "state.hpp"
#include "../server.hpp"

void ares::character::account_server::packet_handler<ares::packet::current<ares::packet::ATHENA_AH_AID_AUTH_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "ATHENA_AH_AID_AUTH_RESULT: begin");
  session_ptr s;
  std::lock_guard<std::mutex> lock(server_.mutex());
  s = server_.session_by_auth_request_id(p_->request_id());
  if (s != nullptr) {
    if (p_->fail() == 0) {
      if (s->is_mono()) {
        log()->info("Aid {} was authenticated by account server", p_->aid());
        server_.remove_auth_aid_request(p_->request_id());
        auto new_state = s->as_mono();
        s->session_state_.emplace<client::state>(std::move(new_state));
        auto& c = s->as_client();
        c.aid = p_->aid();
        c.auth_code1 = p_->auth_code1();
        c.auth_code2 = p_->auth_code2();
        server_.add(s);
        session_.emplace_and_send<packet::current<packet::ATHENA_HA_ACCOUNT_DATA_REQ>>(p_->aid());
      } else {
        log()->error("Received authentication result from account server for aid {}, but it's session is not in mono state, ignoring.", p_->aid());
      }
    } else {
      log()->warn("Aid {} is not authenticated by account server, closing session!", p_->aid());
      s->emplace_and_send<packet::current<packet::AC_REFUSE_LOGIN>>(8); // Rejected by server
      s->close_gracefuly();
    }
  } else {
    log()->error("ATHENA_AH_AID_AUTH_RESULT account server rejected login request. Check login/password configuration");
    s->close_gracefuly();
  }
}
