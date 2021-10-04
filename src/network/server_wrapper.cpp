#include "server_wrapper.h"

Battleship::ServerWrapper::ServerWrapper(
    std::unique_ptr<AbstractBattleshipGame> &&game, unsigned short port)
    : _game(std::move(game)), _server(port), _client("127.0.0.1", port) {}

void Battleship::ServerWrapper::initServer() {
  _server.bind("getTileState", [this](uint8_t x, uint8_t y, uint8_t playerId) {
    return static_cast<int>(_game->getTileState(x, y, playerId));
  });
  _server.bind("registerTurn", [this](uint8_t x, uint8_t y, uint8_t playerId) {
    return static_cast<int>(_game->registerTurn(x, y, playerId));
  });
  _server.bind("addTile", [this](uint8_t x, uint8_t y, uint8_t playerId) {
    return static_cast<int>(_game->addTile(x, y, playerId));
  });
  _server.bind("finished", [this](uint8_t playerId) {
    return static_cast<int>(_game->finished(playerId));
  });
  _server.async_run(1);
}

std::unique_ptr<Battleship::AbstractBattleshipGame>
Battleship::ServerWrapper::wrap(
    std::unique_ptr<Battleship::AbstractBattleshipGame> &&game,
    unsigned short port) {
  auto wrapper = std::make_unique<ServerWrapper>(std::move(game), port);
  wrapper->initServer();
  return wrapper;
}

Battleship::TileState
Battleship::ServerWrapper::getTileState(uint8_t x, uint8_t y,
                                        uint8_t playerId) {
  return static_cast<TileState>(
      _client.call("getTileState", x, y, playerId).as<int>());
}

Battleship::TurnStatus
Battleship::ServerWrapper::registerTurn(uint8_t x, uint8_t y,
                                        uint8_t playerId) {
  return static_cast<TurnStatus>(
      _client.call("registerTurn", x, y, playerId).as<int>());
}

Battleship::MappingStatus
Battleship::ServerWrapper::addTile(uint8_t x, uint8_t y, uint8_t playerId) {
  return static_cast<MappingStatus>(
      _client.call("addTile", x, y, playerId).as<int>());
}

Battleship::TerminationStatus
Battleship::ServerWrapper::finished(uint8_t playerId) {
  return static_cast<TerminationStatus>(
      _client.call("finished", playerId).as<int>());
}
