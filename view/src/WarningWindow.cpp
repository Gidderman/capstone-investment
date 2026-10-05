#include "WarningWindow.h"

WarningWindow::WarningWindow(QString warningText) {
  this->warningText = warningText;

  this->setText(warningText);
  this->setStandardButtons(QMessageBox::Apply | QMessageBox::Cancel);
  this->setDefaultButton(QMessageBox::Cancel);
}

WarningWindow::~WarningWindow() {}
