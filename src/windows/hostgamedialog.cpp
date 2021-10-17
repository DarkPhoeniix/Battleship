#include "hostgamedialog.h"
#include "ui_hostgamedialog.h"
#include <QRandomGenerator>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QTcpServer>

HostGameDialog::HostGameDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::HostGameDialog) {
  ui->setupUi(this);
  fillBlanks();
}

HostGameDialog::~HostGameDialog() { delete ui; }

void HostGameDialog::fillBlanks() {
  connect(this, &HostGameDialog::accepted, [this]() {
    emit reportInput(ui->clientInput->text(), ui->playerName->text(),
                     ui->opponentName->text());
  });
  for (const auto &address : QNetworkInterface::allAddresses())
    if (address.protocol() == QAbstractSocket::IPv4Protocol &&
        address != QHostAddress::LocalHost)
      ui->ipList->addItem(address.toString());
  ui->playerName->setText(
      QString("Player ") +
      QString::number(QRandomGenerator::system()->bounded(1000, 9999)));
  ui->opponentName->setText(
      QString("Player ") +
      QString::number(QRandomGenerator::system()->bounded(1000, 9999)));
}
