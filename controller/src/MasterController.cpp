#include "MasterController.h"
#include "AdminController.h"
#include "Authorizer.h"
#include "TraderController.h"

#include <iostream>

// Initializes the view and makes the connections between the log out buttons
// from the admin main view and trader main view, as well as the log in button
// from the log in veiw. At the end, it displays the log in screen.
MasterController::MasterController() {
  logInView = new LogInView();

  connect(&traderController, &TraderController::informMasterControllerOfLogOut,
          this, &MasterController::listenForLogOut);
  connect(&adminController, &AdminController::informMasterControllerOfLogOut,
          this, &MasterController::listenForLogOut);
  connect(logInView, &LogInView::notifyOfLogInAttempt, this,
          &MasterController::detectLogin);

  logInView->show();
}

MasterController::~MasterController() {}

// TODO: init functions
void MasterController::executeMainFunctions() {};

// Performs the log in functions, determining which controller to log into.
// Additionally, initializes the authorizer setting the allowed role in the
// process.
void MasterController::executeLogin() {
  // Returns a tuple, the first of which is the employee information which will
  // be used in the appropriate screens. The second index is the allowed ROLE.
  // I've sent that seperately instead using the role within the Employee data
  // struct. This is due to the fact that the handleLogInAttempt function
  // verifies the credentials of the logged in individual, and if that is
  // correct it will set a role. This is an attempt to prevent a false user
  // gaining access unauthorized access while bypassing the credential
  // verification.
  std::tuple<Employee, ROLE> logInInformation = logInService.handleLogInAttempt(
      logInView->getEnteredUsername().toStdString(),
      logInView->getEnteredPassword().toStdString());

  // Pass the allowed role to the authorizer constructer. This sets the
  // authorized role for the entire time the user is logged in, and they must
  // log out and log back in to change it.
  Authorizer authorizer(std::get<1>(logInInformation));

  // depending on the role, move into that portion of the application, hiding
  // the log in window.
  switch (std::get<1>(logInInformation)) {
  case ADMIN:
    logInView->hide();
    adminController.run(std::get<0>(logInInformation).accountID, &authorizer);
    break;

  case TRADER:
    logInView->hide();
    traderController.run(std::get<0>(logInInformation).accountID, &authorizer);
    break;

  default:
    std::cout << "INVALID LOG IN!" << std::endl;
  }
};

// Clear the credentials from the previous log in and display the log in view.
void MasterController::executeLogOut() {
  logInView->clear();
  logInView->show();
}

// ************************* SLOTS **********************************
void MasterController::detectLogin() {
  executeLogin();
} // connected to the log in button on the log in screen

void MasterController::listenForLogOut() {
  executeLogOut();
} // connected to the log out buttons on the admin and trader screens
