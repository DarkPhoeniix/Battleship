
#ifndef __PLAYER_H_INCLUDED
#define __PLAYER_H_INCLUDED

#include <iostream>
#include <utility>
#include <string>

#include "enumerations.h"
#include "map.h"

namespace Battleship {
    class Player
    {
    private:
        std::string _name;
        Map _playerMap;
        unsigned _score;

    public:
        explicit Player(const std::string& name, const Map& playerMap = Map());
        Player(const Player& copy);
        Player(Player&& obj) noexcept;
        ~Player();

        Player& operator=(const Player& copy);
        Player& operator=(Player&& obj) noexcept;

        bool setCoordinateState(const Coordinate& Coordinate, TileState tileState);
        bool setShips();
        void addScore(unsigned score);
        std::string getName() const;
        const Map& getPlayerMap() const;
        unsigned getScore() const;
    };
} // namespace Battleship

#endif // __PLAYER_H_INCLUDED
