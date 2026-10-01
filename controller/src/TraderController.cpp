// This class implements TraderController.h. For information on the purpose of
// this class, see the header file.

#include "TraderController.h"
#include "Authorizer.h"
#include "Customer.h"
#include "CustomerManagerView.h"
#include "PurchaseStockWindow.h"
#include "SellStockWindow.h"
#include "TraderMainView.h"
#include "TraderService.h"

#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>

TraderController::TraderController(TraderService &traderService)
    : traderService(traderService) {}

TraderController::~TraderController() {}

// This is the primary entry point into the trader screen. It initializes my
// custom containers, performs some of the connections for the slots, and starts
// with a authorization verification.
void TraderController::run(Employee employee, Authorizer *authorizer) {
  // The passed in authorizer has a role set by the log in controller. This is
  // the allowable role for the given user, if it doesn't match the role for
  // this screen then the program will not proceed.
  if (!authorizer->authorizeUser(TRADER)) {
    // TODO: Throw an exception
  }

  loggedInEmployee = employee;

  try {
    pTraderMainView = new TraderMainView(
        formatLoggedInEmployeeForDisplay(),
        traderService.getListOfManagedCustomers(loggedInEmployee.accountID));
  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }

  connect(pTraderMainView, &TraderMainView::notifyOfLogOut, this,
          &TraderController::listenForLogOut);
  connect(pTraderMainView, &TraderMainView::notifyOfCustomerSelection, this,
          &TraderController::listenForCustomerSelection);

  pTraderMainView->show();
}

//*******************PRIVATE FUNCTIONS****************************

// This returns a vector of the logged in employee's information formatted for
// display on screen.
std::vector<QString> TraderController::formatLoggedInEmployeeForDisplay() {
  std::vector<QString> returnVector;
  returnVector.push_back(QString::fromStdString(
      loggedInEmployee.firstName + " " + loggedInEmployee.lastName));
  returnVector.push_back(QString::number(loggedInEmployee.accountID));
  returnVector.push_back(QString::number(loggedInEmployee.numAccountsManaged) +
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

void TraderController::displayError(QString errorText) {
  ErrorWindow *errorWindow = new ErrorWindow(errorText);
  int windowAcknowledged = errorWindow->exec();
  delete errorWindow;
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
  try {
    selectedCustomerId = id;
    Customer selectedCustomer = traderService.getCustomerById(id);

    pCustomerManagerView = new CustomerManagerView(
        formatIndividualCustomerForDisplay(selectedCustomer),
        selectedCustomer.investments);
  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }

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
  std::tuple<QString, QString, QString> stockItem;

  try {
    std::vector<std::tuple<QString, QString, QString>> stockList;

    std::vector<Stock> unformattedStockData =
        traderService.getListOfAvailableStocks();

    for (Stock stock : unformattedStockData) {
      stockItem = {QString::fromStdString(stock.stockName),
                   QString::fromStdString(stock.stockCode),
                   QString::number(stock.stockPrice)};
      stockList.push_back(stockItem);
    }

    pPurchaseStockWindow = new PurchaseStockWindow(stockList);
  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }

  connect(pPurchaseStockWindow, &PurchaseStockWindow::notifyOfConfirmPurchase,
          this, &TraderController::listenForStockPurchaseConfirmation);
  connect(pPurchaseStockWindow, &PurchaseStockWindow::notifyOfCancelPurchase,
          this, &TraderController::listenForStockPurchaseCancellation);
  connect(pPurchaseStockWindow, &PurchaseStockWindow::notifyOfPriceCalculation,
          this, &TraderController::listenForPurchasePriceCalculation);

  pPurchaseStockWindow->run();
}

// This function listens for confirmation of a stock purchase. When it is
// called, it adds the stock to the appropriate customer, and hides the purchase
// stock screen.
void TraderController::listenForStockPurchaseConfirmation(
    std::tuple<QString, int, QString> transaction) {

  try {
    traderService.executeStockPurchase(selectedCustomerId,
                                       {std::get<0>(transaction).toStdString(),
                                        std::get<1>(transaction),
                                        std::get<2>(transaction).toFloat()});

    // We create a Customer variable here to limit database calls when calling
    // the refresh page
    Customer selectedCustomer =
        traderService.getCustomerById(selectedCustomerId);

    pPurchaseStockWindow->hide();
    pCustomerManagerView->refreshPage(
        formatIndividualCustomerForDisplay(selectedCustomer),
        selectedCustomer.investments);
  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// This function listens for cancellation of a stock purchase, which if
// performed will then hide the window.
void TraderController::listenForStockPurchaseCancellation() {
  pPurchaseStockWindow->hide();
}

// This function is called when the user clicks on the Sell Stock button. It
// creates the SellStockWindow, connects the buttons, and shows the window.
void TraderController::listenForStockSaleInitiation() {
  try {
    std::vector<QString> availableStockNames;
    std::vector<QString> availableStockCodes;

    for (Investment investment :
         traderService.getCustomerById(selectedCustomerId).investments) {
      availableStockNames.push_back(
          QString::fromStdString(investment.stock.stockName));
      availableStockCodes.push_back(
          QString::fromStdString(investment.stock.stockCode));
    }

    pSellStockWindow =
        new SellStockWindow({availableStockNames, availableStockCodes});
  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }

  connect(pSellStockWindow, &SellStockWindow::notifyOfConfirmSale, this,
          &TraderController::listenForStockSaleConfirmation);
  connect(pSellStockWindow, &SellStockWindow::notifyOfCancelSale, this,
          &TraderController::listenForStockSaleCancellation);
  connect(pSellStockWindow, &SellStockWindow::notifyOfSaleCalculation, this,
          &TraderController::listenForSalePriceCalculation);

  pSellStockWindow->run();
}

// This function listens for confirmation of a stock sale. When it is called,
// it removes the proper number of stocks from the appropriate customer, and
// hides the sell stock screen.
void TraderController::listenForStockSaleConfirmation(
    std::tuple<QString, int, QString> transaction) {
  try {
    traderService.executeStockSale(selectedCustomerId,
                                   {std::get<0>(transaction).toStdString(),
                                    std::get<1>(transaction),
                                    std::get<2>(transaction).toFloat()});

    Customer selectedCustomer =
        traderService.getCustomerById(selectedCustomerId);

    pSellStockWindow->hide();
    pCustomerManagerView->refreshPage(
        formatIndividualCustomerForDisplay(selectedCustomer),
        selectedCustomer.investments);
  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// This function listens for cancellation of a stock sale, which if performed
// will then hide the window.
void TraderController::listenForStockSaleCancellation() {
  pSellStockWindow->hide();
}

// When notified that the display price should change, displays the following
void TraderController::listenForPurchasePriceCalculation(
    std::tuple<QString, int> stockAndNumber) {
  try {

    pPurchaseStockWindow->setDisplayPrice(
        QString::number(traderService.calculatePriceOfStockPurchase(
                            std::get<0>(stockAndNumber).toStdString(),
                            std::get<1>(stockAndNumber)),
                        'f', 2));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

void TraderController::listenForSalePriceCalculation(
    std::tuple<QString, int> stockAndNumber) {
  try {
    std::vector<float> transactionInfo = traderService.calculateResultOfSale(
        selectedCustomerId, std::get<0>(stockAndNumber).toStdString(),
        std::get<1>(stockAndNumber));

    pSellStockWindow->setDisplayInfo(
        {QString::number(transactionInfo.at(0), 'f', 2),
         QString::number(transactionInfo.at(1), 'f', 2),
         QString::number(transactionInfo.at(2), 'f', 2)},
        (int)transactionInfo.at(3));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}
