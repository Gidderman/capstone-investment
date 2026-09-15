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
  std::cout << "BUTTON CLICK DETECTED" << std::endl;
};

// ************************* SLOTS **********************************
void MasterController::detectLogin() { executeLogin(); };
