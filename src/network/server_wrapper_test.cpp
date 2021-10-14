#include "game/battleship/battleshipgame.h"
#include "network/server_wrapper.h"
#include <QTest>

class ServerWrapperTest : public QObject {
  Q_OBJECT
private:
  std::vector<std::pair<uint8_t, uint8_t>> correctCoords = {
      {0, 0}, {0, 2}, {0, 4}, {0, 6}, {0, 8}, {0, 9}, {2, 0},
      {2, 1}, {2, 3}, {2, 4}, {2, 6}, {2, 7}, {2, 8}, {4, 0},
      {4, 1}, {4, 2}, {4, 3}, {4, 5}, {4, 6}, {4, 7}};

private slots:
private slots:
  void initTestCase() {}

  void testMapper() {
    auto game = Battleship::ServerWrapper::wrap(
        std::unique_ptr<Battleship::BattleshipGame>(
            new Battleship::BattleshipGame({Battleship::Player{"Player_1"},
                                            Battleship::Player{"Player_2"}})),
        8800);

    for (const auto &item : correctCoords) {
      QCOMPARE(game->addTile(item.first, item.second, 0),
               Battleship::MappingStatus::Ok);
      QCOMPARE(game->addTile(item.first, item.second, 1),
               Battleship::MappingStatus::Ok);
    }

    QCOMPARE(game->registerTurn(0, 0, 0), Battleship::TurnStatus::Ok);
    QCOMPARE(game->addTile(0, 0, 1),
             Battleship::MappingStatus::WrongPhaseError);
  }

  void testTurnRegistrator() {
    auto game = Battleship::ServerWrapper::wrap(
        std::unique_ptr<Battleship::BattleshipGame>(
            new Battleship::BattleshipGame({Battleship::Player{"Player_1"},
                                            Battleship::Player{"Player_2"}})),
        8801);

    for (const auto &item : correctCoords) {
      QCOMPARE(game->addTile(item.first, item.second, 0),
               Battleship::MappingStatus::Ok);
      QCOMPARE(game->addTile(item.first, item.second, 1),
               Battleship::MappingStatus::Ok);
    }

    QCOMPARE(game->registerTurn(0, 0, 0), Battleship::TurnStatus::Ok);
    QCOMPARE(game->registerTurn(0, 0, 0), Battleship::TurnStatus::TileError);
    QCOMPARE(game->registerTurn(0, 5, 0), Battleship::TurnStatus::Ok);
    QCOMPARE(game->registerTurn(0, 6, 0), Battleship::TurnStatus::OrderError);
    QCOMPARE(game->registerTurn(9, 9, 1), Battleship::TurnStatus::Ok);
    QCOMPARE(game->registerTurn(0, 6, 1), Battleship::TurnStatus::OrderError);
  }

  void testMapScanner() {
    auto game = Battleship::ServerWrapper::wrap(
        std::unique_ptr<Battleship::BattleshipGame>(
            new Battleship::BattleshipGame({Battleship::Player{"Player_1"},
                                            Battleship::Player{"Player_2"}})),
        8802);

    for (const auto &item : correctCoords) {
      QCOMPARE(game->addTile(item.first, item.second, 0),
               Battleship::MappingStatus::Ok);
      QCOMPARE(game->addTile(item.first, item.second, 1),
               Battleship::MappingStatus::Ok);
    }

    QCOMPARE(game->registerTurn(0, 0, 0), Battleship::TurnStatus::Ok);
    QCOMPARE(game->registerTurn(0, 5, 0), Battleship::TurnStatus::Ok);
    QCOMPARE(game->registerTurn(5, 5, 1), Battleship::TurnStatus::Ok);
    QCOMPARE(game->registerTurn(4, 7, 0), Battleship::TurnStatus::Ok);

    QCOMPARE(game->getTileState(5, 5, 0),
             Battleship::TileState::Empty | Battleship::TileState::WasShot);
    QCOMPARE(game->getTileState(0, 0, 1),
             Battleship::TileState::Empty | Battleship::TileState::WasShot |
                 Battleship::TileState::ShipAfloat |
                 Battleship::TileState::ShipSunk);
    QCOMPARE(game->getTileState(2, 5, 0), Battleship::TileState::Empty);
    QCOMPARE(game->getTileState(4, 7, 1),
             Battleship::TileState::Empty | Battleship::TileState::WasShot |
                 Battleship::TileState::ShipAfloat);
  }

  void testTerminationChecker() {
    auto game = Battleship::ServerWrapper::wrap(
        std::unique_ptr<Battleship::BattleshipGame>(
            new Battleship::BattleshipGame({Battleship::Player{"Player_1"},
                                            Battleship::Player{"Player_2"}})),
        8803);

    for (const auto &item : correctCoords) {
      QCOMPARE(game->addTile(item.first, item.second, 0),
               Battleship::MappingStatus::Ok);
      QCOMPARE(game->addTile(item.first, item.second, 1),
               Battleship::MappingStatus::Ok);
    }

    for (const auto &item : correctCoords) {
      QCOMPARE(game->registerTurn(item.first, item.second, 0),
               Battleship::TurnStatus::Ok);
    }

    QCOMPARE(game->finished(1), Battleship::TerminationStatus::PlayerLost);
  }

  void cleanupTestCase() {}
};

QTEST_MAIN(ServerWrapperTest)
#include "server_wrapper_test.moc"