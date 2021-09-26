
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
        int _score;

    public:
        Player(const std::string& name, const Map& playerMap = Map());
        Player(const Player& copy);
        Player(Player&& obj);
        ~Player();

        Player& operator=(const Player& copy);
        Player& operator=(Player&& obj);

        bool attack(const std::pair<uint8_t, uint8_t>& attackCoordinate);
        bool setCoordinateState(const std::pair<uint8_t, uint8_t>& coordinate, TileState tileState);
        void addScore(int score);
        std::string getName() const;
        const Map& getPlayerMap() const;
        int getScore() const;
    };
} // namespace Battleship

#endif // __PLAYER_H_INCLUDED
