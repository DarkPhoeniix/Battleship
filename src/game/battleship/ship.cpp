#include "ship.h"

namespace Battleship {
Ship::Ship(const std::vector<Coordinate> &coordinates, bool isDestroyed)
    : _coordinates(coordinates), _isDestroyed(isDestroyed) {}

Ship::Ship(const Ship &copy)
    : _coordinates(copy._coordinates), _isDestroyed(copy._isDestroyed) {}

Ship::Ship(Ship &&obj) noexcept
    : _coordinates(std::move(obj._coordinates)),
      _isDestroyed(obj._isDestroyed) {}

Ship::~Ship() {}

Ship &Ship::operator=(const Ship &copy) {
  if (this != &copy) {
    this->_coordinates = copy._coordinates;
    this->_isDestroyed = copy._isDestroyed;
  }
  return *this;
}

Ship &Ship::operator=(Ship &&obj) noexcept {
  if (this != &obj) {
    this->_coordinates = std::move(obj._coordinates);
    this->_isDestroyed = obj._isDestroyed;
  }
  return *this;
}

void Ship::setIsDestroyed(bool isDestroyed) { _isDestroyed = isDestroyed; }

std::vector<Coordinate> Ship::getCoordinates() const { return _coordinates; }

bool Ship::isDestroyed() const { return _isDestroyed; }

bool operator==(const Ship &lhs, const Ship &rhs) {
  return lhs._coordinates == rhs._coordinates &&
         lhs._isDestroyed == rhs._isDestroyed;
}
} // namespace Battleship
