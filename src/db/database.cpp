#include "database.h"

bool Battleship::Database::addVictory(QString player, int hit, int miss) {
  return addEntry(player, 1, hit, miss);
}

bool Battleship::Database::addLoss(QString player, int hit, int miss) {
  return addEntry(player, 0, hit, miss);
}

QSqlQueryModel *Battleship::Database::summary() {
  QSqlQueryModel *model = new QSqlQueryModel;
  model->setQuery("SELECT player, sum(win), sum(hit), sum(miss) FROM entries "
                  "GROUP BY player ORDER BY sum(win) DESC");
  model->setHeaderData(0, Qt::Horizontal, "Name");
  model->setHeaderData(1, Qt::Horizontal, "Victories");
  model->setHeaderData(2, Qt::Horizontal, "Hits");
  model->setHeaderData(3, Qt::Horizontal, "Misses");
  return model;
}

void Battleship::Database::close() {
  if (_enabled)
    _db.close();
  _enabled = false;
}

Battleship::Database::Database() {
  if (!initDir())
    _enabled = false;
  else if (!initDb())
    _enabled = false;
}

bool Battleship::Database::initDir() {
  if (!_workingDir.cd(".battleship")) {
    _workingDir.mkdir(".battleship");
    if (!_workingDir.cd(".battleship"))
      return false;
  }
  return true;
}

bool Battleship::Database::initDb() {
  _db.setDatabaseName(_workingDir.filePath("records.sqlite"));
  if (!_db.open())
    return false;
  QSqlQuery query;
  query.prepare("CREATE TABLE IF NOT EXISTS entries ("
                "id       INTEGER NOT NULL UNIQUE,"
                "player   TEXT NOT NULL,"
                "win      INTEGER NOT NULL,"
                "hit      INTEGER NOT NULL,"
                "miss     INTEGER NOT NULL,"
                "PRIMARY KEY(id AUTOINCREMENT))");
  if (!query.exec())
    return false;
  return true;
}

bool Battleship::Database::addEntry(QString player, int win, int hit,
                                    int miss) {
  if (!_enabled)
    return false;
  QSqlQuery query;
  query.prepare("INSERT INTO entries (player, win , hit, miss)"
                "VALUES(:player, :win, :hit, :miss)");
  query.bindValue(":player", player);
  query.bindValue(":win", win);
  query.bindValue(":hit", hit);
  query.bindValue(":miss", miss);
  return query.exec();
}
