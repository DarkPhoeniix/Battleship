#include "mainwindow.h"
#include "db/database.h"
#include "game/battleship/AI/AI.h"
#include "game/battleship/battleshipgame.h"
#include "game/network.h"
#include "rpc/client.h"
#include "rpc/server.h"
#include "ui_mainwindow.h"
#include "windows/connecttogamedialog.h"
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
  connect(this, &MainWindow::showWinner, this, &MainWindow::showWinnerSlot);
}

MainWindow::~MainWindow() { delete ui; }

bool validateShipsInput(const QSet<QPair<int, int>> &ships) {
  Battleship::Map map;
  for (const auto &ship : ships)
    map.setTile(Battleship::Coordinate(ship.first, ship.second),
                Battleship::TileState::ShipAfloat);
  return map.setShips();
}

std::function<void(int, int)> MainWindow::createClickHandler(
    std::shared_ptr<Battleship::AbstractBattleshipGame> game, int playerId,
    PlayerUI ui, PlayerUI enemyUi, bool skipMapping) {
  return [this, game, playerId, enemyId = std::abs(playerId - 1), ui, enemyUi,
          ships = QSet<QPair<int, int>>(),
          isMappingPhase = !skipMapping](int x, int y) mutable {
    if (isMappingPhase) {
      if (ships.contains({x, y})) {
        ships.remove({x, y});
        emit ui.field->removeFieldItemsSignal(x, y);
      } else {
        ships.insert({x, y});
        emit ui.field->addFieldCrossSignal(x, y, Qt::black);
      }
      if (ships.size() == 20 && validateShipsInput(ships)) {
        for (const auto &ship : ships) {
          emit ui.field->removeFieldItemsSignal(ship.first, ship.second);
          auto res = game->addTile(ship.first, ship.second, playerId);
          if (res != Battleship::MappingStatus::Ok)
            qDebug().verbosity(QDebug::MinimumVerbosity)
                << "Wrong mapping status: " << static_cast<int>(res);
        }
        ships.clear();
        isMappingPhase = false;
      }
    } else {
      auto turn = game->registerTurn(x, y, playerId);
      if (turn == Battleship::TurnStatus::Ok) {
        qDebug().verbosity(QDebug::MaximumVerbosity)
            << "Player made a move: " << x << y;
        auto tile = game->getTileState(x, y, enemyId);
        if (static_cast<bool>(tile & Battleship::TileState::ShipSunk)) {
          emit ui.field->addFieldCrossSignal(x, y, Qt::blue);
          ui.hit->display(ui.hit->value() + 1);
        } else if (static_cast<bool>(tile &
                                     Battleship::TileState::ShipAfloat)) {
          emit ui.field->addFieldCrossSignal(x, y, Qt::red);
          ui.hit->display(ui.hit->value() + 1);
        } else if (static_cast<bool>(tile & Battleship::TileState::WasShot)) {
          emit ui.field->addFieldDotSignal(x, y, Qt::black);
          ui.miss->display(ui.miss->value() + 1);
        }
        if (game->finished(playerId) ==
            Battleship::TerminationStatus::PlayerLost) {
          Battleship::Database::Instance().addLoss(
              ui.name->text(), ui.hit->value(), ui.miss->value());
          Battleship::Database::Instance().addVictory(enemyUi.name->text(),
                                                      enemyUi.hit->value(),
                                                      enemyUi.miss->value());
          emit showWinner(enemyUi.name->text());
        } else if (game->finished(enemyId) ==
                   Battleship::TerminationStatus::PlayerLost) {
          Battleship::Database::Instance().addVictory(
              ui.name->text(), ui.hit->value(), ui.miss->value());
          Battleship::Database::Instance().addLoss(enemyUi.name->text(),
                                                   enemyUi.hit->value(),
                                                   enemyUi.miss->value());
          emit showWinner(ui.name->text());
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
  ui->actionLocal->setDisabled(true);
  ui->actionConnect->setDisabled(true);
  ui->actionHost->setDisabled(true);
  ui->actionAI->setDisabled(true);
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
  connect(ui->leftField, &FieldWidget::click,
          createClickHandler(game, 0,
                             {ui->leftNameEdit, ui->leftField,
                              ui->leftScoredCount, ui->leftMissedCount},
                             {ui->rightNameEdit, ui->rightField,
                              ui->rightScoredCount, ui->rightMissedCount}));
  connect(ui->rightField, &FieldWidget::click,
          createClickHandler(game, 1,
                             {ui->rightNameEdit, ui->rightField,
                              ui->rightScoredCount, ui->rightMissedCount},
                             {ui->leftNameEdit, ui->leftField,
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

void MainWindow::startHostedGame(QString clientIp, QString playerName,
                                 QString opponentName) {
  uiToGameMode();
  ui->leftNameEdit->setText(playerName);
  ui->rightNameEdit->setText(opponentName);
  auto server = new rpc::server(5050);
  connect(this, &MainWindow::destroyed, [server]() { delete server; });
  server->bind("click", [field = ui->rightField](int x, int y) {
    emit field->click(x, y);
  });
  server->async_run(1);
  connect(ui->rightField, &FieldWidget::removeFieldItemsSignal,
          [ip = clientIp.toStdString()](int x, int y) {
            rpc::client(ip, Battleship::ClientPort)
                .call("rightRemoveFieldItems", x, y);
          });
  connect(ui->leftField, &FieldWidget::removeFieldItemsSignal,
          [ip = clientIp.toStdString()](int x, int y) {
            rpc::client(ip, Battleship::ClientPort)
                .call("leftRemoveFieldItems", x, y);
          });
  connect(ui->rightField, &FieldWidget::addFieldCrossSignal,
          [ip = clientIp.toStdString()](int x, int y, Qt::GlobalColor color) {
            rpc::client(ip, Battleship::ClientPort)
                .call("rightAddFieldCross", x, y, static_cast<int>(color));
          });
  connect(ui->leftField, &FieldWidget::addFieldCrossSignal,
          [ip = clientIp.toStdString()](int x, int y, Qt::GlobalColor color) {
            rpc::client(ip, Battleship::ClientPort)
                .call("leftAddFieldCross", x, y, static_cast<int>(color));
          });
  connect(ui->rightField, &FieldWidget::addFieldDotSignal,
          [ip = clientIp.toStdString()](int x, int y, Qt::GlobalColor color) {
            rpc::client(ip, Battleship::ClientPort)
                .call("rightAddFieldDot", x, y, static_cast<int>(color));
          });
  connect(ui->leftField, &FieldWidget::addFieldDotSignal,
          [ip = clientIp.toStdString()](int x, int y, Qt::GlobalColor color) {
            rpc::client(ip, Battleship::ClientPort)
                .call("leftAddFieldDot", x, y, static_cast<int>(color));
          });
  connect(this, &MainWindow::showWinner,
          [ip = clientIp.toStdString()](QString name) {
            rpc::client(ip, Battleship::ClientPort)
                .call("showWinner", name.toStdString());
          });
  auto game = std::shared_ptr<Battleship::AbstractBattleshipGame>(
      new Battleship::BattleshipGame(
          {Battleship::Player{ui->leftNameEdit->text().toStdString()},
           Battleship::Player{ui->rightNameEdit->text().toStdString()}}));
  connect(ui->leftField, &FieldWidget::click,
          createClickHandler(game, 0,
                             {ui->leftNameEdit, ui->leftField,
                              ui->leftScoredCount, ui->leftMissedCount},
                             {ui->rightNameEdit, ui->rightField,
                              ui->rightScoredCount, ui->rightMissedCount}));
  connect(ui->rightField, &FieldWidget::click,
          createClickHandler(game, 1,
                             {ui->rightNameEdit, ui->rightField,
                              ui->rightScoredCount, ui->rightMissedCount},
                             {ui->leftNameEdit, ui->leftField,
                              ui->leftScoredCount, ui->leftMissedCount}));
  connect(ui->rightField, &FieldWidget::addFieldCrossSignal,
          [field = ui->rightField](int x, int y, Qt::GlobalColor color) {
            if (color == Qt::black)
              field->removeFieldItems(x, y);
          });
}

void MainWindow::connectToGame() {
  auto dialog = new ConnectToGameDialog(this);
  connect(dialog, &ConnectToGameDialog::finished, dialog,
          &ConnectToGameDialog::deleteLater);
  connect(dialog, &ConnectToGameDialog::reportInput, this,
          &MainWindow::startGameAsClient);
  dialog->show();
}

void MainWindow::startGameAsClient(QString hostIp) {
  uiToGameMode();
  ui->rightNameLabel->hide();
  ui->rightNameEdit->hide();
  ui->rightFooter->hide();
  ui->leftNameLabel->hide();
  ui->leftNameEdit->hide();
  ui->leftFooter->hide();
  connect(ui->rightField, &FieldWidget::click,
          [ip = hostIp.toStdString()](int x, int y) {
            rpc::client(ip, Battleship::ServerPort).call("click", x, y);
          });
  auto server = new rpc::server(5055);
  connect(this, &MainWindow::destroyed, [server]() { delete server; });
  server->bind("rightRemoveFieldItems", [field = ui->rightField](int x, int y) {
    emit field->removeFieldItemsSignal(x, y);
  });
  server->bind("leftRemoveFieldItems", [field = ui->leftField](int x, int y) {
    emit field->removeFieldItemsSignal(x, y);
  });
  server->bind("rightAddFieldCross", [field = ui->rightField](int x, int y,
                                                              int color) {
    emit field->addFieldCrossSignal(x, y, static_cast<Qt::GlobalColor>(color));
  });
  server->bind("leftAddFieldCross",
               [field = ui->leftField](int x, int y, int color) {
                 if (static_cast<Qt::GlobalColor>(color) != Qt::black)
                   emit field->addFieldCrossSignal(
                       x, y, static_cast<Qt::GlobalColor>(color));
               });
  server->bind("rightAddFieldDot", [field = ui->rightField](int x, int y,
                                                            int color) {
    emit field->addFieldDotSignal(x, y, static_cast<Qt::GlobalColor>(color));
  });
  server->bind("leftAddFieldDot", [field = ui->leftField](int x, int y,
                                                          int color) {
    emit field->addFieldDotSignal(x, y, static_cast<Qt::GlobalColor>(color));
  });
  server->bind("showWinner", [this](std::string name) {
    emit showWinner(QString::fromStdString(name));
  });
  server->async_run(1);
}

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
  connect(ui->leftField, &FieldWidget::click,
          createClickHandler(game, 0,
                             {ui->leftNameEdit, ui->leftField,
                              ui->leftScoredCount, ui->leftMissedCount},
                             {ui->rightNameEdit, ui->rightField,
                              ui->rightScoredCount, ui->rightMissedCount}));
  connect(ui->leftField, &FieldWidget::addFieldDotSignal,
          [this, game,
           ui = PlayerUI{ui->rightNameEdit, ui->rightField,
                         ui->rightScoredCount, ui->rightMissedCount},
           enemyUi = PlayerUI{ui->leftNameEdit, ui->leftField,
                              ui->leftScoredCount, ui->leftMissedCount},
           ai, prev = Battleship::TileState::Empty]() mutable {
            while (true) {
              auto attackCoordinate = ai.generateAttack(prev);
              if (attackCoordinate == Battleship::ErrCoord) {
                break;
              }
              auto [x, y] = attackCoordinate;
              auto turn = game->registerTurn(x, y, 1);
              qDebug().verbosity(QDebug::MaximumVerbosity)
                  << "AI made a move:" << x << y;
              if (turn == Battleship::TurnStatus::Ok) {
                auto tile = game->getTileState(x, y, 0);
                ai.approveAttack(Battleship::Coordinate(x, y), tile);
                prev = tile;
                if (static_cast<bool>(tile & Battleship::TileState::ShipSunk)) {
                  ui.field->addFieldCross(x, y, Qt::blue);
                  ui.hit->display(ui.hit->value() + 1);
                } else if (static_cast<bool>(
                               tile & Battleship::TileState::ShipAfloat)) {
                  ui.field->addFieldCross(x, y, Qt::red);
                  ui.hit->display(ui.hit->value() + 1);
                } else if (static_cast<bool>(tile &
                                             Battleship::TileState::WasShot)) {
                  ui.field->addFieldDot(x, y, Qt::black);
                  ui.miss->display(ui.miss->value() + 1);
                  break;
                }
                if (game->finished(1) ==
                    Battleship::TerminationStatus::PlayerLost) {
                  Battleship::Database::Instance().addLoss(
                      ui.name->text(), ui.hit->value(), ui.miss->value());
                  Battleship::Database::Instance().addVictory(
                      enemyUi.name->text(), enemyUi.hit->value(),
                      enemyUi.miss->value());
                  emit showWinner(enemyUi.name->text());
                  break;
                }
              } else {
                qDebug().verbosity(QDebug::MinimumVerbosity)
                    << "Wrong turn status in AI:" << static_cast<int>(turn);
                break;
              }
            }
          });
}

void MainWindow::showWinnerSlot(const QString &winner) {
  auto box = new QMessageBox();
  box->setText(winner + " won");
  QObject::connect(box, &QMessageBox::finished, box, &QMessageBox::deleteLater);
  QObject::connect(box, &QMessageBox::finished, box,
                   []() { qApp->exit(MainWindow::RestartCode); });
  box->show();
}
