#include "connecttogamedialog.h"
#include "ui_connecttogamedialog.h"
#include <QRandomGenerator>
#include <QtNetwork/QNetworkInterface>

ConnectToGameDialog::ConnectToGameDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ConnectToGameDialog) {
  ui->setupUi(this);
  fillBlanks();
}

ConnectToGameDialog::~ConnectToGameDialog() { delete ui; }

void ConnectToGameDialog::fillBlanks() {
  connect(this, &ConnectToGameDialog::accepted, [this]() {
    auto endpoint = ui->endpointInput->text().split(":");
    emit reportInput(ui->playerName->text(), ui->opponentName->text(),
                     ui->ipInput->currentText(), endpoint[0],
                     endpoint[1].toUShort());
  });
  for (const auto &address : QNetworkInterface::allAddresses())
    if (address.protocol() == QAbstractSocket::IPv4Protocol &&
        address != QHostAddress::LocalHost)
      ui->ipInput->addItem(address.toString());
  ui->playerName->setText(
      QString("Player ") +
      QString::number(QRandomGenerator::system()->bounded(1000, 9999)));
  ui->opponentName->setText(
      QString("Player ") +
      QString::number(QRandomGenerator::system()->bounded(1000, 9999)));
}
