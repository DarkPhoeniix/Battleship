#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game/battleship/abstract_game.h"
#include "widgets/fieldwidget.h"
#include <QApplication>
#include <QLCDNumber>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <functional>
#include <memory>

namespace Ui {
class MainWindow;
}

struct PlayerUI {
  QLineEdit *name;
  FieldWidget *field;
  QLCDNumber *hit;
  QLCDNumber *miss;
};

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  static constexpr int RestartCode = 101;

private:
  Ui::MainWindow *ui;
  std::unique_ptr<Battleship::AbstractBattleshipGame> _game;
  std::function<void(int, int)>
  createClickHandler(std::shared_ptr<Battleship::AbstractBattleshipGame> game,
                     int playerId, PlayerUI ui, PlayerUI enemyUI,
                     bool skipMapping = false);
  void uiToGameMode();
signals:
  void showWinner(const QString &winner);
private slots:
  void showScoreboardSummary();
  void startSingleScreenGame();
  void hostGame();
  void startHostedGame(QString clientIp, QString playerName,
                       QString opponentName);
  void connectToGame();
  void startGameAsClient(QString hostIp);
  void startGameWithAI();
  void showWinnerSlot(const QString &winner);
};

#endif // MAINWINDOW_H
