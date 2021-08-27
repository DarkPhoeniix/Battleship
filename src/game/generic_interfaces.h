#pragma once
#include <utility>
namespace Interfaces {
template <typename TileState> struct IMapScanner {
  virtual TileState getTileState(uint8_t x, uint8_t y,
                                 uint8_t playerId = 0) = 0;
};
template <typename TurnStatus> struct ITurnRegistrar {
  virtual TurnStatus registerTurn(uint8_t x, uint8_t y,
                                  uint8_t playerId = 0) = 0;
};
template <typename MappingStatus> struct IMapper {
  virtual MappingStatus addTile(uint8_t x, uint8_t y, uint8_t playerId = 0) = 0;
};
template <typename TerminationStatus> struct ITerminationChecker {
  virtual TerminationStatus finished(uint8_t playerId = 0) = 0;
};
} // namespace Interfaces
