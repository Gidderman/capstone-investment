#include "MasterController.h"
#include "AdminController.h"
#include "Authorizer.h"
#include "CRUDManager.h"
#include "ErrorWindow.h"
#include "LogInView.h"
#include "TraderController.h"

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
      initService(InitService(dataManager)),
      adminService(AdminService(dataManager)),
      traderService(TraderService(dataManager)),
      adminController(AdminController(adminService)),
      traderController(TraderController(traderService)) {

  logInView = new LogInView();

  connect(&traderController, &TraderController::informMasterControllerOfLogOut,
          this, &MasterController::listenForLogOut);
  connect(&adminController, &AdminController::informMasterControllerOfLogOut,
          this, &MasterController::listenForLogOut);
  connect(logInView, &LogInView::notifyOfLogInAttempt, this,
          &MasterController::detectLogin);
  connect(logInView, &LogInView::notifyOfUsernameEntry, this,
          &MasterController::detectUsernameEntry);
  connect(logInView, &LogInView::userRequestsGoingBack, this,
          &MasterController::listenForGoBack);
  connect(logInView, &LogInView::notifyOfPasswordVerificationFieldChange, this,
          &MasterController::listenForPasswordVerificationFieldChange);
  connect(logInView, &LogInView::notifyOfPasswordCreation, this,
          &MasterController::detectPasswordCreation);

  connect(&initService, &InitService::errorOccurred, this,
          &MasterController::listenForStockRefreshError);
  connect(&initService, &InitService::refreshFinished, this,
          &MasterController::listenForStockRefreshCompletion);

  logInView->runUsernameScreen();
}

MasterController::~MasterController() {}

// TODO: init functions
void MasterController::executeMainFunctions() {
  initService.updateStockData();
};

// Performs the log in functions, determining which controller to log into.
// Additionally, initializes the authorizer setting the allowed role in the
// process.
void MasterController::executeLogin(std::string enteredPassword) {
  // Returns a tuple, the first of which is the employee information which will
  // be used in the appropriate screens. The second index is the allowed ROLE.
  // I've sent that seperately instead using the role within the Employee data
  // struct. This is due to the fact that the handleLogInAttempt function
  // verifies the credentials of the logged in individual, and if that is
  // correct it will set a role. This is an attempt to prevent a false user
  // gaining access unauthorized access while bypassing the credential
  // verification.
  std::optional<std::tuple<Employee, ROLE>> logInInformation;
  try {
    logInInformation = logInService.handleLogInAttempt(enteredPassword);
  } catch (std::logic_error &e) {
    displayError(e.what());
  } catch (std::exception &e) {
    displayError(e.what());
  }

  // An invalid log in was attempted, we exit the function.
  if (!logInInformation.has_value()) {
    logInView->displayWarningText(
        QString("Invalid username or password. Please try again."));
    return;
  }

  // The account was locked during the log in attempts due to invalid
  // information
  if (std::get<1>(logInInformation.value()) == INVALID) {
    logInView->displayWarningText(
        "This account is locked due to greater than three invalid log in "
        "attempts. Please contact your system administrator.");
    return;
  }

  // Pass the allowed role to the authorizer constructer. This sets the
  // authorized role for the entire time the user is logged in, and they must
  // log out and log back in to change it.
  Authorizer authorizer(std::get<1>(logInInformation.value()));

  // depending on the role, move into that portion of the application, hiding
  // the log in window.
  switch (std::get<1>(logInInformation.value())) {
  case ADMIN:

    logInView->hide();
    adminController.run(std::get<0>(logInInformation.value()).accountID,
                        &authorizer);
    break;

  case TRADER:
    logInView->hide();
    traderController.run(std::get<0>(logInInformation.value()), &authorizer);
    break;

  default:
    break;
  }
};

// Clear the credentials from the previous log in and display the log in view.
void MasterController::executeLogOut() {
  logInView->clear();
  logInView->runUsernameScreen();
}

//**************************PRIVATE FUNCTIONS************************
void MasterController::displayError(QString errorText) {
  ErrorWindow *errorWindow = new ErrorWindow(errorText);
  int windowAcknowledged = errorWindow->exec();
  delete errorWindow;
}

// ************************* SLOTS **********************************
void MasterController::detectUsernameEntry(QString username) {
  // Before we do anything, we confirm that the user actually entered a
  // username. If they didn't we throw a error at them.
  if (username.size() <= 0) {
    logInView->displayWarningText(QString("Please enter a username."));
  } else {
    // The first bool returns true if the account for that username does not
    // exist, The second returns true when the user needs to create a new
    // password, The third returns true when the account is locked.
    std::tuple<bool, bool, bool> isTheAccountValid =
        logInService.doesAccountHaveValidCredentials(username.toStdString());

    // First we see if an account exists associated with that username. If not
    // then we proceed as normal, but we won't be incrementing the attempted log
    // in counter as there isn't an account to lock.
    if (std::get<0>(isTheAccountValid)) {
      logInService.setLogAttemptedLogIns(false);
      logInView->runPasswordScreen();
    }
    // Next we check to see if the account is locked. If so we notify the user
    // that their account is locked and they need to seek an admin for
    // assistance.
    else if (std::get<2>(isTheAccountValid)) {
      logInView->displayWarningText(
          QString("This account is locked. Please see your administrator to "
                  "unlock the account."));
    }
    // Finally we check if the account is new and needs to set a password, if so
    // we send then to the password creation screen.
    else if (std::get<1>(isTheAccountValid)) {
      logInService.setLogAttemptedLogIns(true);
      logInView->runCreatePasswordScreen();
    }
    // None of the conditions where true, we will commence with a normal log in.
    else {
      logInService.setLogAttemptedLogIns(true);
      logInView->runPasswordScreen();
    }
  }
}

void MasterController::detectLogin(QString password) {
  // First verify the user has entered a password, and if they haven't then send
  // a warning
  if (password.size() <= 0) {
    logInView->displayWarningText("Please enter a password.");
  } else {
    executeLogin(password.toStdString());
  }
} // connected to the log in button on the log in screen

void MasterController::listenForPasswordVerificationFieldChange(
    QString password, QString passwordVerification) {
  if (!logInService.verifyPasswordsMatch(password.toStdString(),
                                         passwordVerification.toStdString())) {
    logInView->displayWarningText("Passwords do not match.");
  } else {
    logInView->displayWarningText("");
  }
}

void MasterController::detectPasswordCreation(QString password,
                                              QString passwordVerification) {
  if (!logInService.verifyPasswordComplexityRequirements(
          password.toStdString(), passwordVerification.toStdString())) {
    logInView->displayWarningText(
        "Passwords do not meet complexity requirements.");
  } else {
    try {
      logInService.createPassword(password.toStdString());
      logInView->clear();
      logInView->runUsernameScreen();
    } catch (std::logic_error &e) {
      displayError(e.what());

    } catch (std::exception &e) {
      displayError(e.what());
    }
  }
}

void MasterController::listenForLogOut() {
  executeLogOut();
} // connected to the log out buttons on the admin and trader screens

// The user wants to go back to the username entry screen.
void MasterController::listenForGoBack() { logInView->runUsernameScreen(); }

void MasterController::listenForStockRefreshError(std::string errorInfo) {
  displayError(QString::fromStdString(errorInfo));
}

void MasterController::listenForStockRefreshCompletion() {
  initService.commitStockUpdates();
}
