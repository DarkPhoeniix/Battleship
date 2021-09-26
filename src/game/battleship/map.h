
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
        std::map<std::pair<uint8_t, uint8_t>, TileState> _map;
        std::vector<Ship> _ships;

        std::vector<std::pair<uint8_t, uint8_t>> _findShipCoords(std::pair<uint8_t, uint8_t> begin) const;
        bool _validateShips() const;
        bool _hasSuchShip(const std::pair<uint8_t, uint8_t>& coordinate) const;

    public:
        Map();
        Map(const Map& copy);
        Map(Map&& obj);

        Map& operator=(const Map& copy);
        Map& operator=(Map&& obj);

        TileState getTile(const std::pair<uint8_t, uint8_t>& coordinates) const;
        bool setTile(const std::pair<uint8_t, uint8_t>& coordinates, TileState tileState);
        const std::vector<Ship>& getShips() const;
        bool setShips();
    };
}

#endif // __MAP_H_INCLUDED
