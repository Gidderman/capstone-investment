#ifndef TRADER_CONTROLLER_H
#define TRADER_CONTROLLER_H

#include "Authorizer.h"
#include "CustomerDisplayItem.h"
#include "CustomerManagerView.h"
#include "TraderMainView.h"
#include "TraderService.h"

#include <QObject>
#include <vector>

class TraderController : public QObject {
  Q_OBJECT

private:
  TraderMainView *pTraderMainView;
  CustomerManagerView *pCustomerManagerView;

  std::vector<Customer> customerList;
  std::vector<Stock> stockListForSelectedCustomer;

  std::vector<CustomerDisplayItem *> customerDisplayList;
  std::vector<StockDisplayItem *> stockByCustomerDisplayList;

  std::vector<Customer> getListOfManagedCustomers(int employeeID);
  std::vector<Stock> getListAndFormatOfCustomerStock(int customerID);

  void formatManagedCustomersForDisplay();

public:
  TraderController();
  ~TraderController();
  void run(Authorizer *authorizer);

public slots:
  void listenForLogOut();
  void listenForCustomerSelection();
  void listenForReturnFromCustomerScreen();

signals:
  void informMasterControllerOfLogOut();
};

#endif
