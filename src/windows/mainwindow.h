#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game/battleship/abstract_game.h"
#include "widgets/fieldwidget.h"
#include <QLCDNumber>
#include <QLineEdit>
#include <QMainWindow>
#include <functional>
#include <memory>

namespace Ui {
class MainWindow;
}

struct PlayerUI {
  QLineEdit *name;
  QLineEdit *enemyName;
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
                     int playerId, PlayerUI w, bool skipMapping = false);
  void uiToGameMode();
private slots:
  void showScoreboardSummary();
  void startSingleScreenGame();
  void hostGame();
  void startHostedGame(unsigned short port, QString playerName,
                       QString opponentName);
  void connectToGame();
  void startGameAsClient(QString playerName, QString opponentName,
                         QString clientIp, QString hostIp,
                         unsigned short hostPort);
  void startGameWithAI();
};

#endif // MAINWINDOW_H
