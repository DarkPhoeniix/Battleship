
#ifndef __SHIP_H_INCLUDED
#define __SHIP_H_INCLUDED

#include <cstdint>
#include <vector>

namespace Battleship {
    class Ship
    {
    private:
        std::vector<std::pair<uint8_t, uint8_t>> _coordinates;
        bool _isDestroyed;

    public:
        Ship(const std::vector<std::pair<uint8_t, uint8_t>>& coordinates, bool isDestroyed = false);
        Ship(const Ship& copy);
        Ship(Ship&& obj) noexcept;
        ~Ship();

        Ship& operator=(const Ship& copy);
        Ship& operator=(Ship&& obj) noexcept;

        void setIsDestroyed(bool isDestroyed);
        std::vector<std::pair<uint8_t, uint8_t>> getCoordinates() const;
        bool isDestroyed() const;

        friend bool operator==(const Ship& lhs, const Ship& rhs);
    };
}

#endif // __SHIP_H_INCLUDED
