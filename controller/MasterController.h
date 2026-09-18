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
  LogInView *logInView;

  LogInService logInService;
  AdminController adminController;
  TraderController traderController;

public:
  MasterController();
  ~MasterController();
  void executeMainFunctions();
  void executeLogin();
  void executeLogOut();

public slots:
  void detectLogin();
  void listenForLogOut();
};

#endif
