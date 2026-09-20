// This class implements TraderController.h. For information on the purpose of
// this class, see the header file.

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

// This is the primary entry point into the trader screen. It initializes my
// custom containers, performs some of the connections for the slots, and starts
// with a authorization verification.
void TraderController::run(Employee loggedInEmployee, Authorizer *authorizer) {
  // The passed in authorizer has a role set by the log in controller. This is
  // the allowable role for the given user, if it doesn't match the role for
  // this screen then the program will not proceed.
  if (!authorizer->authorizeUser(TRADER)) {
    // TODO: Throw an exception
  }

  this->loggedInEmployee = loggedInEmployee;

  // Get the customer list for the logged in employee and format it for display
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
// Uses the passed in employeeID to find all managed customers and returns
// them in a vector.
std::vector<Customer>
TraderController::getListOfManagedCustomers(int employeeID) {
  return traderService.getListOfManagedCustomers(employeeID);
}

// Uses a customer ID to find all stocks that customer is invested in and
// returns them in a vector. This function doesn't make any calls to the
// database, instead searching through the list of managed customers to find the
// appropriate one. If nothing is found, it returns an empty vector. We write to
// two vectors here, one is the returned vector which will be used for any stock
// information management, the other is the class member variable for the
// displayStockInformation, which is formatted for displaying the stock on
// screen.
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

// We format the customers for display, first clearing the list to ensure we
// don't double populate.
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

// This returns a vector of the logged in employee's information formatted for
// display on screen.
std::vector<QString> TraderController::formatLoggedInEmployeeForDisplay() {
  std::vector<QString> returnVector;
  returnVector.push_back(QString::fromStdString(
      loggedInEmployee.firstName + " " + loggedInEmployee.lastName));
  returnVector.push_back(QString::number(loggedInEmployee.accountID));
  returnVector.push_back(QString::number(customerList.size()) +
                         " total accounts managed.");

  return returnVector;
}

// This returns a vector for a selected customer's information formatted for
// display on screen.
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
// When a log out request is made, then the TraderController notifies the
// MasterController and hides its display.
void TraderController::listenForLogOut() {
  pTraderMainView->hide();
  emit informMasterControllerOfLogOut();
}

// This is called when the user double clicks on a displayed customer from the
// TraderMainView screen. It pulls the information of the selected customer,
// formats it for display in the CustomerManagerView, gets the applicable stock
// information and formats that for display, and connects the buttons from the
// CustomerManagerView screen to the appropriate slots before hiding the
// TraderMainView and displaying the CustomerManagerView
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

// This function is called when the back button is clicked in the
// CustomerManagerView
void TraderController::listenForReturnFromCustomerScreen() {
  pCustomerManagerView->hide();
  pTraderMainView->show();
}

// This function is called when the user clicks on the Purchase Stock button. It
// creates the PurchaseStockWindow, connects the buttons, and shows the window.
void TraderController::listenForStockPurchaseInitiation() {
  pPurchaseStockWindow = new PurchaseStockWindow();
  connect(pPurchaseStockWindow, &PurchaseStockWindow::notifyOfConfirmPurchase,
          this, &TraderController::listenForStockPurchaseConfirmation);
  connect(pPurchaseStockWindow, &PurchaseStockWindow::notifyOfCancelPurchase,
          this, &TraderController::listenForStockPurchaseCancellation);

  pPurchaseStockWindow->show();
}

// This function listens for confirmation of a stock purchase. When it is
// called, it adds the stock to the appropriate customer, and hides the purchase
// stock screen.
void TraderController::listenForStockPurchaseConfirmation() {
  // TODO: Logic for purchasing a stock
  pPurchaseStockWindow->hide();
}

// This function listens for cancellation of a stock purchase, which if
// performed will then hide the window.
void TraderController::listenForStockPurchaseCancellation() {
  pPurchaseStockWindow->hide();
}

// This function is called when the user clicks on the Sell Stock button. It
// creates the SellStockWindow, connects the buttons, and shows the window.
void TraderController::listenForStockSaleInitiation() {
  pSellStockWindow = new SellStockWindow();
  connect(pSellStockWindow, &SellStockWindow::notifyOfConfirmSale, this,
          &TraderController::listenForStockSaleConfirmation);
  connect(pSellStockWindow, &SellStockWindow::notifyOfCancelSale, this,
          &TraderController::listenForStockSaleCancellation);

  pSellStockWindow->show();
}

// This function listens for confirmation of a stock sale. When it is called,
// it removes the proper number of stocks from the appropriate customer, and
// hides the sell stock screen.
void TraderController::listenForStockSaleConfirmation() {
  // TODO: Logic for selling a stock
  pSellStockWindow->hide();
}

// This function listens for cancellation of a stock sale, which if performed
// will then hide the window.
void TraderController::listenForStockSaleCancellation() {
  pSellStockWindow->hide();
}
