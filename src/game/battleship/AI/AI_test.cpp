
#include "game/battleship/AI/AI.h"
#include "game/battleship/map.h"
#include <QTest>
#include <ctime>
#include <iostream>

using namespace Battleship;

class AITest : public QObject {
  Q_OBJECT
private slots:
  void initTestCase() { srand(time(NULL)); }

  void testGeneratingMap() {
    Battleship::Map m;
    Battleship::AI ai;

    m = std::move(ai.generateMap());
    QVERIFY(m.setShips());
  }

  void testGeneratingAttack() {
    Battleship::Map m;
    Battleship::AI ai;

    m = std::move(ai.generateMap());
    QVERIFY(m.setShips());

    auto endVerify = [&ships = m.getShips()]() {
      for (const auto &ship : ships) {
        if (ship.isDestroyed() == true) {
          return false;
        }
      }
      return true;
    };

    TileState lastHit = TileState::Empty;
    Coordinate attack;
    while (!endVerify()) {
      attack = ai.generateAttack(lastHit);
      QVERIFY(m.setTile(attack, TileState::WasShot));
      lastHit = m.getTile(attack);
    }
  }

  void cleanupTestCase() {}
};

QTEST_MAIN(AITest)
#include "AI_test.moc"
