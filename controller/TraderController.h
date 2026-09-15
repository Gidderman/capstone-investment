#ifndef TRADER_CONTROLLER_H
#define TRADER_CONTROLLER_H

#include "CustomerManagerView.h"
#include "TraderMainView.h"
#include "TraderService.h"
#include <QObject>

class TraderController : public QObject {
  Q_OBJECT

private:
public:
  TraderController();
  ~TraderController();
};

#endif
