// This class implements CustomerManagerView.h, see that header file for more
// information

#include "CustomerManagerView.h"
#include "ScrollableContainer.h"
#include <qboxlayout.h>
#include <qpushbutton.h>

CustomerManagerView::CustomerManagerView(
    std::vector<QString> customerInfo,
    std::vector<StockDisplayItem *> *stocks) {

  // Initialize the display components with the appropriate information
  pCustomerFullName = new QLabel(customerInfo.at(0));
  pCustomerPhoneNumber = new QLabel(customerInfo.at(1));
  pCustomerEmail = new QLabel(customerInfo.at(2));
  pDateAccountedOpened = new QLabel(customerInfo.at(3));
  pAccountType = new QLabel(customerInfo.at(4));
  pUninvestedFunds = new QLabel(customerInfo.at(5));

  // After declaring the type of data to be displayed, the list of items
  // to display must be passed in
  pHeldStocks = new ScrollableContainer(STOCKS);
  pHeldStocks->addDisplayList(stocks);

  pBuyStockButton = new QPushButton("Buy Stock");
  pSellStockButton = new QPushButton("Sell Stock");
  pBackButton = new QPushButton("Back");

  // Set up the screen in the appropriate layout:
  // Across the top from left to right:
  //   Customer name
  //   Back button
  // In the middle of the page:
  //   On the left in a column is the customer information
  //   To the right is the list of all investments the customer has
  // At the bottom from left to right:
  //   Buy Stock button
  //   Sell stock button
  pHeaderLayout = new QHBoxLayout();
  pHeaderLayout->addWidget(pCustomerFullName);
  pHeaderLayout->addWidget(pBackButton);

  pCustomerInformationLayout = new QVBoxLayout();
  pCustomerInformationLayout->addWidget(pCustomerPhoneNumber);
  pCustomerInformationLayout->addWidget(pCustomerEmail);
  pCustomerInformationLayout->addWidget(pDateAccountedOpened);
  pCustomerInformationLayout->addWidget(pAccountType);
  pCustomerInformationLayout->addWidget(pUninvestedFunds);

  pBuyAndSellStockButtonLayout = new QHBoxLayout();
  pBuyAndSellStockButtonLayout->addWidget(pBuyStockButton);
  pBuyAndSellStockButtonLayout->addWidget(pSellStockButton);

  pButtonAndStockDisplayLayout = new QVBoxLayout();
  pButtonAndStockDisplayLayout->addWidget(pHeldStocks);
  pButtonAndStockDisplayLayout->addLayout(pBuyAndSellStockButtonLayout);

  pBodyLayout = new QHBoxLayout();
  pBodyLayout->addLayout(pCustomerInformationLayout);
  pBodyLayout->addLayout(pButtonAndStockDisplayLayout);

  pMainLayout = new QVBoxLayout(this);
  pMainLayout->addLayout(pHeaderLayout);
  pMainLayout->addLayout(pBodyLayout);

  // Make the appropriate connections
  connect(pBackButton, &QPushButton::clicked, this,
          &CustomerManagerView::listenForBackButton);
  connect(pBuyStockButton, &QPushButton::clicked, this,
          &CustomerManagerView::listenForBuyStockButton);
  connect(pSellStockButton, &QPushButton::clicked, this,
          &CustomerManagerView::listenForSellStockButton);
}

CustomerManagerView::~CustomerManagerView() {}

//***************************SLOTS***********************************
// Called when the user clicks the back button, tells the trader controller to
// stop displaying the CustomerManagerView and instead display the
// MainTraderView
void CustomerManagerView::listenForBackButton() { emit notifyOfBackButton(); }

// Called when the user clicks the buy stock button, tells the trader controller
// to display the buy stock window
void CustomerManagerView::listenForBuyStockButton() {
  emit notifyOfBuyStockButton();
}

// Called when the user clicks the sell stock button, tells the trader
// controller to display the sell stock window
void CustomerManagerView::listenForSellStockButton() {
  emit notifyOfSellStockButton();
}
