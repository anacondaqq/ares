#pragma once

#include <ares/network>

#include "session.hpp"

namespace ares {
  namespace zone {
    struct server;
    
    struct auth_request_manager : network::auth_request_manager<zone::auth_request_manager, server, session>,
                                  std::enable_shared_from_this<zone::auth_request_manager> {
      using network::auth_request_manager<zone::auth_request_manager, server, session>::auth_request_manager;

      void auth_timeout(session_ptr s);
    };
  }
}
