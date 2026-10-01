#include "MasterController.h"
#include "AdminController.h"
#include "Authorizer.h"
#include "CRUDManager.h"
#include "TraderController.h"

#include <iostream>
#include <ostream>
#include <stdexcept>

// Initializes the view and makes the connections between the log out buttons
// from the admin main view and trader main view, as well as the log in button
// from the log in veiw. At the end, it displays the log in screen.
// Additionally creates instances of all the services and controllers. This is
// done to prevent recreating database connections, ensuring that each service
// and controller only connects to a single DataManager and therefore a single
// CRUD manager
MasterController::MasterController()
    : crudManager(CRUDManager()), dataManager(DataManager(crudManager)),
      logInService(LogInService(dataManager)),
      adminService(AdminService(dataManager)),
      traderService(TraderService(dataManager)),
      adminController(AdminController(adminService)),
      traderController(TraderController(traderService)) {

  std::cout << "MasterController::MasterController - entering" << std::endl;

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
  std::tuple<Employee, ROLE> logInInformation;
  try {
    logInInformation = logInService.handleLogInAttempt();

  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }

  std::cout << "MasterController::executeLogIn - returned from LogInService"
            << std::endl;

  // Pass the allowed role to the authorizer constructer. This sets the
  // authorized role for the entire time the user is logged in, and they must
  // log out and log back in to change it.
  Authorizer authorizer(std::get<1>(logInInformation));

  // depending on the role, move into that portion of the application, hiding
  // the log in window.
  switch (std::get<1>(logInInformation)) {
  case ADMIN:
    std::cout << "MasterController::executeLogIn - attempting admin log in"
              << std::endl;

    logInView->hide();
    adminController.run(std::get<0>(logInInformation).accountID, &authorizer);
    break;

  case TRADER:
    logInView->hide();
    traderController.run(std::get<0>(logInInformation), &authorizer);
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

//**************************PRIVATE FUNCTIONS************************
void MasterController::displayError(QString errorText) {
  ErrorWindow *errorWindow = new ErrorWindow(errorText);
  int windowAcknowledged = errorWindow->exec();
  delete errorWindow;
}

// ************************* SLOTS **********************************
void MasterController::detectUsernameEntry(QString username) {}

void MasterController::detectLogin(QString password) {
  executeLogin();
} // connected to the log in button on the log in screen

void MasterController::detectPasswordCreation(QString password,
                                              QString passwordVerification) {}

void MasterController::listenForLogOut() {
  executeLogOut();
} // connected to the log out buttons on the admin and trader screens
