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
  std::tuple<Employee, ROLE> logInInformation = logInService.handleLogInAttempt(
      logInView->getEnteredUsername().toStdString(),
      logInView->getEnteredPassword().toStdString());

  switch (std::get<1>(logInInformation)) {
  case ADMIN:
    std::cout << "ADMIN LOG IN INITIATED" << std::endl;
    break;

  case TRADER:
    std::cout << "TRADER LOG IN INITIATED" << std::endl;
    break;
  default:
    std::cout << "INVALID LOG IN!" << std::endl;
  }
};

// ************************* SLOTS **********************************
void MasterController::detectLogin() { executeLogin(); };
