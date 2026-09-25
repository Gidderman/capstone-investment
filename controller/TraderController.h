// The TraderController handles all the input and output for the TraderMainView,
// as well as directs the TraderService to perform various actions. It will
// direct the display of four seperate screens, the TraderMainView which
// displays the logged in trader's information, as well as the list of customer
// accounts that they manage, the CustomerManagerView which displays the
// customer information and the the stocks that the customer is invested in, the
// PurchaseStockWindow which allows the trader to purchase stocks, and the
// SellStockWindow which allows the trader to sell stocks.

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
  // This macro is necessary to handle Qt SIGNALS and SLOTS
  Q_OBJECT

private:
  // Screen declaration
  TraderMainView *pTraderMainView;
  CustomerManagerView *pCustomerManagerView;
  PurchaseStockWindow *pPurchaseStockWindow;
  SellStockWindow *pSellStockWindow;

  // Service initialization
  TraderService traderService;

  // This id are used for displaying the logged in employee information as
  // well as finding and displaying the associated customers
  int loggedInEmployeeId;

  // This id is used to track the customer being managed when the user
  // selects the customer from the main trader page.
  int selectedCustomerId;

  // The following functions format various data for display, which is necessary
  // due to most display items requiring a QString.
  std::vector<CustomerDisplayItem *> formatAllManagedCustomersForDisplay();
  std::vector<InvestmentDisplayItem *>
  formatCustomerInvestmentsForDisplay(std::vector<Investment> investments);
  std::vector<QString> formatLoggedInEmployeeForDisplay();
  std::vector<QString> formatIndividualCustomerForDisplay(Customer customer);

public:
  TraderController();
  ~TraderController();
  // This is the primary entry point into this class.
  void run(int loggedInEmployeeId, Authorizer *authorizer);

public slots:
  void
  listenForLogOut(); // Connected to the log out button on the MainTraderScreen
  void listenForCustomerSelection(
      int id); // Connected to individual customer displays, called when the
               // user double clicks on a displayed customer
  void listenForReturnFromCustomerScreen(); // Connected to the back button on
                                            // the CustomerManagerView
  void listenForStockPurchaseInitiation();  // Connected to the Buy Stock button
                                            // on the CustomerManagerView
  void listenForStockPurchaseConfirmation(
      std::tuple<QString, int, QString>
          transaction); // Connected to the Confirm Purchase
                        // button on the PurchaseStockWindow
  void
  listenForStockPurchaseCancellation(); // Connected to the Cancel Purchase
                                        // button on the PurchaseStockWindow
  void listenForStockSaleInitiation();  // Connected to the Sell Stock button on
                                        // the CustomerManagerView
  void listenForStockSaleConfirmation(
      std::tuple<QString, int, QString>
          transaction);                  // Connected to the Confirm Sale button
                                         // on the SellStockWindow
  void listenForStockSaleCancellation(); // Connected to the Cancel Sale button
                                         // on the SellStockWindow
  void listenForPurchasePriceCalculation(
      std::tuple<QString, int>
          stockAndNumber); // Connected to the notifyOfPriceCalculation signal
                           // from the PurchaseStockWindow. Used to calculate
                           // the price of purchase that is displayed

  void listenForSalePriceCalculation(
      std::tuple<QString, int>
          stockAndNumber); // Connected to notifyOfSaleCalculation in
                           // SellStockWindow, used to calculate display
                           // information

signals:
  void informMasterControllerOfLogOut(); // Informs the MasterController that a
                                         // log out has occured.
};

#endif
