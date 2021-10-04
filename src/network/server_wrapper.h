#pragma once
#include "game/battleship/abstract_game.h"
#include "rpc/client.h"
#include "rpc/server.h"
#include <functional>
#include <memory>

namespace Battleship {
class ServerWrapper : public AbstractBattleshipGame {
  std::unique_ptr<AbstractBattleshipGame> _game;
  rpc::server _server;
  rpc::client _client;
  std::function<void(uint8_t, uint8_t, uint8_t)> _updateCallback;
  std::vector<std::pair<std::string, unsigned short>> _subscribers;
  ServerWrapper(
      std::unique_ptr<AbstractBattleshipGame> &&game, unsigned short port,
      std::function<void(uint8_t, uint8_t, uint8_t)> updateCallback =
          [](uint8_t, uint8_t, uint8_t) {});
  void initServer();

public:
  static std::unique_ptr<AbstractBattleshipGame> wrap(
      std::unique_ptr<AbstractBattleshipGame> &&game, unsigned short port,
      std::function<void(uint8_t, uint8_t, uint8_t)> updateCallback =
          [](uint8_t, uint8_t, uint8_t) {});
  TileState getTileState(uint8_t x, uint8_t y, uint8_t playerId = 0) override;
  ;
  TurnStatus registerTurn(uint8_t x, uint8_t y, uint8_t playerId = 0) override;
  ;
  MappingStatus addTile(uint8_t x, uint8_t y, uint8_t playerId = 0) override;
  ;
  TerminationStatus finished(uint8_t playerId = 0) override;
  ;
};
} // namespace Battleship
