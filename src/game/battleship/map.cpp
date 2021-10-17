
#include "map.h"
#include <iostream>

namespace Battleship {
bool Map::_validateFleet() const {
  bool result = true;

  do {
    if (_ships.size() != _FLEET_SIZE) {
      result = false;
      break;
    }

    if (!_validateShipsParameters()) {
      result = false;
      break;
    }

    if (!_validateFleetParameters()) {
      result = false;
      break;
    }
  } while (false);

  return result;
}

bool Map::_validateShipsParameters() const {
  bool result = false;

  result = true;
  for (const auto &item : _ships) {
    if (item.getCoordinates().size() > _BATTLESHIP_SIZE) {
      result = false;
      break;
    }
  }

  return result;
}

bool Map::_validateFleetParameters() const {
  std::vector<uint8_t> sizesOfShips(4, 0);
  bool result = false;

  result = true;
  for (const auto &item : _ships) {
    sizesOfShips[item.getCoordinates().size() - 1]++;
    if (!_validateShipArea(item)) {
      result = false;
      break;
    }
  }

  if (sizesOfShips != std::vector<uint8_t>{_SUBMARINE_COUNT, _DESTROYER_COUNT,
                                           _CRUISER_COUNT, _BATTLESHIP_COUNT}) {
    result = false;
  }

  return result;
}

bool Map::_validateShipArea(const Ship &ship) const {
  bool result = true;
  auto coords = ship.getCoordinates();
  auto currentCoord = coords[0];

  for (const auto &item : coords) {
    if (item.x < 9 && item.y < 9) {
      currentCoord = Coordinate(item.x + 1, item.y + 1);
      if (!_isCoordinateInShip(coords, currentCoord)) {
        result = false;
        break;
      }
    }
    if (item.x < 9 && item.y > 0) {
      currentCoord = Coordinate(item.x + 1, item.y - 1);
      if (!_isCoordinateInShip(coords, currentCoord)) {
        result = false;
        break;
      }
    }
    if (item.x > 0 && item.y < 9) {
      currentCoord = Coordinate(item.x - 1, item.y + 1);
      if (!_isCoordinateInShip(coords, currentCoord)) {
        result = false;
        break;
      }
    }
    if (item.x > 0 && item.y > 0) {
      currentCoord = Coordinate(item.x - 1, item.y - 1);
      if (!_isCoordinateInShip(coords, currentCoord)) {
        result = false;
        break;
      }
    }
  }
  return result;
}

std::vector<Coordinate> Map::_findShipCoords(Coordinate begin) const {
  std::vector<Coordinate> ship;
  ship.reserve(_BATTLESHIP_SIZE);
  ship.push_back(begin);
  Coordinate currentCoord;

  for (int i = 0; i < ship.size(); i++) {
    if (ship[i].x < 9) {
      currentCoord = Coordinate(ship[i].x + 1, ship[i].y);
      if (!_isCoordinateInShip(ship, currentCoord)) {
        ship.push_back(currentCoord);
      }
    }
    if (ship[i].x > 0) {
      currentCoord = Coordinate(ship[i].x - 1, ship[i].y);
      if (!_isCoordinateInShip(ship, currentCoord)) {
        ship.push_back(currentCoord);
      }
    }
    if (ship[i].y < 9) {
      currentCoord = Coordinate(ship[i].x, ship[i].y + 1);
      if (!_isCoordinateInShip(ship, currentCoord)) {
        ship.push_back(currentCoord);
      }
    }
    if (ship[i].y > 0) {
      currentCoord = Coordinate(ship[i].x, ship[i].y - 1);
      if (!_isCoordinateInShip(ship, currentCoord)) {
        ship.push_back(currentCoord);
      }
    }
  }
  return ship;
}

bool Map::_isCoordinateInShip(const Ship &ship,
                              const Coordinate &coordinate) const {
  bool result = false;
  auto shipCoords = ship.getCoordinates();

  result = true;
  if ((_map.at(coordinate) & TileState::ShipAfloat) == TileState::ShipAfloat &&
      find(shipCoords.begin(), shipCoords.end(), coordinate) ==
          shipCoords.end()) {
    result = false;
  }

  return result;
}

bool Map::_checkIsShipSunk(const Coordinate &coordinate) {
  if ((_map.at(coordinate) & (TileState::ShipAfloat | TileState::WasShot)) ==
      (TileState::ShipAfloat | TileState::WasShot)) {
    auto currentShip = _findShip(coordinate);
    if (_isShipSunkAfterShot(*currentShip)) {
      _setShipSunk(*currentShip);
      currentShip->setIsDestroyed(true);
    }
    return true;
  }
  return false;
}

bool Map::_isShipSunkAfterShot(const Ship &ship) const {
  bool result = false;

  result = true;
  for (const auto &coord : ship.getCoordinates()) {
    if ((_map.at(coord) & TileState::WasShot) != TileState::WasShot) {
      result = false;
      break;
    }
  }

  return result;
}

void Map::_setShipSunk(const Ship &ship) {
  for (const auto &coord : ship.getCoordinates()) {
    _map[coord] |= TileState::ShipSunk;
    _showAreaAroundSunkShip(ship);
  }
}

void Map::_showAreaAroundSunkShip(const Ship &ship) {
  Coordinate currentCoord;
  for (const auto &coord : ship.getCoordinates()) {
    if (coord.x > 0) {
      currentCoord = Coordinate(coord.x - 1, coord.y);
      _map[currentCoord] |= TileState::WasShot;
    }
    if (coord.x < 9) {
      currentCoord = Coordinate(coord.x + 1, coord.y);
      _map[currentCoord] |= TileState::WasShot;
    }
    if (coord.y > 0) {
      currentCoord = Coordinate(coord.x, coord.y - 1);
      _map[currentCoord] |= TileState::WasShot;
    }
    if (coord.y < 9) {
      currentCoord = Coordinate(coord.x, coord.y + 1);
      _map[currentCoord] |= TileState::WasShot;
    }
    if (coord.x > 0 && coord.y > 0) {
      currentCoord = Coordinate(coord.x - 1, coord.y - 1);
      _map[currentCoord] |= TileState::WasShot;
    }
    if (coord.x > 0 && coord.y < 9) {
      currentCoord = Coordinate(coord.x - 1, coord.y + 1);
      _map[currentCoord] |= TileState::WasShot;
    }
    if (coord.x < 9 && coord.y > 0) {
      currentCoord = Coordinate(coord.x + 1, coord.y - 1);
      _map[currentCoord] |= TileState::WasShot;
    }
    if (coord.x < 9 && coord.y < 9) {
      currentCoord = Coordinate(coord.x + 1, coord.y + 1);
      _map[currentCoord] |= TileState::WasShot;
    }
  }
}

std::vector<Ship>::iterator Map::_findShip(const Coordinate &coordinate) {
  auto coordSearcher = [&coordinate](const Ship &ship) -> bool {
    auto coords = ship.getCoordinates();
    return (std::find(coords.begin(), coords.end(), coordinate) !=
            coords.end());
  };

  return std::find_if(_ships.begin(), _ships.end(), coordSearcher);
}

bool Map::_hasSuchShip(const Coordinate &coordinate) const {
  bool isInVect = false;

  for (const auto &item : _ships) {
    auto vect = item.getCoordinates();
    if (find(vect.begin(), vect.end(), coordinate) != vect.end()) {
      isInVect = true;
    }
  }

  return isInVect;
}

Map::Map() : _isBattlePhase(false), _setShipCoords(0) {
  _ships.reserve(_BATTLESHIP_COUNT + _CRUISER_COUNT + _DESTROYER_COUNT +
                 _SUBMARINE_COUNT);

  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++) {
      _map[Coordinate(i, j)] = TileState::Empty;
    }
  }
}

