#include "hostgamedialog.h"
#include "ui_hostgamedialog.h"
#include <QRandomGenerator>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QTcpServer>

HostGameDialog::HostGameDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::HostGameDialog) {
  ui->setupUi(this);
  if (!fillBlanks())
    reject();
}

HostGameDialog::~HostGameDialog() { delete ui; }

unsigned short getPort() {
  try {
    QTcpServer srv;
    srv.listen();
    return srv.serverPort();
  } catch (...) {
    return 0;
  }
}

bool HostGameDialog::fillBlanks() {
  auto port = getPort();
  if (!port)
    return false;
  connect(this, &HostGameDialog::accepted, [this, port]() {
    emit reportInput(port, ui->playerName->text(), ui->opponentName->text());
  });
  for (const auto &address : QNetworkInterface::allAddresses())
    if (address.protocol() == QAbstractSocket::IPv4Protocol &&
        address != QHostAddress::LocalHost)
      ui->ipList->addItem(address.toString() + ":" + QString::number(port));
  ui->playerName->setText(
      QString("Player ") +
      QString::number(QRandomGenerator::system()->bounded(1000, 9999)));
  ui->opponentName->setText(
      QString("Player ") +
      QString::number(QRandomGenerator::system()->bounded(1000, 9999)));
  return true;
}
