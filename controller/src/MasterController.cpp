#include "MasterController.h"
#include <iostream>

MasterController::MasterController() {
  logInView = new LogInView();

  connect(logInView, &LogInView::notifyOfLogInAttempt, this,
          &MasterController::detectLogin);

  logInView->show();
}

MasterController::~MasterController() {}

void MasterController::executeMainFunctions() {};

void MasterController::executeLogin() {
  ROLE userRole = logInService.handleLogInAttempt(
      logInView->getEnteredUsername().toStdString(),
      logInView->getEnteredPassword().toStdString());
  if (userRole == ROLE::ADMIN) {
    std::cout << "LOG IN TO ADMIN" << std::endl;
  }
};

// ************************* SLOTS **********************************
void MasterController::detectLogin() { executeLogin(); };
