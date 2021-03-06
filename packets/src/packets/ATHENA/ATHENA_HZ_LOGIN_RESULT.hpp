template <typename PacketSet>
struct type<PacketSet, ATHENA_HZ_LOGIN_RESULT> {
  using packet_set = PacketSet;
  using packet_name = ATHENA_HZ_LOGIN_RESULT;
  
  void emplace(const uint8_t ErrorCode) {
    PacketType = packet_sets::id_of<PacketSet, packet_name>::value;
    ErrorCode_ = ErrorCode;
  }
  
  explicit type(const uint8_t ErrorCode) {
    emplace(ErrorCode);
  }

  uint8_t ErrorCode() const {
    return ErrorCode_;
  }

  uint16_t PacketType;
private:
  uint8_t ErrorCode_;
};
