#include "state.hpp"
#include "../server.hpp"

void ares::character::zone_server::packet_handler<ares::packet::current<ares::packet::ARES_ZH_MAP_NAMES_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "ARES_ZH_MAP_NAMES_REQ: begin");
  auto& maps_to_send = session_.as_zone_server().maps_to_send;
  if (maps_to_send.size() > 0) {
    const size_t max_batch_sz = p_->buf_sz() > (session_.send_buf().unfragmented_free_size() / 2)
      ? session_.send_buf().unfragmented_free_size() / 2
      : p_->buf_sz();
    if (max_batch_sz > 20) {
      size_t batch_sz{0};
      for (auto it = maps_to_send.begin(); (batch_sz < max_batch_sz - 20) && (it != maps_to_send.end()); ++it) {
        batch_sz += it->size() + 1;
      }
      SPDLOG_TRACE(log(), "Creating map names batch of {} bytes", batch_sz);
      session_.emplace_and_send<packet::current<packet::ARES_HZ_MAP_NAMES>>(batch_sz);
      size_t sz{0};
      auto it = maps_to_send.begin();
      for (; (sz < batch_sz) && (it != maps_to_send.end()); ++it) {
        session_.as_zone_server().map_names.insert(*it);
        session_.copy_and_send(it->c_str(), it->size() + 1);
        sz += it->size() + 1;
      }
      maps_to_send.erase(maps_to_send.begin(), it);
      SPDLOG_TRACE(log(), "Sent maps batch of {} bytes to zone server", sz);
    } else {
      log()->error("Can't send maps to zone server: buffer size limits are too low, closing session");
      session_.close_gracefuly();
    }
  } else {
    SPDLOG_TRACE(log(), "No more maps to send to zone server");
    session_.emplace_and_send<packet::current<packet::ARES_HZ_MAP_NAMES>>(0);
  }
  SPDLOG_TRACE(log(), "ARES_ZH_MAP_NAMES_REQ: end");
}
