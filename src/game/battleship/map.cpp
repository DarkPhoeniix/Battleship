
#include "map.h"

namespace Battleship {
    std::vector<std::pair<uint8_t, uint8_t>> Map::_findShipCoords(std::pair<uint8_t, uint8_t> begin) const
    {
        std::vector<std::pair<uint8_t, uint8_t>> ship;
        ship.reserve(4);
        ship.push_back(begin);

        auto checker = [*this, &ship](std::pair<uint8_t, uint8_t> cur) -> void {
            if (_map.at(cur) != TileState::Empty &&
            find(ship.begin(), ship.end(), cur) == ship.end())
            {
                ship.push_back(cur);
            }
        };

        for (int i = 0; i < ship.size(); i++) {
            std::pair<uint8_t, uint8_t> cur = ship[i];
            if (cur.first < 9)
            {
                checker({ cur.first + 1, cur.second });
            }
            if (cur.first > 0)
            {
                checker({ cur.first - 1, cur.second });
            }
            if (cur.second < 9)
            {
                checker({ cur.first, cur.second + 1});
            }
            if (cur.second > 0)
            {
                checker({ cur.first, cur.second - 1 });
            }
        }
        return ship;
    }

    bool Map::_validateShips() const
    {
        bool result = true;

        do {
            if (_ships.size() != 10) {
                result = false;
                break;
            }

            std::vector<int> sizesOfShips(4, 0);
            for (const auto& item : _ships) {
                if (item.getCoordinates().size() > 4) {
                    result = false;
                    break;
                }
                sizesOfShips[item.getCoordinates().size() - 1]++;
            }

            if (sizesOfShips != std::vector<int>{4,3,2,1}) {
                result = false;
                break;
            }
        } while(false);

        return result;
    }

    bool Map::_hasSuchShip(const std::pair<uint8_t, uint8_t>& coordinate) const {
        bool isInVect = false;
        for (const auto& item : _ships) {
            auto vect = item.getCoordinates();
            if (find(vect.begin(), vect.end(), coordinate) != vect.end()) {
                isInVect = true;
            }
        }
        return isInVect;
    }



    Map::Map()
    {
        _ships.reserve(10);

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                _map[std::make_pair(i, j)] = TileState::Empty;
            }
        }
    }

    Map::Map(const Map& copy)
        : _map(copy._map), _ships(copy._ships)
    {   }

    Map::Map(Map&& obj)
        : _map(std::move(obj._map)), _ships(std::move(obj._ships))
    {   }



    Map& Map::operator=(const Map& copy)
    {
        if (this != &copy) {
            _map = copy._map;
            _ships = copy._ships;
        }
        return *this;
    }

    Map& Map::operator=(Map&& obj)
    {
        if (this != &obj) {
            _map = std::move(obj._map);
            _ships = std::move(obj._ships);
        }
        return *this;
    }



    TileState Map::getTile(const std::pair<uint8_t, uint8_t>& coordinates) const
    {
        return _map.at(coordinates);
    }

    bool Map::setTile(const std::pair<uint8_t, uint8_t>& coordinates, TileState tileState)
    {
        if ((_map.at(coordinates) & tileState) == TileState::Empty) {
            _map[coordinates] = _map[coordinates] | tileState;
            return true;
        }
        return false;
    }

    const std::vector<Ship>& Map::getShips() const
    {
        return _ships;
    }

    bool Map::setShips()
    {
        for (uint8_t i = 0; i < 10; i++) {
            for (uint8_t j = 0; j < 10; j++) {
                std::pair<uint8_t, uint8_t> currentCoord = {i, j};
                if (!_hasSuchShip(currentCoord) && _map[currentCoord] != TileState::Empty) {
                    _ships.push_back(Ship(_findShipCoords(currentCoord)));
                }
            }
        }

        return _validateShips();
    }
}
