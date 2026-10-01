#include "ErrorWindow.h"

ErrorWindow::ErrorWindow(QString errorMessage) {

  this->setText(errorMessage);
  this->setStandardButtons(QMessageBox::Ok);
  this->setDefaultButton(QMessageBox::Ok);
}

ErrorWindow::~ErrorWindow() {}
