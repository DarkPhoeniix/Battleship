#ifndef HOSTGAMEDIALOG_H
#define HOSTGAMEDIALOG_H

#include <QDialog>

namespace Ui {
class HostGameDialog;
}

class HostGameDialog : public QDialog {
  Q_OBJECT

public:
  explicit HostGameDialog(QWidget *parent = nullptr);
  ~HostGameDialog();
signals:
  void reportInput(unsigned short port, QString playerName,
                   QString opponentName);

private:
  Ui::HostGameDialog *ui;
  bool fillBlanks();
};

#endif // HOSTGAMEDIALOG_H
