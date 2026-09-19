#include "TraderController.h"
#include "Authorizer.h"
#include "Customer.h"
#include "CustomerManagerView.h"
#include "PurchaseStockWindow.h"
#include "SellStockWindow.h"
#include "StockDisplayItem.h"
#include "TraderMainView.h"

#include <iostream>

TraderController::TraderController() {}

TraderController::~TraderController() {}

void TraderController::run(Employee loggedInEmployee, Authorizer *authorizer) {
  if (!authorizer->authorizeUser(TRADER)) {
    // TODO: Throw an exception
  }

  this->loggedInEmployee = loggedInEmployee;

  customerList = getListOfManagedCustomers(this->loggedInEmployee.accountID);

  formatAllManagedCustomersForDisplay();

  pTraderMainView = new TraderMainView(formatLoggedInEmployeeForDisplay(),
                                       &customerDisplayList);
  pCustomerManagerView = nullptr;

  connect(pTraderMainView, &TraderMainView::notifyOfLogOut, this,
          &TraderController::listenForLogOut);
  connect(pTraderMainView, &TraderMainView::notifyOfCustomerSelection, this,
          &TraderController::listenForCustomerSelection);

  pTraderMainView->show();
}

//*******************PRIVATE FUNCTIONS****************************
std::vector<Customer>
TraderController::getListOfManagedCustomers(int employeeID) {
  return traderService.getListOfManagedCustomers(employeeID);
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

void TraderController::formatAllManagedCustomersForDisplay() {
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

std::vector<QString> TraderController::formatLoggedInEmployeeForDisplay() {
  std::vector<QString> returnVector;
  returnVector.push_back(QString::fromStdString(
      loggedInEmployee.firstName + " " + loggedInEmployee.lastName));
  returnVector.push_back(QString::number(loggedInEmployee.accountID));
  returnVector.push_back(QString::number(customerList.size()) +
                         " total accounts managed.");

  return returnVector;
}

std::vector<QString>
TraderController::formatIndividualCustomerForDisplay(Customer customer) {
  std::vector<QString> returnVector;
  returnVector.push_back(
      QString::fromStdString(customer.firstName + " " + customer.lastName));
  returnVector.push_back(QString::fromStdString(customer.phoneNum));
  returnVector.push_back(QString::fromStdString(customer.email));
  returnVector.push_back(QString::fromStdString(customer.dateAccountOpened));
  if (customer.accountType == RETIREMENT) {
    returnVector.push_back(QString("Retirement"));
  } else {
    returnVector.push_back(QString("Brokerage"));
  }
  returnVector.push_back(QString::number(customer.uninvestedFunds));

  return returnVector;
}

//*************************SLOTS*************************************
void TraderController::listenForLogOut() {
  pTraderMainView->hide();
  emit informMasterControllerOfLogOut();
}

void TraderController::listenForCustomerSelection(int id) {
  stockListForSelectedCustomer = getListAndFormatOfCustomerStock(id);
  Customer customerToDisplay;
  for (Customer customer : customerList) {
    if (customer.customerID == id) {
      customerToDisplay = customer;
    }
  }
  pCustomerManagerView = new CustomerManagerView(
      formatIndividualCustomerForDisplay(customerToDisplay),
      &stockByCustomerDisplayList);

  connect(pCustomerManagerView, &CustomerManagerView::notifyOfBackButton, this,
          &TraderController::listenForReturnFromCustomerScreen);
  connect(pCustomerManagerView, &CustomerManagerView::notifyOfBuyStockButton,
          this, &TraderController::listenForStockPurchaseInitiation);
  connect(pCustomerManagerView, &CustomerManagerView::notifyOfSellStockButton,
          this, &TraderController::listenForStockSaleInitiation);

  pTraderMainView->hide();
  pCustomerManagerView->show();
}

void TraderController::listenForReturnFromCustomerScreen() {
  std::cout << "Hey we are trying to return..." << std::endl;
  pCustomerManagerView->hide();
  pTraderMainView->show();
}

void TraderController::listenForStockPurchaseInitiation() {
  pPurchaseStockWindow = new PurchaseStockWindow();
  connect(pPurchaseStockWindow, &PurchaseStockWindow::notifyOfConfirmPurchase,
          this, &TraderController::listenForStockPurchaseConfirmation);
  connect(pPurchaseStockWindow, &PurchaseStockWindow::notifyOfCancelPurchase,
          this, &TraderController::listenForStockPurchaseCancellation);

  pPurchaseStockWindow->show();
}

void TraderController::listenForStockPurchaseConfirmation() {
  // TODO: Logic for purchasing a stock
  pPurchaseStockWindow->hide();
}

void TraderController::listenForStockPurchaseCancellation() {
  pPurchaseStockWindow->hide();
}

void TraderController::listenForStockSaleInitiation() {
  pSellStockWindow = new SellStockWindow();
  connect(pSellStockWindow, &SellStockWindow::notifyOfConfirmSale, this,
          &TraderController::listenForStockSaleConfirmation);
  connect(pSellStockWindow, &SellStockWindow::notifyOfCancelSale, this,
          &TraderController::listenForStockSaleCancellation);

  pSellStockWindow->show();
}

void TraderController::listenForStockSaleConfirmation() {
  // TODO: Logic for selling a stock
  pSellStockWindow->hide();
}

void TraderController::listenForStockSaleCancellation() {
  pSellStockWindow->hide();
}
