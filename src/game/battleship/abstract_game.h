#pragma once
#include "../generic_interfaces.h"
#include "enumerations.h"
namespace Battleship {
struct AbstractBattleshipGame
    : public Interfaces::IMapScanner<TileState>,
      public Interfaces::ITurnRegistrar<TurnStatus>,
      public Interfaces::IMapper<MappingStatus>,
      public Interfaces::ITerminationChecker<TerminationStatus> {};
} // namespace Battleship
