#include "server_wrapper.h"

Battleship::ServerWrapper::ServerWrapper(
    std::unique_ptr<AbstractBattleshipGame> &&game, unsigned short port,
    std::function<void(uint8_t, uint8_t, uint8_t)> updateCallback)
    : _game(std::move(game)), _server(port), _client("127.0.0.1", port),
      _updateCallback(updateCallback) {}

void Battleship::ServerWrapper::initServer() {
  _server.bind("subscribe", [this](const std::string &ip, unsigned short port) {
    _subscribers.emplace_back(ip, port);
  });
  _server.bind("getTileState", [this](uint8_t x, uint8_t y, uint8_t playerId) {
    return static_cast<int>(_game->getTileState(x, y, playerId));
  });
  _server.bind("registerTurn", [this](uint8_t x, uint8_t y, uint8_t playerId) {
    auto res = _game->registerTurn(x, y, playerId);
    _updateCallback(x, y, playerId);
    for (const auto &sub : _subscribers) {
      rpc::client c(sub.first, sub.second);
      c.call("update", x, y, playerId);
    }
    return static_cast<int>(res);
  });
  _server.bind("addTile", [this](uint8_t x, uint8_t y, uint8_t playerId) {
    auto res = _game->addTile(x, y, playerId);
    _updateCallback(x, y, playerId);
    for (const auto &sub : _subscribers) {
      rpc::client c(sub.first, sub.second);
      c.call("update", x, y, playerId);
    }
    return static_cast<int>(res);
  });
  _server.bind("finished", [this](uint8_t playerId) {
    return static_cast<int>(_game->finished(playerId));
  });
  _server.async_run(1);
}

std::unique_ptr<Battleship::AbstractBattleshipGame>
Battleship::ServerWrapper::wrap(
    std::unique_ptr<Battleship::AbstractBattleshipGame> &&game,
    unsigned short port,
    std::function<void(uint8_t, uint8_t, uint8_t)> updateCallback) {
  auto wrapper = std::unique_ptr<ServerWrapper>(
      new ServerWrapper(std::move(game), port, updateCallback));
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
