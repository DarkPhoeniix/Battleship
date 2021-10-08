#include "scoreboardwindow.h"
#include "ui_scoreboardwindow.h"

ScoreboardWindow::ScoreboardWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::ScoreboardWindow) {
  ui->setupUi(this);
}

QTableView *ScoreboardWindow::view() { return ui->tableView; }

ScoreboardWindow::~ScoreboardWindow() { delete ui; }
