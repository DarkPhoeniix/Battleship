
#ifndef __BATTLESHIPGAME_H_INCLUDED
#define __BATTLESHIPGAME_H_INCLUDED

#include <memory>

#include "abstract_game.h"
#include "player.h"

namespace Battleship {
    class BattleshipGame
        : public AbstractBattleshipGame
    {
    private:
        std::pair<Player, Player> _players;
        uint8_t _idOfTurn;

    public:
        BattleshipGame(const std::pair<Player, Player>& players, uint8_t _idOfTurn = 0);
        ~BattleshipGame();

        TileState getTileState(uint8_t xCoordinate,
                               uint8_t yCoordinate,
                               uint8_t playerId = 0) override;
        TurnStatus registerTurn(uint8_t xCoordinate,
                                uint8_t yCoordinate,
                                uint8_t playerId = 0) override;
        MappingStatus addTile(uint8_t xCoordinate,
                              uint8_t yCoordinate,
                              uint8_t playerId = 0) override;
        TerminationStatus finished(uint8_t playerId = 0) override;
    };
} // namespace Battleship

#endif // __BATTLESHIPGAME_H_INCLUDED
