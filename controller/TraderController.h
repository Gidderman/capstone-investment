#ifndef TRADER_CONTROLLER_H
#define TRADER_CONTROLLER_H

#include "Authorizer.h"
#include "CustomerDisplayItem.h"
#include "CustomerManagerView.h"
#include "PurchaseStockWindow.h"
#include "SellStockWindow.h"
#include "TraderMainView.h"
#include "TraderService.h"

#include <QObject>
#include <vector>

class TraderController : public QObject {
  Q_OBJECT

private:
  TraderMainView *pTraderMainView;
  CustomerManagerView *pCustomerManagerView;
  PurchaseStockWindow *pPurchaseStockWindow;
  SellStockWindow *pSellStockWindow;

  TraderService traderService;
  Employee loggedInEmployee;

  std::vector<Customer> customerList;
  std::vector<Stock> stockListForSelectedCustomer;

  std::vector<CustomerDisplayItem *> customerDisplayList;
  std::vector<StockDisplayItem *> stockByCustomerDisplayList;

  std::vector<Customer> getListOfManagedCustomers(int employeeID);
  std::vector<Stock> getListAndFormatOfCustomerStock(int customerID);

  void formatAllManagedCustomersForDisplay();
  std::vector<QString> formatLoggedInEmployeeForDisplay();
  std::vector<QString> formatIndividualCustomerForDisplay(Customer customer);

public:
  TraderController();
  ~TraderController();
  void run(Employee loggedInEmployee, Authorizer *authorizer);

public slots:
  void listenForLogOut();
  void listenForCustomerSelection(int id);
  void listenForReturnFromCustomerScreen();
  void listenForStockPurchaseInitiation();
  void listenForStockPurchaseConfirmation();
  void listenForStockPurchaseCancellation();
  void listenForStockSaleInitiation();
  void listenForStockSaleConfirmation();
  void listenForStockSaleCancellation();

signals:
  void informMasterControllerOfLogOut();
};

#endif
