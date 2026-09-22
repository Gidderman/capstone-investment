// This class handles the display of the CustomerManagerView. From this display
// the user is able to purchase/sell stocks for a given customer, as well as see
// the customer information. This window is only accessible from the
// TraderMainView window and reports to the TraderController.

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING MORE WORK FOR FORMATING AND ACTUAL
// STOCK MANAGEMENT FUNCTIONALITY

#ifndef CUSTOMER_MANAGER_VIEW_H
#define CUSTOMER_MANAGER_VIEW_H

#include "PurchaseStockWindow.h"
#include "ScrollableContainer.h"
#include "SellStockWindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

class CustomerManagerView : public QWidget {
  // This macro is required to utilize SLOTS and SIGNALS
  Q_OBJECT

private:
  // Declare the various componenets for the screen
  QLabel *pCustomerFullName;
  QLabel *pCustomerPhoneNumber;
  QLabel *pCustomerEmail;
  QLabel *pDateAccountedOpened;
  QLabel *pAccountType;
  QLabel *pUninvestedFunds;

  // This is a custom widget used to display stock data in a
  // scrollable format
  ScrollableContainer *pHeldStocks;

  QPushButton *pBuyStockButton;
  QPushButton *pSellStockButton;
  QPushButton *pBackButton;

  QHBoxLayout *pHeaderLayout;
  QHBoxLayout *pBodyLayout;
  QVBoxLayout *pCustomerInformationLayout;
  QVBoxLayout *pButtonAndStockDisplayLayout;
  QHBoxLayout *pBuyAndSellStockButtonLayout;
  QVBoxLayout *pMainLayout;

public:
  // We pass in the customer info and the stocks to display upon construction
  CustomerManagerView(std::vector<QString> customerDisplayInfo,
                      std::vector<StockDisplayItem *> *stocks);
  ~CustomerManagerView();
  void refreshPage();

public slots:
  void
  listenForBackButton(); // Connected to the back button on CustomerManagerView
  void listenForBuyStockButton();  // Connected to the buy stock button on
                                   // CustomerManagerView
  void listenForSellStockButton(); // Connected to the sell stock button on
                                   // CustomerManagerView

signals:
  void notifyOfBackButton(); // Notifies TraderController that the user wants to
                             // return to TraderMainView
  void notifyOfBuyStockButton();  // Notifies TraderController that the user
                                  // wants to buy stocks
  void notifyOfSellStockButton(); // Notifies the TraderController that the user
                                  // wants to sell stocks
};

#endif
