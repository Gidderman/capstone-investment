#include "TraderController.h"
#include "Authorizer.h"
#include "Customer.h"
#include "CustomerManagerView.h"
#include "StockDisplayItem.h"
#include "TraderMainView.h"

#include <iostream>

TraderController::TraderController() {
  customerList =
      getListOfManagedCustomers(0001); // TODO: pass in the current employee?
  formatManagedCustomersForDisplay();
  pTraderMainView = new TraderMainView(&customerDisplayList);
  pCustomerManagerView = nullptr;

  connect(pTraderMainView, &TraderMainView::notifyOfLogOut, this,
          &TraderController::listenForLogOut);
  connect(pTraderMainView, &TraderMainView::notifyOfCustomerSelection, this,
          &TraderController::listenForCustomerSelection);
}

TraderController::~TraderController() {}

void TraderController::run(Authorizer *authorizer) {
  if (!authorizer->authorizeUser(TRADER)) {
    // TODO: Throw an exception
  }

  pTraderMainView->show();
}

//*******************PRIVATE FUNCTIONS****************************
std::vector<Customer>
TraderController::getListOfManagedCustomers(int employeeID) {
  // TODO: the actual logic
  Stock stock = {0001, "Apple", "APL", 1500.00f};
  Stock stock2 = {0002, "Nvidia", "NVD", 300.00f};
  Investment investment = {0001, stock, 10, 100.00f, 0001};
  Investment investment2 = {0002, stock2, 10, 100.00f, 0001};
  std::vector<Investment> investments = {investment, investment2};
  Customer testCustomer = {
      0001,        "John",    "Smith",    1008008000,  "JSmith@email.com",
      "20Feb2000", BROKERAGE, 200000.00f, investments, 0001};
  std::vector<Customer> testingData = {testCustomer};

  return testingData;
}

std::vector<Stock>
TraderController::getListAndFormatOfCustomerStock(int customerID) {
  stockListForSelectedCustomer.clear();
  stockByCustomerDisplayList.clear();

  for (Customer customer : customerList) {
    if (customerID == customer.customerID) {
      for (Investment investment : customer.investments) {
        stockListForSelectedCustomer.push_back(investment.stock);

        StockDisplayItem *displayFormat = new StockDisplayItem(
            QString::fromStdString(investment.stock.stockName),
            QString::fromStdString(investment.stock.stockCode),
            QString::number(investment.numHeld),
            QString::number(investment.stock.stockPrice));

        stockByCustomerDisplayList.push_back(displayFormat);
      }
      return stockListForSelectedCustomer;
    }
  }

  return stockListForSelectedCustomer;
}

void TraderController::formatManagedCustomersForDisplay() {
  customerDisplayList.clear();

  for (Customer customer : customerList) {
    float currentWorth = 0.0f;

    for (Investment investment : customer.investments) {
      currentWorth += investment.currentInvestmentWorth;
    }

    std::string customerDisplayName =
        customer.lastName + ", " + customer.firstName.at(0);

    CustomerDisplayItem *displayItem = new CustomerDisplayItem(
        QString::fromStdString(customerDisplayName),
        QString::number(customer.customerID), QString::number(currentWorth),
        QString::number(customer.uninvestedFunds));

    customerDisplayList.push_back(displayItem);
  }
}

//*************************SLOTS*************************************
void TraderController::listenForLogOut() {
  pTraderMainView->hide();
  emit informMasterControllerOfLogOut();
}

void TraderController::listenForCustomerSelection() {
  stockListForSelectedCustomer = getListAndFormatOfCustomerStock(0001);
  pCustomerManagerView = new CustomerManagerView(&stockByCustomerDisplayList);

  connect(pCustomerManagerView, &CustomerManagerView::notifyOfBackButton, this,
          &TraderController::listenForReturnFromCustomerScreen);

  pTraderMainView->hide();
  pCustomerManagerView->show();
}

void TraderController::listenForReturnFromCustomerScreen() {
  std::cout << "Hey we are trying to return..." << std::endl;
  pCustomerManagerView->hide();
  pTraderMainView->show();
}
