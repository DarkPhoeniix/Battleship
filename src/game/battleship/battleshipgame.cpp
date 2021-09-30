
#include "battleshipgame.h"

namespace Battleship {
    BattleshipGame::BattleshipGame(const std::pair<Player, Player>& players,
                                   uint8_t idOfTurn)
        : _players(players), _idOfTurn(idOfTurn)
    {   }

    BattleshipGame::~BattleshipGame()
    {   }



    TileState BattleshipGame::getTileState(uint8_t xCoordinate,
                                           uint8_t yCoordinate,
                                           uint8_t playerId)
    {
        Player currentPlayer = (playerId == 0) ? _players.first :
                                                 _players.second;
        auto coordinate = std::make_pair(xCoordinate, yCoordinate);

        return currentPlayer.getPlayerMap().getTile(coordinate);
    }

    TurnStatus BattleshipGame::registerTurn(uint8_t xCoordinate,
                                            uint8_t yCoordinate,
                                            uint8_t playerId)
    {
        Player currentPlayer = (playerId == 0) ? _players.first :
                                                 _players.second;
        auto coordinates = std::make_pair(xCoordinate, yCoordinate);
        auto currentCoord = currentPlayer.getPlayerMap().getTile(coordinates);
        TurnStatus result = TurnStatus::Ok;

        do {
            if (_idOfTurn != playerId) {
                result = TurnStatus::OrderError;
                break;
            }
            if ((currentCoord & TileState::WasShot) != TileState::Empty) {
                result = TurnStatus::TileError;
                break;
            }
        } while(false);

        if (result == TurnStatus::Ok &&
            (currentCoord & TileState::ShipAfloat) != TileState::ShipAfloat)
        {
            _idOfTurn = (playerId + 1) % 2;
        }
        return result;
    }

    MappingStatus BattleshipGame::addTile(uint8_t xCoordinate,
                                          uint8_t yCoordinate,
                                          uint8_t playerId)
    {
        Player currentPlayer = (playerId == 0) ? _players.first :
                                                 _players.second;
        auto coordinates = std::make_pair(xCoordinate, yCoordinate);
        bool isPlacementCorrect = currentPlayer.setCoordinateState(coordinates, TileState::ShipAfloat);
        currentPlayer.setShips();
        MappingStatus result = MappingStatus::Ok;

        do {
            if (currentPlayer.getPlayerMap().isBattlePhase()) {
                result = MappingStatus::WrongPhaseError;
                break;
            }
            if (!isPlacementCorrect) {
                result = MappingStatus::PlacementError;
                break;
            }
        } while(false);

        return result;
    }

    TerminationStatus BattleshipGame::finished(uint8_t playerId) {
        Player currentPlayer = (playerId == 0) ? _players.first :
                                                 _players.second;

        for (const auto& item : currentPlayer.getPlayerMap().getShips()) {
            if (item.isDestroyed() == false) {
                return TerminationStatus::NotTerminated;
            }
        }
        return TerminationStatus::PlayerLost;
    }
} // namespace Battleship
