
#include "game/battleship/player.h"
#include <QTest>

class PlayerTest : public QObject {
        Q_OBJECT
private slots:
    void initTestCase() { }

    void testObjectConstruction() {
        Battleship::Player pl("Player");

        QCOMPARE(pl.getName(), std::string("Player"));
        QCOMPARE(pl.getPlayerMap().getShips(), std::vector<Battleship::Ship>());
        QCOMPARE(pl.getScore(), 0);
    }

    void testObjectCopying() {
        Battleship::Player pl_1("Player");

        Battleship::Player pl_2(pl_1);
        Battleship::Player pl_3("None");
        pl_3 = pl_1;

        QCOMPARE(pl_2.getName(), std::string("Player"));
        QCOMPARE(pl_2.getPlayerMap().getShips(), std::vector<Battleship::Ship>());
        QCOMPARE(pl_2.getScore(), 0);

        QCOMPARE(pl_3.getName(), std::string("Player"));
        QCOMPARE(pl_3.getPlayerMap().getShips(), std::vector<Battleship::Ship>());
        QCOMPARE(pl_3.getScore(), 0);
    }

    void testObjectMoving() {
        Battleship::Player pl_1("Player");

        Battleship::Player pl_2(std::move(pl_1));

        QVERIFY(pl_1.getName() != std::string("Player"));
        QVERIFY(pl_1.getScore() == 0);

        QCOMPARE(pl_2.getName(), std::string("Player"));
        QCOMPARE(pl_2.getPlayerMap().getShips(), std::vector<Battleship::Ship>());
        QCOMPARE(pl_2.getScore(), 0);

        Battleship::Player pl_3("None");
        pl_3 = std::move(pl_2);

        QCOMPARE(pl_3.getName(), std::string("Player"));
        QCOMPARE(pl_3.getPlayerMap().getShips(), std::vector<Battleship::Ship>());
        QCOMPARE(pl_3.getScore(), 0);
    }

    void testCoordAnsShipSetting() {
        Battleship::Player pl("Player");

        QVERIFY(!pl.setShips());

        QVERIFY(pl.setCoordinateState({0,0}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({0,2}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({0,4}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({0,6}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({0,8}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({0,9}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({2,0}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({2,1}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({2,3}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({2,4}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({2,6}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({2,7}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({2,8}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({4,0}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({4,1}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({4,2}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({4,4}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({4,5}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({4,6}, Battleship::TileState::ShipAfloat));
        QVERIFY(pl.setCoordinateState({4,7}, Battleship::TileState::ShipAfloat));

        QVERIFY(pl.setCoordinateState({2,1}, Battleship::TileState::WasShot));

        QVERIFY(pl.setCoordinateState({2,6}, Battleship::TileState::WasShot));
        QVERIFY(pl.setCoordinateState({2,7}, Battleship::TileState::WasShot));

        QVERIFY(!pl.setCoordinateState({4,7}, Battleship::TileState::ShipAfloat));
        QVERIFY(!pl.setCoordinateState({2,7}, Battleship::TileState::WasShot));

        QVERIFY(pl.setCoordinateState({4,0}, Battleship::TileState::WasShot | Battleship::TileState::ShipSunk));
        QVERIFY(pl.setCoordinateState({4,1}, Battleship::TileState::WasShot | Battleship::TileState::ShipSunk));
        QVERIFY(pl.setCoordinateState({4,2}, Battleship::TileState::WasShot | Battleship::TileState::ShipSunk));

        QVERIFY(pl.setShips());
    }

    void testCoordGetting() {
        Battleship::Player pl("MyPlayer");

        pl.setCoordinateState({0,0}, Battleship::TileState::ShipAfloat);
        pl.setCoordinateState({2,2}, Battleship::TileState::ShipAfloat);
        pl.setCoordinateState({9,9}, Battleship::TileState::ShipAfloat);

        pl.setCoordinateState({2,2}, Battleship::TileState::WasShot);
        pl.setCoordinateState({9,9}, Battleship::TileState::WasShot);
        pl.setCoordinateState({9,9}, Battleship::TileState::ShipSunk);

        QCOMPARE(pl.getPlayerMap().getTile({0,0}), Battleship::TileState::Empty |
                                                   Battleship::TileState::ShipAfloat);
        QCOMPARE(pl.getPlayerMap().getTile({3,3}), Battleship::TileState::Empty);
        QCOMPARE(pl.getPlayerMap().getTile({2,2}), Battleship::TileState::Empty |
                                                   Battleship::TileState::ShipAfloat |
                                                   Battleship::TileState::WasShot);
        QCOMPARE(pl.getPlayerMap().getTile({9,9}), Battleship::TileState::Empty |
                                                   Battleship::TileState::ShipAfloat |
                                                   Battleship::TileState::WasShot |
                                                   Battleship::TileState::ShipSunk);
    }

    void testNameSetterAndGetter() {
        Battleship::Player pl_1("Player1");
        Battleship::Player pl_2(pl_1);
        Battleship::Player pl_3("Unknown");
        pl_3 = pl_1;

        QCOMPARE(pl_1.getName(), std::string("Player1"));
        QCOMPARE(pl_2.getName(), std::string("Player1"));
        QCOMPARE(pl_3.getName(), std::string("Player1"));

        Battleship::Player pl_4(std::move(pl_1));
        Battleship::Player pl_5("Unknown");
        pl_5 = std::move(pl_2);

        QCOMPARE(pl_4.getName(), std::string("Player1"));
        QCOMPARE(pl_5.getName(), std::string("Player1"));
    }

    void testScoreCounter() {
        Battleship::Player pl("Player");

        pl.addScore(10);
        QCOMPARE(pl.getScore(), 10);
        pl.addScore(40);
        QCOMPARE(pl.getScore(), 50);
    }

    void cleanupTestCase() { }
};

QTEST_MAIN(PlayerTest)
#include "player_test.moc"
