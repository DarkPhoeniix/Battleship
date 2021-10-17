#ifndef SCOREBOARDWINDOW_H
#define SCOREBOARDWINDOW_H

#include <QDialog>
#include <QTableView>

namespace Ui {
class ScoreboardWindow;
}

class ScoreboardWindow : public QDialog {
  Q_OBJECT

public:
  explicit ScoreboardWindow(QWidget *parent = nullptr);
  QTableView *view();
  ~ScoreboardWindow();

private:
  Ui::ScoreboardWindow *ui;
};

#endif // SCOREBOARDWINDOW_H
