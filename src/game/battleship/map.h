
#ifndef __MAP_H_INCLUDED
#define __MAP_H_INCLUDED

#include <algorithm>
#include <vector>
#include <map>

#include "enumerations.h"
#include "ship.h"

namespace Battleship {
    class Map
    {
    private:
        static constexpr uint8_t _FLEET_SIZE = 10;

        static constexpr uint8_t _BATTLESHIP_SIZE = 4;
        static constexpr uint8_t _CRUISER_SIZE = 3;
        static constexpr uint8_t _DESTROYER_SIZE = 2;
        static constexpr uint8_t _SUBMARINE_SIZE = 1;

        static constexpr uint8_t _BATTLESHIP_COUNT = 1;
        static constexpr uint8_t _CRUISER_COUNT = 2;
        static constexpr uint8_t _DESTROYER_COUNT = 3;
        static constexpr uint8_t _SUBMARINE_COUNT = 4;

        std::map<Coordinate, TileState> _map;
        std::vector<Ship> _ships;
        bool _isBattlePhase;
        uint8_t _setShipCoords;

        bool _validateFleet() const;
        bool _validateShipsParameters() const;
        bool _validateFleetParameters() const;
        bool _validateShipArea(const Ship& ship) const;
        std::vector<Coordinate> _findShipCoords(Coordinate begin) const;
        bool _isCoordinateInShip(const Ship& ship, const Coordinate& coordinate) const;
        bool _checkIsShipSunk(const Coordinate& coordinate);
        bool _isShipSunkAfterShot(const Ship& ship) const;
        void _setShipSunk(const Ship& ship);
        void _showAreaAroundSunkShip(const Ship& ship);
        std::vector<Ship>::iterator _findShip(const Coordinate& coordinate);
        bool _hasSuchShip(const Coordinate& coordinate) const;

    public:
        static constexpr uint8_t MAP_SIZE = 10;

        Map();
        Map(const Map& copy);
        Map(Map&& obj) noexcept;

        Map& operator=(const Map& copy);
        Map& operator=(Map&& obj) noexcept;

        TileState getTile(const Coordinate& coordinates) const;
        const std::vector<Ship>& getShips() const;
        bool isBattlePhase() const;
        bool setTile(const Coordinate& coordinates, TileState tileState);
        bool setShips();
    };
} // namespace Battleship

#endif // __MAP_H_INCLUDED
