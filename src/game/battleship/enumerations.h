#pragma once
namespace Battleship {
enum class TileState { Empty = 0, WasShot = 1, ShipAfloat = 2, ShipSunk = 4 };
enum class TurnStatus { Ok = 0, TileError = 1, OrderError = 2 };
enum class MappingStatus { Ok = 0, PlacementError = 1, WrongPhaseError = 2 };
enum class TerminationStatus {
  NotTerminated = 0,
  PlayerWon = 1,
  PlayerLost = 2
};
} // namespace Battleship
