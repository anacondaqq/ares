template <typename PacketSet>
struct type<PacketSet, ATHENA_ZH_MAP_NAMES> {
  using packet_name = ATHENA_ZH_MAP_NAMES;
  
  void emplace(const size_t num_maps) {
    PacketType = PacketSet::template id_of<type<PacketSet, packet_name>>::value;
    PacketLength = sizeof(*this) + num_maps * sizeof(uint32_t);
  }

  explicit type(const size_t num_maps) {
    emplace(num_maps);
  }

  const uint32_t* map_ids() const {
    return map_ids_;
  }
  
  uint16_t PacketType;
  uint16_t PacketLength;
private:
  uint32_t map_ids_[];
};
