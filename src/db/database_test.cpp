#include "db/database.h"
#include <QTest>
class TestDatabase : public QObject {
  Q_OBJECT
private slots:
  void initTestCase() {
    auto dir = QDir(QDir::home().absoluteFilePath(".battleship"));
    if (dir.exists() && dir.absolutePath() != QDir::home().absolutePath())
      dir.removeRecursively();
  }
  void writeEntries() {
    QVERIFY(Battleship::Database::Instance().addVictory("P0", 20, 5));
    QVERIFY(Battleship::Database::Instance().addVictory("P1", 20, 3));
    QVERIFY(Battleship::Database::Instance().addLoss("P1", 2, 6));
    QVERIFY(Battleship::Database::Instance().addVictory("P1", 20, 10));
    QVERIFY(Battleship::Database::Instance().addVictory("P2", 20, 5));
    QVERIFY(Battleship::Database::Instance().addLoss("P2", 3, 3));
  };
  void readEntries() {
    auto summary = Battleship::Database::Instance().summary();
    QCOMPARE(summary->rowCount(), 3);
    QCOMPARE(summary->record(0).field(0).value().toString(), QString("P1"));
    QCOMPARE(summary->record(0).field(1).value().toInt(), 2);
    QCOMPARE(summary->record(0).field(2).value().toInt(), 42);
    QCOMPARE(summary->record(0).field(3).value().toInt(), 19);
    delete summary;
  }
  void cleanupTestCase() {
    Battleship::Database::Instance().close();
    auto dir = QDir(QDir::home().absoluteFilePath(".battleship"));
    if (dir.exists() && dir.absolutePath() != QDir::home().absolutePath())
      dir.removeRecursively();
  }
};

QTEST_MAIN(TestDatabase)
#include "database_test.moc"
