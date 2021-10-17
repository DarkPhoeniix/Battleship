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
  void reportInput(QString clientIp, QString playerName, QString opponentName);

private:
  Ui::HostGameDialog *ui;
  void fillBlanks();
};

#endif // HOSTGAMEDIALOG_H
