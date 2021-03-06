#pragma once

#include <list>
#include <unordered_set>

#include <ares/network>
#include <ares/packets>

#include "../config.hpp"

namespace ares {
  namespace character {
    struct server;
    struct session;

    namespace mono {
      struct state;
    }
    
    namespace zone_server {
      struct state {
        state(server& server, session& sess);        
        state(const mono::state& mono_state);

        void on_connect();
        void on_connection_reset();
        void on_operation_aborted();
        void on_eof();
        void on_socket_error();
        void defuse_asio();

        packet::alloc_info allocate(const uint16_t packet_id);
        void dispatch_packet(void* buf, std::function<void(void*)> deallocator);
        
        std::shared_ptr<spdlog::logger> log() const;
        const config& conf() const;

        // Data
        std::string login;
        std::list<std::string> maps_to_send;
        std::unordered_set<std::string> map_names;
        
        asio::ip::address_v4 ip_v4;
        uint16_t port;

        std::map<uint32_t, uint32_t> online_aid_to_cid;
        std::map<uint32_t, uint32_t> online_cid_to_aid;
        uint32_t base_rate{0};
        uint32_t job_rate{0};
        uint32_t drop_rate{0};

      private:
        server& server_;
        session& session_;
      };
      
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      ARES_PACKET_HANDLER(ATHENA_ZH_ONLINE_USERS);
      ARES_PACKET_HANDLER(ATHENA_ZH_USER_COUNT);
      ARES_PACKET_HANDLER(ATHENA_ZH_GAME_RATES);
      ARES_PACKET_HANDLER(ATHENA_ZH_PING_REQ);
      ARES_PACKET_HANDLER(ARES_ZH_MAP_NAMES_REQ);
      ARES_PACKET_HANDLER(ARES_ZH_CID_AUTH_REQ);
    }
  }
}
