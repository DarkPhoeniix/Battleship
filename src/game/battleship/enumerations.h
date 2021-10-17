
#ifndef __ENUMERATIONS_H_INCLUDED
#define __ENUMERATIONS_H_INCLUDED

namespace Battleship {
enum class TileState { Empty = 0, WasShot = 1, ShipAfloat = 2, ShipSunk = 4 };

enum class TurnStatus { Ok = 0, TileError = 1, OrderError = 2 };

enum class MappingStatus { Ok = 0, PlacementError = 1, WrongPhaseError = 2 };

enum class TerminationStatus {
  NotTerminated = 0,
  PlayerWon = 1,
  PlayerLost = 2
};

inline TileState operator|(TileState lhs, TileState rhs) {
  return static_cast<TileState>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline TileState &operator|=(TileState &lhs, const TileState &rhs) {
  return lhs = lhs | rhs;
}

inline TileState operator&(TileState lhs, TileState rhs) {
  return static_cast<TileState>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

inline TileState &operator&=(TileState &lhs, const TileState &rhs) {
  return lhs = lhs & rhs;
}

inline bool operator==(TileState lhs, TileState rhs) {
  return static_cast<int>(lhs) == static_cast<int>(rhs);
}
} // namespace Battleship

#endif // __ENUMERATIONS_H_INCLUDED
