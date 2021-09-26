#include "ship.h"

namespace Battleship {
    Ship::Ship(const std::vector<std::pair<uint8_t, uint8_t>>& coordinates, bool isDestroyed)
        : _coordinates(coordinates), _isDestroyed(false)
    {   }

    Ship::Ship(const Ship& copy)
        : _coordinates(copy._coordinates), _isDestroyed(copy._isDestroyed)
    {   }

    Ship::Ship(Ship&& obj) noexcept
        : _coordinates(std::move(obj._coordinates)), _isDestroyed(obj._isDestroyed)
    {   }

    Ship::~Ship()
    {   }



    Ship& Ship::operator=(const Ship& copy) {
        if (this != &copy) {
            this->_coordinates = copy._coordinates;
            this->_isDestroyed = copy._isDestroyed;
        }
        return *this;
    }

    Ship& Ship::operator=(Ship&& obj) noexcept {
        if (this != &obj) {
            this->_coordinates = std::move(obj._coordinates);
            this->_isDestroyed = obj._isDestroyed;
        }
        return *this;
    }



    std::vector<std::pair<uint8_t, uint8_t>> Ship::getCoordinates() const {
        return _coordinates;
    }

    bool Ship::isDestroyed() const {
        return _isDestroyed;
    }
}
