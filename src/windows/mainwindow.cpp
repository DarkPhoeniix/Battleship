#include "mainwindow.h"
#include "db/database.h"
#include "game/battleship/AI/AI.h"
#include "game/battleship/battleshipgame.h"
#include "network/server_wrapper.h"
#include "ui_mainwindow.h"
#include "windows/hostgamedialog.h"
#include "windows/scoreboardwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSet>
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

bool validateShipsInput(const QSet<QPair<int, int>> &ships) {
  Battleship::Map map;
  for (const auto &ship : ships)
    map.setTile(Battleship::Coordinate(ship.first, ship.second),
                Battleship::TileState::ShipAfloat);
  return map.setShips();
}

std::function<void(int, int)> MainWindow::createClickHandler(
    std::shared_ptr<Battleship::AbstractBattleshipGame> game, int playerId,
    PlayerUI ui, bool skipMapping) {
  return [game, playerId, enemyId = std::abs(playerId - 1), ui,
          ships = QSet<QPair<int, int>>(),
          isMappingPhase = !skipMapping](int x, int y) mutable {
    if (isMappingPhase) {
      if (ships.contains({x, y})) {
        ships.remove({x, y});
        ui.field->removeFieldItems(x, y);
      } else {
        ships.insert({x, y});
        ui.field->addFieldCross(x, y, Qt::black);
      }
      if (ships.size() == 20 && validateShipsInput(ships)) {
        for (const auto &ship : ships) {
          ui.field->removeFieldItems(ship.first, ship.second);
          auto res = game->addTile(ship.first, ship.second, playerId);
          if (res != Battleship::MappingStatus::Ok)
            qDebug().verbosity(QDebug::MinimumVerbosity)
                << "Wrong mapping status: " << static_cast<int>(res);
        }
        ships.clear();
        isMappingPhase = false;
      }
    } else {
      auto turn = game->registerTurn(x, y, enemyId);
      if (turn == Battleship::TurnStatus::Ok) {
        auto tile = game->getTileState(x, y, playerId);
        if (static_cast<bool>(tile & Battleship::TileState::ShipSunk)) {
          ui.field->addFieldCross(x, y, Qt::blue);
          ui.hit->display(ui.hit->value() + 1);
        } else if (static_cast<bool>(tile &
                                     Battleship::TileState::ShipAfloat)) {
          ui.field->addFieldCross(x, y, Qt::red);
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
  };
}

void MainWindow::uiToGameMode() {
  ui->leftNameEdit->setReadOnly(true);
  ui->rightNameEdit->setReadOnly(true);
  ui->leftScoredCount->display(0);
  ui->leftMissedCount->display(0);
  ui->rightScoredCount->display(0);
  ui->rightMissedCount->display(0);
}

void MainWindow::showScoreboardSummary() {
  auto window = new ScoreboardWindow(this);
  window->view()->setModel(Battleship::Database::Instance().summary());
  window->show();
}

void MainWindow::startSingleScreenGame() {
  uiToGameMode();
  auto game = std::shared_ptr<Battleship::AbstractBattleshipGame>(
      new Battleship::BattleshipGame(
          {Battleship::Player{ui->leftNameEdit->text().toStdString()},
           Battleship::Player{ui->rightNameEdit->text().toStdString()}}));
  connect(
      ui->leftField, &FieldWidget::click,
      createClickHandler(game, 0,
                         {ui->leftNameEdit, ui->rightNameEdit, ui->leftField,
                          ui->rightScoredCount, ui->rightMissedCount}));
  connect(
      ui->rightField, &FieldWidget::click,
      createClickHandler(game, 1,
                         {ui->rightNameEdit, ui->leftNameEdit, ui->rightField,
                          ui->leftScoredCount, ui->leftMissedCount}));
}

void MainWindow::hostGame() {
  auto dialog = new HostGameDialog(this);
  connect(dialog, &HostGameDialog::finished, dialog,
          &HostGameDialog::deleteLater);
  connect(dialog, &HostGameDialog::reportInput, this,
          &MainWindow::startHostedGame);
  dialog->show();
}

void MainWindow::startHostedGame(unsigned short port, QString playerName,
                                 QString opponentName) {
  uiToGameMode();
  ui->leftNameEdit->setText(playerName);
  ui->rightNameEdit->setText(opponentName);
  auto game = Battleship::ServerWrapper::wrap(
      std::unique_ptr<Battleship::BattleshipGame>(
          new Battleship::BattleshipGame(
              {Battleship::Player{playerName.toStdString()},
               Battleship::Player{opponentName.toStdString()}})),
      port);
  connect(
      ui->leftField, &FieldWidget::click,
      createClickHandler(std::move(game), 0,
                         {ui->leftNameEdit, ui->rightNameEdit, ui->leftField,
                          ui->rightScoredCount, ui->rightMissedCount}));
}

void MainWindow::connectToGame() {}

void MainWindow::startGameWithAI() {
  uiToGameMode();
  ui->rightNameEdit->setText("HAL 9000");
  Battleship::AI ai;
  auto map = ai.generateMap();
  map.setShips();
  auto game = std::shared_ptr<Battleship::AbstractBattleshipGame>(
      new Battleship::BattleshipGame(
          {Battleship::Player{ui->leftNameEdit->text().toStdString()},
           Battleship::Player{ui->rightNameEdit->text().toStdString(), map}}));
  connect(
      ui->leftField, &FieldWidget::click,
      createClickHandler(game, 0,
                         {ui->leftNameEdit, ui->rightNameEdit, ui->leftField,
                          ui->rightScoredCount, ui->rightMissedCount}));
  connect(
      ui->rightField, &FieldWidget::click,
      createClickHandler(game, 1,
                         {ui->rightNameEdit, ui->leftNameEdit, ui->rightField,
                          ui->leftScoredCount, ui->leftMissedCount},
                         true));
  connect(ui->rightField, &FieldWidget::click,
          [game, field = ui->leftField, ai,
           prev = Battleship::TileState::Empty]() mutable {
            while (true) {
              auto [x, y] = ai.generateAttack(prev);
              auto turn = game->registerTurn(x, y, 1);
              if (turn == Battleship::TurnStatus::Ok) {
                qDebug().verbosity(QDebug::MaximumVerbosity)
                    << "AI made a move:" << x << y;
                auto tile = game->getTileState(x, y, 0);
                if (static_cast<bool>(tile & Battleship::TileState::ShipSunk)) {
                  field->addFieldCross(x, y, Qt::blue);
                } else if (static_cast<bool>(
                               tile & Battleship::TileState::ShipAfloat)) {
                  field->addFieldCross(x, y, Qt::red);
                } else if (static_cast<bool>(tile &
                                             Battleship::TileState::WasShot)) {
                  field->addFieldDot(x, y, Qt::black);
                  break;
                }
                // ISSUE: AI probably loops if given correct data about previous
                // tile
                // TODO: fix infinite loop in AI and uncomment the line below
                // prev = tile;
              } else {
                qDebug().verbosity(QDebug::MaximumVerbosity)
                    << "Wrong turn status in AI:" << static_cast<int>(turn);
                break;
              }
            }
          });
}
