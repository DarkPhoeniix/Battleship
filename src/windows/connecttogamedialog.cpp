#include "connecttogamedialog.h"
#include "ui_connecttogamedialog.h"
#include <QtNetwork/QNetworkInterface>

ConnectToGameDialog::ConnectToGameDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ConnectToGameDialog) {
  ui->setupUi(this);
  fillBlanks();
}

ConnectToGameDialog::~ConnectToGameDialog() { delete ui; }

void ConnectToGameDialog::fillBlanks() {
  connect(this, &ConnectToGameDialog::accepted,
          [this]() { emit reportInput(ui->hostInput->text()); });
  for (const auto &address : QNetworkInterface::allAddresses())
    if (address.protocol() == QAbstractSocket::IPv4Protocol &&
        address != QHostAddress::LocalHost)
      ui->ipList->addItem(address.toString());
}
