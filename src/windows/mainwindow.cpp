#include "mainwindow.h"
#include "db/database.h"
#include "ui_mainwindow.h"
#include "windows/scoreboardwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showScoreboardSummary() {
  auto window = new ScoreboardWindow(this);
  window->view()->setModel(Battleship::Database::Instance().summary());
  window->show();
}

void MainWindow::startSingleScreenGame() {}

void MainWindow::hostGame() {}

void MainWindow::connectToGame() {}
