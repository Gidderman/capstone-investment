// The starting point of the application. It constructs a Qt Application, as
// well as the master controller that will direct the rest of the application.

#include "MasterController.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MasterController MasterController;

  return app.exec();
}
