#include "MasterController.h"
#include "AdminController.h"
#include "Authorizer.h"
#include <iostream>

MasterController::MasterController() {
  logInView = new LogInView();

  connect(logInView, &LogInView::notifyOfLogInAttempt, this,
          &MasterController::detectLogin);
  connect(&adminController, &AdminController::informMasterControllerOfLogOut,
          this, &MasterController::listenForLogOut);
  connect(&traderController, &TraderController::informMasterControllerOfLogOut,
          this, &MasterController::listenForLogOut);

  logInView->show();
}

MasterController::~MasterController() {}

void MasterController::executeMainFunctions() {};

void MasterController::executeLogin() {
  std::tuple<Employee, ROLE> logInInformation = logInService.handleLogInAttempt(
      logInView->getEnteredUsername().toStdString(),
      logInView->getEnteredPassword().toStdString());

  Authorizer authorizer(std::get<1>(logInInformation));

  switch (std::get<1>(logInInformation)) {
  case ADMIN:
    logInView->hide();
    adminController.run(&authorizer);
    break;

  case TRADER:
    logInView->hide();
    traderController.run(&authorizer);
    break;
  default:
    std::cout << "INVALID LOG IN!" << std::endl;
  }
};

void MasterController::executeLogOut() { logInView->show(); }

// ************************* SLOTS **********************************
void MasterController::detectLogin() { executeLogin(); }

void MasterController::listenForLogOut() { executeLogOut(); }
