#pragma once

namespace ares {
  namespace packet_sets {
    /*! Packet set for intraserver communications
     */

    ARES_PACKETSET_DECLARE(server, base);
    

#define ARES_PCK_ID_NAME(ID, NAME) ARES_PACKETSET_ID_NAME(server, ID, NAME)

    // Athena packets
    // Account <-> Character
    ARES_PCK_ID_NAME(0x2710, ATHENA_HA_LOGIN_REQ);
    ARES_PCK_ID_NAME(0x2711, ATHENA_AH_LOGIN_RESULT);
    ARES_PCK_ID_NAME(0x2719, ATHENA_HA_PING_REQ);
    ARES_PCK_ID_NAME(0x2718, ATHENA_AH_PING_ACK);
    ARES_PCK_ID_NAME(0x2712, ATHENA_HA_AID_AUTH_REQ);
    ARES_PCK_ID_NAME(0x2713, ATHENA_AH_AID_AUTH_RESULT);
    ARES_PCK_ID_NAME(0x2714, ATHENA_HA_USER_COUNT);
    ARES_PCK_ID_NAME(0x2716, ATHENA_HA_ACCOUNT_DATA_REQ);
    ARES_PCK_ID_NAME(0x2717, ATHENA_AH_ACCOUNT_DATA_RESULT);
    ARES_PCK_ID_NAME(0x272b, ATHENA_HA_SET_AID_ONLINE);
    ARES_PCK_ID_NAME(0x272c, ATHENA_HA_SET_AID_OFFLINE);
    ARES_PCK_ID_NAME(0x272d, ATHENA_HA_ONLINE_AIDS);
    ARES_PCK_ID_NAME(0x2734, ATHENA_AH_KICK_AID);
    ARES_PCK_ID_NAME(0x2737, ATHENA_HA_SET_ALL_AIDS_OFFLINE);

    // Character <-> Zone
    ARES_PCK_ID_NAME(0x2af8, ATHENA_ZH_LOGIN_REQ);
    ARES_PCK_ID_NAME(0x2af9, ATHENA_HZ_LOGIN_RESULT);
    ARES_PCK_ID_NAME(0x2afa, ATHENA_ZH_MAP_NAMES);
    ARES_PCK_ID_NAME(0x2afb, ATHENA_HZ_PRIVATE_MSG_NAME);
    ARES_PCK_ID_NAME(0x2b23, ATHENA_ZH_PING_REQ);
    ARES_PCK_ID_NAME(0x2b24, ATHENA_HZ_PING_ACK);
    ARES_PCK_ID_NAME(0x2aff, ATHENA_ZH_ONLINE_USERS);
    ARES_PCK_ID_NAME(0x2afe, ATHENA_ZH_USER_COUNT);
    ARES_PCK_ID_NAME(0x2b16, ATHENA_ZH_GAME_RATES);
    ARES_PCK_ID_NAME(0x2b26, ATHENA_ZH_CID_AUTH_REQ);
    ARES_PCK_ID_NAME(0x2b27, ATHENA_HZ_CID_AUTH_FAILED);

    // Ares
    ARES_PCK_ID_NAME(0x4001, ARES_ZH_MAP_NAMES_REQ);
    ARES_PCK_ID_NAME(0x4002, ARES_HZ_MAP_NAMES);

#undef ARES_PCK_ID_NAME
    
  }
}
