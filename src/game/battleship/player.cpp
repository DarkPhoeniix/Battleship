
#include "player.h"

namespace Battleship {
    Player::Player(const std::string& name, const Map& playerMap)
        : _name(name), _playerMap(playerMap)
    {   }

    Player::Player(const Player& copy)
        : _name(copy._name), _playerMap(copy._playerMap), _score(copy._score)
    {   }

    Player::~Player()
    {   }



    Player& Player::operator=(const Player &copy) {
        if (this != &copy) {

        }
    }



    bool Player::setCoordinateState(const std::pair<uint8_t, uint8_t>& coordinate, TileState tileState)
    {
        return _playerMap.setTile(coordinate, tileState);
    }

    void Player::addScore(int score)
    {
       _score += score;
    }

    std::string Player::getName() const
    {
        return _name;
    }

    const Map& Player::getPlayerMap() const
    {
        return _playerMap;
    }

    int Player::getScore() const
    {
        return _score;
    }
} // namespace Battleship
