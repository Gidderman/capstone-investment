#include "SellStockWindow.h"

SellStockWindow::SellStockWindow() {
  pSelectedStockDisplay = new QComboBox();
  pNumberToSellDisplay = new QSpinBox();
  pSellProfitOrLossDisplay = new QLabel(QString("SALE PROFIT/LOSS"));

  pConfirmSaleButton = new QPushButton(QString("Confirm Sale"));
  pCancelSaleButton = new QPushButton(QString("Cancel"));

  pButtonLayout = new QHBoxLayout();
  pButtonLayout->addWidget(pConfirmSaleButton);
  pButtonLayout->addWidget(pCancelSaleButton);

  pMainLayout = new QVBoxLayout(this);
  pMainLayout->addWidget(pSelectedStockDisplay);
  pMainLayout->addWidget(pNumberToSellDisplay);
  pMainLayout->addWidget(pSellProfitOrLossDisplay);
  pMainLayout->addLayout(pButtonLayout);

  connect(pConfirmSaleButton, &QPushButton::clicked, this,
          &SellStockWindow::listenForConfirmSale);
  connect(pCancelSaleButton, &QPushButton::clicked, this,
          &SellStockWindow::listenForCancelSale);
}

SellStockWindow::~SellStockWindow() {}

//**********************************SLOTS***************************
void SellStockWindow::listenForConfirmSale() { emit notifyOfConfirmSale(); }

void SellStockWindow::listenForCancelSale() { emit notifyOfCancelSale(); }
