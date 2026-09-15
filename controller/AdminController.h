#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include "AdminMainView.h"
#include "AdminService.h"
#include "CustomerCreationView.h"
#include "TraderCreationView.h"
#include <QObject>

class AdminController : public QObject {
  Q_OBJECT

private:
public:
  AdminController();
  ~AdminController();
};

#endif
