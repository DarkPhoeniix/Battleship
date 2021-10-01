
#include "map.h"
#include <iostream>

namespace Battleship {
    std::vector<std::pair<uint8_t, uint8_t>> Map::_findShipCoords(std::pair<uint8_t, uint8_t> begin) const
    {
        std::vector<std::pair<uint8_t, uint8_t>> ship;
        ship.reserve(4);
        ship.push_back(begin);

        auto checker = [&map = _map, &ship](std::pair<uint8_t, uint8_t> cur) -> void {
            if (map.at(cur) != TileState::Empty &&
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
                checker({ cur.first, cur.second + 1 });
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

                if (!_validateShipArea(item)) {
                    result = false;
                    break;
                }
            }

            if (sizesOfShips != std::vector<int>{4, 3, 2, 1}) {
                result = false;
                break;
            }
        } while (false);

        return result;
    }

    bool Map::_validateShipArea(const Ship& ship) const {
        bool result = true;
        auto coords = ship.getCoordinates();
        auto currentCoord = coords[0];

        for (const auto& item : coords) {
            if (item.first < 9 && item.second < 9) {
                currentCoord = std::pair<uint8_t, uint8_t>(item.first + 1, item.second + 1);
                if ((_map.at(currentCoord) & TileState::ShipAfloat) != TileState::Empty &&
                    std::find(coords.begin(), coords.end(), currentCoord) == coords.end()) {
                    result = false;
                    break;
                }
            }
            if (item.first < 9 && item.second > 0) {
                currentCoord = std::pair<uint8_t, uint8_t>(item.first + 1, item.second - 1);
                if ((_map.at(currentCoord) & TileState::ShipAfloat) != TileState::Empty &&
                    std::find(coords.begin(), coords.end(), currentCoord) == coords.end()) {
                    result = false;
                    break;
                }
            }
            if (item.first > 0 && item.second < 9) {
                currentCoord = std::pair<uint8_t, uint8_t>(item.first - 1, item.second + 1);
                if ((_map.at(currentCoord) & TileState::ShipAfloat) != TileState::Empty &&
                    std::find(coords.begin(), coords.end(), currentCoord) == coords.end()) {
                    result = false;
                    break;
                }
            }
            if (item.first > 0 && item.second > 0) {
                currentCoord = std::pair<uint8_t, uint8_t>(item.first - 1, item.second - 1);
                if ((_map.at(currentCoord) & TileState::ShipAfloat) != TileState::Empty &&
                    std::find(coords.begin(), coords.end(), currentCoord) == coords.end()) {
                    result = false;
                    break;
                }
            }
        }
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
        : _isBattlePhase(false), _setShipCoords(0)
    {
        _ships.reserve(10);

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                _map[std::make_pair(i, j)] = TileState::Empty;
            }
        }
    }

    Map::Map(const Map& copy)
        : _map(copy._map),
        _ships(copy._ships),
        _isBattlePhase(copy._isBattlePhase),
        _setShipCoords(copy._setShipCoords)
    {   }

    Map::Map(Map&& obj) noexcept
        : _map(std::move(obj._map)),
        _ships(std::move(obj._ships)),
        _isBattlePhase(obj._isBattlePhase),
        _setShipCoords(obj._setShipCoords)
    {   }



    Map& Map::operator=(const Map& copy)
    {
        if (this != &copy) {
            _map = copy._map;
            _ships = copy._ships;
            _isBattlePhase = copy._setShipCoords;
            _isBattlePhase = copy._isBattlePhase;
        }
        return *this;
    }

    Map& Map::operator=(Map&& obj) noexcept
    {
        if (this != &obj) {
            _map = std::move(obj._map);
            _ships = std::move(obj._ships);
            _isBattlePhase = obj._setShipCoords;
            _isBattlePhase = obj._isBattlePhase;
        }
        return *this;
    }



    TileState Map::getTile(const std::pair<uint8_t, uint8_t>& coordinates) const
    {
        return _map.at(coordinates);
    }

    const std::vector<Ship>& Map::getShips() const
    {
        return _ships;
    }

    bool Map::isBattlePhase() const
    {
        return _isBattlePhase;
    }

    bool Map::setTile(const std::pair<uint8_t, uint8_t>& coordinates, TileState tileState)
    {
        if ((_map.at(coordinates) & tileState) == TileState::Empty) {
            _map[coordinates] = _map[coordinates] | tileState;

            if ((_map.at(coordinates) & TileState::ShipAfloat) == TileState::ShipAfloat &&
                tileState == TileState::WasShot) {
                bool isShipDestroyed = true;
                for (auto& item : _ships) {
                    for (const auto& i : item.getCoordinates()) {
                        if ((_map.at(i) & TileState::WasShot) != TileState::WasShot) {
                            isShipDestroyed = false;
                        }
                    }
                    if (isShipDestroyed) {
                        item.setIsDestroyed(true);
                        isShipDestroyed = true;

                        for (const auto& i : item.getCoordinates()) {
                            _map.at(i) |= TileState::ShipSunk;
                        }
                    }
                }

            }
            return true;
        }
        return false;
    }

    bool Map::setShips()
    {
        bool result = false;

        for (uint8_t i = 0; i < 10; i++) {
            for (uint8_t j = 0; j < 10; j++) {
                std::pair<uint8_t, uint8_t> currentCoord = { i, j };
                if (!_hasSuchShip(currentCoord) && _map[currentCoord] != TileState::Empty) {
                    _ships.emplace_back(_findShipCoords(currentCoord));
                }
            }
        }
        result = _validateShips();
        if (result) {
            _isBattlePhase = true;
        }
        return result;
    }
}
