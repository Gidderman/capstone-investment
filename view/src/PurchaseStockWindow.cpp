// This class implements PurchaseStockWindow.h, see the header file for more
// info

#include "PurchaseStockWindow.h"

PurchaseStockWindow::PurchaseStockWindow() {
  // Initialize display items
  pSelectedStockDisplay = new QComboBox();
  pSelectedStockCodeDisplay = new QComboBox();
  pNumberOfStockToPurchaseDisplay = new QSpinBox();
  pTotalPriceOfPurchase = new QLabel(QString("TOTAL PRICE"));

  pConfirmPurchaseButton = new QPushButton(QString("Confirm Purchase"));
  pCancelPurchaseButton = new QPushButton(QString("Cancel"));

  // Set up the Layout. The entry items are displayed in the order they
  // are added to the layout, with the buttons at the bottom
  pButtonLayout = new QHBoxLayout();
  pButtonLayout->addWidget(pConfirmPurchaseButton);
  pButtonLayout->addWidget(pCancelPurchaseButton);

  pMainLayout = new QVBoxLayout(this);
  pMainLayout->addWidget(pSelectedStockDisplay);
  pMainLayout->addWidget(pSelectedStockCodeDisplay);
  pMainLayout->addWidget(pNumberOfStockToPurchaseDisplay);
  pMainLayout->addWidget(pTotalPriceOfPurchase);
  pMainLayout->addLayout(pButtonLayout);

  // Make connections
  connect(pConfirmPurchaseButton, &QPushButton::clicked, this,
          &PurchaseStockWindow::listenForConfirmPurchase);
  connect(pCancelPurchaseButton, &QPushButton::clicked, this,
          &PurchaseStockWindow::listenForCancelPurchase);
}

PurchaseStockWindow::~PurchaseStockWindow() {}

//**********************SLOTS**************************
// This is called when the user clicks teh confirm purchase button,
// which then notifies the TraderController that a stock purchase occured
void PurchaseStockWindow::listenForConfirmPurchase() {
  emit notifyOfConfirmPurchase();
}

// This is called when the user clicks the cancel button, which then
// notifies the TraderController to hid this window
void PurchaseStockWindow::listenForCancelPurchase() {
  emit notifyOfCancelPurchase();
}
