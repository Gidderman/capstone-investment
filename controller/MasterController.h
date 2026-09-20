// The Master Controller is the main entry point for the application. It handles
// displaying the input and output of the Log In screen, as well as directing
// both the LogInService class for handling log ins, and the InitService class
// for handling various functions that need to run upon application initiation.
// In addition to those responsibilities this class also will declare and
// initialize the controllers for the Admin user and the Trader user.

#ifndef MASTER_CONTROLLER_H
#define MASTER_CONTROLLER_H

#include "AdminController.h"
#include "Authorizer.h"
#include "InitService.h"
#include "LogInService.h"
#include "LogInView.h"
#include "TraderController.h"

#include <QObject>
#include <tuple>

class MasterController : public QObject {
  // Necessary macro for using signals and slots
  Q_OBJECT

private:
  // Screen initialization
  LogInView *logInView;

  // Service initializations
  LogInService logInService;

  // Other controllers
  AdminController adminController;
  TraderController traderController;

public:
  MasterController();
  ~MasterController();
  // The below function would be used to direct the application initialization
  // functions
  void executeMainFunctions();
  // Directs the log in service to begin a log in.
  void executeLogin();
  // Shows the log in screen upon log out.
  void executeLogOut();

public slots:
  // Used to determine when the user clicks the log in button.
  void detectLogin();
  // Used to detect when the adminController or traderController log out
  void listenForLogOut();
};

#endif
