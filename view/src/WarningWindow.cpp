#include "WarningWindow.h"

WarningWindow::WarningWindow() {
  this->warningText =
      "WARNING: You are about to permanently delete an account. Are you sure?";

  this->setText(warningText);
  this->setStandardButtons(QMessageBox::Apply | QMessageBox::Cancel);
  this->setDefaultButton(QMessageBox::Cancel);
}

WarningWindow::~WarningWindow() {}
