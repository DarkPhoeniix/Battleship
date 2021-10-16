#include <QtNetwork/QTcpServer>

unsigned short getPort() {
  try {
    QTcpServer srv;
    srv.listen();
    return srv.serverPort();
  } catch (...) {
    return 0;
  }
}
