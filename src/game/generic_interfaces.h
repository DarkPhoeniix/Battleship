
#ifndef __GENERIC_INTERFACES_H_INCLUDED
#define __GENERIC_INTERFACES_H_INCLUDED

#include <cstdint>
#include <utility>

namespace Interfaces {
    template <typename TileState> struct IMapScanner {
        virtual TileState getTileState(uint8_t xCoordinate,
                                       uint8_t yCoordinate,
                                       uint8_t playerId = 0) = 0;
    };

    template <typename TurnStatus> struct ITurnRegistrator {
        virtual TurnStatus registerTurn(uint8_t xCoordinate,
                                        uint8_t yCoordinate,
                                        uint8_t playerId = 0) = 0;
    };

    template <typename MappingStatus> struct IMapper {
        virtual MappingStatus addTile(uint8_t xCoordinate,
                                      uint8_t yCoordinate,
                                      uint8_t playerId = 0) = 0;
    };

    template <typename TerminationStatus> struct ITerminationChecker {
        virtual TerminationStatus finished(uint8_t playerId = 0) = 0;
    };
} // namespace Interfaces

#endif // __GENERIC_INTERFACES_H_INCLUDED
