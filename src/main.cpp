#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QPushButton button("Battleship", nullptr);
  button.resize(300, 200);
  button.show();
  return QApplication::exec();
}
