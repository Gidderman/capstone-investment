#include "CustomerManagerView.h"
#include "ScrollableContainer.h"
#include <qboxlayout.h>
#include <qpushbutton.h>

CustomerManagerView::CustomerManagerView(
    std::vector<StockDisplayItem *> *stocks) {
  pCustomerFullName = new QLabel("Customer Name");
  pCustomerPhoneNumber = new QLabel("XXX-XXX-XXXX");
  pCustomerEmail = new QLabel("email@email.com");
  pDateAccountedOpened = new QLabel("ddMMMYYYY");
  pAccountType = new QLabel("RETIREMENT");
  pUninvestedFunds = new QLabel("$XXXXXX.XX");

  pHeldStocks = new ScrollableContainer(STOCKS);
  pHeldStocks->addDisplayList(stocks);

  pBuyStockButton = new QPushButton("Buy Stock");
  pSellStockButton = new QPushButton("Sell Stock");
  pBackButton = new QPushButton("Back");

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

  connect(pBackButton, &QPushButton::clicked, this,
          &CustomerManagerView::listenForBackButton);
  connect(pBuyStockButton, &QPushButton::clicked, this,
          &CustomerManagerView::listenForBuyStockButton);
  connect(pSellStockButton, &QPushButton::clicked, this,
          &CustomerManagerView::listenForSellStockButton);
}

CustomerManagerView::~CustomerManagerView() {}

//***************************SLOTS***********************************
void CustomerManagerView::listenForBackButton() { emit notifyOfBackButton(); }

void CustomerManagerView::listenForBuyStockButton() {
  emit notifyOfBuyStockButton();
}

void CustomerManagerView::listenForSellStockButton() {
  emit notifyOfSellStockButton();
}
