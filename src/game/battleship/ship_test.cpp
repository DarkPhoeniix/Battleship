#include "game/battleship/ship.h"
#include <QTest>

class ShipTest : public QObject {
  Q_OBJECT
private slots:
  void initTestCase() {}

  void testObjectConstruction() {
    std::vector<std::vector<Battleship::Coordinate>> coords = {
        {{0, 0}, {1, 0}, {2, 0}}, {{9, 9}}, {{5, 4}, {5, 5}}};
    Battleship::Ship ships[] = {Battleship::Ship(coords[0]),
                                Battleship::Ship(coords[1], true),
                                Battleship::Ship(coords[2], false)};
    QCOMPARE(ships[0].getCoordinates(), coords[0]);
    QCOMPARE(ships[1].getCoordinates(), coords[1]);
    QCOMPARE(ships[2].getCoordinates(), coords[2]);

    QVERIFY(!ships[0].isDestroyed());
    QVERIFY(ships[1].isDestroyed());
    QVERIFY(!ships[2].isDestroyed());
  }

  void testObjectCopying() {
    Battleship::Ship ship({{0, 0}, {1, 0}, {2, 0}, {0, 1}});
    Battleship::Ship copyShip_1(ship);
    Battleship::Ship copyShip_2({{0, 0}});
    copyShip_2 = ship;

    QCOMPARE(copyShip_1, ship);
    QCOMPARE(copyShip_2, ship);

    Battleship::Ship moveCopyShip_1(std::move(ship));
    Battleship::Ship moveCopyShip_2({{0, 0}});
    moveCopyShip_2 = std::move(copyShip_1);

    std::vector<Battleship::Coordinate> empty = {};
    QCOMPARE(ship.getCoordinates(), empty);
    QCOMPARE(copyShip_1.getCoordinates(), empty);
    QCOMPARE(moveCopyShip_1, copyShip_2);
    QCOMPARE(moveCopyShip_2, copyShip_2);
  }

  void cleanupTestCase() {}
};

QTEST_MAIN(ShipTest)
#include "ship_test.moc"
