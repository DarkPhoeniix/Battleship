
#include "player.h"

namespace Battleship {
    Player::Player(const std::string& name, const Map& playerMap)
        : _name(name), _playerMap(playerMap), _score(0)
    {   }

    Player::Player(const Player& copy)
        : _name(copy._name), _playerMap(copy._playerMap), _score(copy._score)
    {   }

    Player::Player(Player&& obj) noexcept
        : _name(std::move(obj._name)), _playerMap(std::move(obj._playerMap)), _score(obj._score)
    {   }

    Player::~Player()
    {   }



    Player& Player::operator=(const Player &copy) {
        if (this != &copy) {
            _name = copy._name;
            _playerMap = copy._playerMap;
            _score = copy._score;
        }
        return *this;
    }

    Player& Player::operator=(Player&& obj) noexcept {
        if (this != &obj) {
            _name = std::move(obj._name);
            _playerMap = std::move(obj._playerMap);
            _score = obj._score;
        }
        return *this;
    }



    bool Player::setCoordinateState(const std::pair<uint8_t, uint8_t>& coordinate, TileState tileState)
    {
        return _playerMap.setTile(coordinate, tileState);
    }

    bool Player::setShips()
    {
        int shipCounter = 0;
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; j++) {
                if ((_playerMap.getTile({i, j}) & TileState::ShipAfloat) != TileState::Empty) {
                    shipCounter++;
                }
            }
        }

        return (shipCounter == 20) ? _playerMap.setShips() : false;
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
