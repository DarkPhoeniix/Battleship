#include "windows/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  srand(time(NULL));
  QApplication a(argc, argv);
  do {
    MainWindow window;
    window.show();
    auto code = QApplication::exec();
    switch (code) {
    case MainWindow::RestartCode:
      break;
    default:
      return code;
    }
  } while (true);
}
