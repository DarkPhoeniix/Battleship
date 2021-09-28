
#include "game/battleship/map.h"
#include <QTest>

class MapTest : public QObject {
        Q_OBJECT
private slots:
    void initTestCase() { }

    void testObjectConstruction() {
        Battleship::Map map;
        for (uint8_t i = 0; i < 10; i++) {
            for (uint8_t j = 0; j < 10; j++) {
            QCOMPARE(map.getTile(std::make_pair(i, j)), Battleship::TileState::Empty);
            }
        }
        QVERIFY(map.getShips().empty());

        Battleship::Map mapCopy_1(map);
        for (uint8_t i = 0; i < 10; i++) {
            for (uint8_t j = 0; j < 10; j++) {
            QCOMPARE(mapCopy_1.getTile(std::make_pair(i, j)), Battleship::TileState::Empty);
            }
        }
        QVERIFY(mapCopy_1.getShips().empty());

        Battleship::Map mapCopy_2;
        mapCopy_2 = map;
        for (uint8_t i = 0; i < 10; i++) {
            for (uint8_t j = 0; j < 10; j++) {
            QCOMPARE(mapCopy_2.getTile(std::make_pair(i, j)), Battleship::TileState::Empty);
            }
        }
        QVERIFY(mapCopy_2.getShips().empty());

        Battleship::Map moveCopyMap_1(std::move(mapCopy_1));
        for (uint8_t i = 0; i < 10; i++) {
            for (uint8_t j = 0; j < 10; j++) {
            QCOMPARE(moveCopyMap_1.getTile(std::make_pair(i, j)), Battleship::TileState::Empty);
            }
        }
        QVERIFY(moveCopyMap_1.getShips().empty());

        Battleship::Map moveCopyMap_2;
        moveCopyMap_2 = std::move(mapCopy_2);
        for (uint8_t i = 0; i < 10; i++) {
            for (uint8_t j = 0; j < 10; j++) {
            QCOMPARE(moveCopyMap_2.getTile(std::make_pair(i, j)), Battleship::TileState::Empty);
            }
        }
        QVERIFY(moveCopyMap_2.getShips().empty());
    }

    void testRigthShipPlacement() {
        Battleship::Map map;

        QVERIFY(map.setTile({0, 4}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({6, 4}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({7, 8}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({9, 0}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({5, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({6, 0}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({8, 2}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({9, 2}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({9, 8}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({9, 9}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 8}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({0, 9}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({1, 9}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({3, 4}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({3, 5}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({3, 6}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({1, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({1, 1}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 1}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setShips());
    }

    void testWrongShipNumber() {
        Battleship::Map map;

        QVERIFY(map.setTile({0, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({0, 2}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({0, 4}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({0, 6}, Battleship::TileState::ShipAfloat));

        QVERIFY(!map.setShips());
    }

    void testWrongShipSize() {
        Battleship::Map map;

        QVERIFY(map.setTile({0, 0}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 2}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 4}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 6}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 8}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({0, 9}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({2, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 1}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({9, 9}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({9, 8}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({2, 4}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 5}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 6}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({4, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 1}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 2}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({4, 4}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 5}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 6}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 7}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 8}, Battleship::TileState::ShipAfloat));

        QVERIFY(!map.setShips());
    }

    void testWrongShipPlacement() {
        Battleship::Map map;

        QVERIFY(map.setTile({0, 0}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 2}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 4}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 6}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 8}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({0, 9}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({2, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 1}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({9, 9}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({9, 8}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({2, 4}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 5}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 6}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({4, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 1}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 2}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({5, 3}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({5, 4}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({5, 5}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({5, 6}, Battleship::TileState::ShipAfloat));

        QVERIFY(!map.setShips());
    }

    void testBattlePhase() {
        Battleship::Map map;

        QVERIFY(!map.isBattlePhase());

        QVERIFY(map.setTile({0, 0}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 2}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 4}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 6}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({0, 8}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({0, 9}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({2, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 1}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({9, 9}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({9, 8}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({2, 4}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 5}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({2, 6}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({4, 0}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 1}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 2}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setTile({4, 4}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 5}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 6}, Battleship::TileState::ShipAfloat));
        QVERIFY(map.setTile({4, 7}, Battleship::TileState::ShipAfloat));

        QVERIFY(map.setShips());

        QVERIFY(map.isBattlePhase());
    }

    void testTileStates() {
        Battleship::Map map;

        map.setTile({0,0}, Battleship::TileState::Empty);
        QCOMPARE(map.getTile({0,0}), Battleship::TileState::Empty);
        map.setTile({0,0}, Battleship::TileState::ShipAfloat);
        QCOMPARE(map.getTile({0,0}), Battleship::TileState::Empty |
                                     Battleship::TileState::ShipAfloat);
        map.setTile({0,0}, Battleship::TileState::WasShot);
        QCOMPARE(map.getTile({0,0}), Battleship::TileState::Empty |
                                     Battleship::TileState::ShipAfloat |
                                     Battleship::TileState::WasShot);
        map.setTile({0,0}, Battleship::TileState::ShipSunk);
        QCOMPARE(map.getTile({0,0}), Battleship::TileState::Empty |
                                     Battleship::TileState::ShipAfloat |
                                     Battleship::TileState::WasShot |
                                     Battleship::TileState::ShipSunk);
    }

    void cleanupTestCase() { }
};

QTEST_MAIN(MapTest)
#include "map_test.moc"
