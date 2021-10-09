#include "mainwindow.h"
#include "db/database.h"
#include "game/battleship/battleshipgame.h"
#include "ui_mainwindow.h"
#include "windows/scoreboardwindow.h"
#include <QDebug>
#include <QMessageBox>
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

void showWinner(const QString &winner) {
  auto box = new QMessageBox();
  box->setWindowTitle("Game Over");
  box->setText(winner + " won");
  QObject::connect(box, &QMessageBox::finished, box, &QMessageBox::deleteLater);
  box->show();
};

std::function<void(int, int)> MainWindow::createClickHandler(
    std::shared_ptr<Battleship::AbstractBattleshipGame> game, int playerId,
    PlayerUI ui) {
  return [game, playerId, enemyId = std::abs(playerId - 1), ui,
          clicks = 0](int x, int y) mutable {
    ++clicks;
    if (clicks <= 20) {
      auto res = game->addTile(x, y, playerId);
      if (res != Battleship::MappingStatus::Ok)
        qDebug().verbosity(QDebug::MinimumVerbosity)
            << "Wrong mapping status: " << static_cast<int>(res);
      ui.field->addFieldCross(x, y, Qt::black);
    } else {
      auto turn = game->registerTurn(x, y, enemyId);
      if (turn == Battleship::TurnStatus::Ok) {
        auto tile = game->getTileState(x, y, playerId);
        if (static_cast<bool>(tile & Battleship::TileState::ShipAfloat)) {
          ui.field->addFieldCross(x, y, Qt::red);
          ui.hit->display(ui.hit->value() + 1);
        } else if (static_cast<bool>(tile & Battleship::TileState::ShipSunk)) {
          ui.field->addFieldCross(x, y, Qt::blue);
          ui.hit->display(ui.hit->value() + 1);
        } else if (static_cast<bool>(tile & Battleship::TileState::WasShot)) {
          ui.field->addFieldDot(x, y, Qt::black);
          ui.miss->display(ui.miss->value() + 1);
        }
        if (game->finished(playerId) ==
            Battleship::TerminationStatus::PlayerLost) {
          showWinner(ui.enemyName->text());
        } else if (game->finished(enemyId) ==
                   Battleship::TerminationStatus::PlayerLost) {
          showWinner(ui.name->text());
        }
      } else
        qDebug().verbosity(QDebug::MaximumVerbosity)
            << "Wrong turn status: " << static_cast<int>(turn);
    }
    if (clicks == 20)
      for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
          ui.field->removeFieldItems(i, j);
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
  ui->leftScoreCount->display(0);
  ui->leftMissedCount->display(0);
  ui->rightScoredCount->display(0);
  ui->rightMissedCount->display(0);
  auto game = std::shared_ptr<Battleship::AbstractBattleshipGame>(
      new Battleship::BattleshipGame(
          {{ui->leftNameEdit->text().toStdString()},
           {ui->rightNameEdit->text().toStdString()}}));
  connect(
      ui->leftField, &FieldWidget::click,
      createClickHandler(game, 0,
                         {ui->leftNameEdit, ui->rightNameEdit, ui->leftField,
                          ui->rightScoredCount, ui->rightMissedCount}));
  connect(
      ui->rightField, &FieldWidget::click,
      createClickHandler(game, 1,
                         {ui->rightNameEdit, ui->leftNameEdit, ui->rightField,
                          ui->leftScoreCount, ui->leftMissedCount}));
}

void MainWindow::hostGame() {}

void MainWindow::connectToGame() {}
