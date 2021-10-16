#ifndef CONNECTTOGAMEDIALOG_H
#define CONNECTTOGAMEDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectToGameDialog;
}

class ConnectToGameDialog : public QDialog {
  Q_OBJECT

public:
  explicit ConnectToGameDialog(QWidget *parent = nullptr);
  ~ConnectToGameDialog();
signals:
  void reportInput(QString playerName, QString opponentName, QString clientIp,
                   QString hostIp, unsigned short hostPort);

private:
  Ui::ConnectToGameDialog *ui;
  void fillBlanks();
};

#endif // CONNECTTOGAMEDIALOG_H
