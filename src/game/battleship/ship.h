
#ifndef __SHIP_H_INCLUDED
#define __SHIP_H_INCLUDED

#include <cstdint>
#include <vector>

#include "coordinate.h"

namespace Battleship {
    class Ship
    {
    private:
        std::vector<Coordinate> _coordinates;
        bool _isDestroyed;

    public:
        Ship(const std::vector<Coordinate>& coordinates, bool isDestroyed = false);
        Ship(const Ship& copy);
        Ship(Ship&& obj) noexcept;
        ~Ship();

        Ship& operator=(const Ship& copy);
        Ship& operator=(Ship&& obj) noexcept;

        void setIsDestroyed(bool isDestroyed);
        std::vector<Coordinate> getCoordinates() const;
        bool isDestroyed() const;

        friend bool operator==(const Ship& lhs, const Ship& rhs);
    };
} // namespace Battleship

#endif // __SHIP_H_INCLUDED
