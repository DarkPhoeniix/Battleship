#ifndef COORDINATE_H
#define COORDINATE_H

#include <cstdint>

namespace Battleship {
    struct Coordinate {
        uint8_t x;
        uint8_t y;

        Coordinate() : x(0), y(0)
        {   }
        Coordinate(uint8_t x, uint8_t y)
            : x(x), y(y)
        {   }
        Coordinate(const Coordinate& copy)
            : x(copy.x), y(copy.y)
        {   }
        ~Coordinate()
        {   }

        Coordinate& operator=(const Coordinate& copy)
        {
            x = copy.x;
            y = copy.y;
            return *this;
        }
        friend bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }
        friend bool operator<(const Coordinate& lhs, const Coordinate& rhs) {
            if (lhs.x == rhs.x) return lhs.y < rhs.y;
            else return lhs.x < rhs.x;
        }
    };
} // namespace Battleship

#endif // COORDINATE_H