Map::Map(const Map &copy)
    : _map(copy._map), _ships(copy._ships), _isBattlePhase(copy._isBattlePhase),
      _setShipCoords(copy._setShipCoords) {}

Map::Map(Map &&obj) noexcept
    : _map(std::move(obj._map)), _ships(std::move(obj._ships)),
      _isBattlePhase(obj._isBattlePhase), _setShipCoords(obj._setShipCoords) {}

Map &Map::operator=(const Map &copy) {
  if (this != &copy) {
    _map = copy._map;
    _ships = copy._ships;
    _setShipCoords = copy._setShipCoords;
    _isBattlePhase = copy._isBattlePhase;
  }
  return *this;
}

Map &Map::operator=(Map &&obj) noexcept {
  if (this != &obj) {
    _map = std::move(obj._map);
    _ships = std::move(obj._ships);
    _setShipCoords = obj._setShipCoords;
    _isBattlePhase = obj._isBattlePhase;
  }
  return *this;
}

TileState Map::getTile(const Coordinate &coordinates) const {
  return _map.at(coordinates);
}

const std::vector<Ship> &Map::getShips() const { return _ships; }

bool Map::isBattlePhase() const { return _isBattlePhase; }

bool Map::setTile(const Coordinate &coordinate, TileState tileState) {
  bool result = false;
  if ((_map.at(coordinate) & tileState) == TileState::Empty) {
    _map[coordinate] = _map[coordinate] | tileState;
    _checkIsShipSunk(coordinate);

    result = true;
  }
  return result;
}

bool Map::setShips() {
  bool result = false;

  for (uint8_t i = 0; i < MAP_SIZE; i++) {
    for (uint8_t j = 0; j < MAP_SIZE; j++) {
      Coordinate currentCoord(i, j);
      if (!_hasSuchShip(currentCoord) &&
          _map[currentCoord] != TileState::Empty) {
        _ships.emplace_back(_findShipCoords(currentCoord));
      }
    }
  }
  result = _validateFleet();
  if (result) {
    _isBattlePhase = true;
  }
  return result;
}
} // namespace Battleship
