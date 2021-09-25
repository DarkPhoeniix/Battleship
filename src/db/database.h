#pragma once

#include <QDir>
#include <QtSql>

namespace Battleship {
class Database {
public:
  static Database &Instance() {
    static Database instance;
    return instance;
  }
  bool addVictory(QString player, int hit, int miss);
  bool addLoss(QString player, int hit, int miss);
  QSqlQueryModel *summary();
  void close();

private:
  bool _enabled = true;
  QDir _workingDir = QDir::home();
  QSqlDatabase _db = QSqlDatabase::addDatabase("QSQLITE");
  Database();
  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;
  bool initDir();
  bool initDb();
  bool addEntry(QString player, int win, int hit, int miss);
};
} // namespace Battleship
