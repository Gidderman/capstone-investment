#include "PurchaseStockWindow.h"
#include <qpushbutton.h>

PurchaseStockWindow::PurchaseStockWindow() {
  pSelectedStockDisplay = new QComboBox();
  pSelectedStockCodeDisplay = new QComboBox();
  pNumberOfStockToPurchaseDisplay = new QSpinBox();
  pTotalPriceOfPurchase = new QLabel(QString("TOTAL PRICE"));

  pConfirmPurchaseButton = new QPushButton(QString("Confirm Purchase"));
  pCancelPurchaseButton = new QPushButton(QString("Cancel"));

  pButtonLayout = new QHBoxLayout();
  pButtonLayout->addWidget(pConfirmPurchaseButton);
  pButtonLayout->addWidget(pCancelPurchaseButton);

  pMainLayout = new QVBoxLayout(this);
  pMainLayout->addWidget(pSelectedStockDisplay);
  pMainLayout->addWidget(pSelectedStockCodeDisplay);
  pMainLayout->addWidget(pNumberOfStockToPurchaseDisplay);
  pMainLayout->addWidget(pTotalPriceOfPurchase);
  pMainLayout->addLayout(pButtonLayout);

  connect(pConfirmPurchaseButton, &QPushButton::clicked, this,
          &PurchaseStockWindow::listenForConfirmPurchase);
  connect(pCancelPurchaseButton, &QPushButton::clicked, this,
          &PurchaseStockWindow::listenForCancelPurchase);
}

PurchaseStockWindow::~PurchaseStockWindow() {}

//**********************SLOTS**************************
void PurchaseStockWindow::listenForConfirmPurchase() {
  emit notifyOfConfirmPurchase();
}

void PurchaseStockWindow::listenForCancelPurchase() {
  emit notifyOfCancelPurchase();
}
