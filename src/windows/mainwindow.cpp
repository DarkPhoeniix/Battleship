#include "mainwindow.h"
#include "db/database.h"
#include "game/battleship/battleshipgame.h"
#include "ui_mainwindow.h"
#include "windows/scoreboardwindow.h"
#include <QDebug>
#include <QRandomGenerator>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->leftNameEdit->setText(
      QString("Player ") +
      QString::number(QRandomGenerator::system()->bounded(1000, 9999)));
  ui->rightNameEdit->setText(
      QString("Player ") +
      QString::number(QRandomGenerator::system()->bounded(1000, 9999)));
}

MainWindow::~MainWindow() { delete ui; }

std::function<void(int, int)> MainWindow::createClickHandler(
    std::shared_ptr<Battleship::AbstractBattleshipGame> game, int playerId,
    FieldWidget *w) {
  return [game, playerId, w, clicks = 0](int x, int y) mutable {
    ++clicks;
    if (clicks <= 20) {
      auto res = game->addTile(x, y, playerId);
      if (res != Battleship::MappingStatus::Ok)
        qDebug().verbosity(QDebug::MinimumVerbosity)
            << "Wrong mapping status: " << static_cast<int>(res);
      w->addFieldCross(x, y, Qt::black);
    } else {
      auto turn = game->registerTurn(x, y, std::abs(playerId - 1));
      if (turn == Battleship::TurnStatus::Ok) {
        auto tile = game->getTileState(x, y, playerId);
        if (static_cast<bool>(tile & Battleship::TileState::ShipAfloat)) {
          w->addFieldCross(x, y, Qt::red);
        } else if (static_cast<bool>(tile & Battleship::TileState::ShipSunk))
          w->addFieldCross(x, y, Qt::blue);
        else if (static_cast<bool>(tile & Battleship::TileState::WasShot))
          w->addFieldDot(x, y, Qt::black);
      } else
        qDebug().verbosity(QDebug::MaximumVerbosity)
            << "Wrong turn status: " << static_cast<int>(turn);
    }
    if (clicks == 20)
      for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
          w->removeFieldItems(i, j);
  };
}

void MainWindow::showScoreboardSummary() {
  auto window = new ScoreboardWindow(this);
  window->view()->setModel(Battleship::Database::Instance().summary());
  window->show();
}

void MainWindow::startSingleScreenGame() {
  ui->leftNameEdit->setReadOnly(true);
  ui->rightNameEdit->setReadOnly(true);
  auto game = std::shared_ptr<Battleship::AbstractBattleshipGame>(
      new Battleship::BattleshipGame(
          {{ui->leftNameEdit->text().toStdString()},
           {ui->rightNameEdit->text().toStdString()}}));
  connect(ui->leftField, &FieldWidget::click,
          createClickHandler(game, 0, ui->leftField));
  connect(ui->rightField, &FieldWidget::click,
          createClickHandler(game, 1, ui->rightField));
}

void MainWindow::hostGame() {}

void MainWindow::connectToGame() {}
