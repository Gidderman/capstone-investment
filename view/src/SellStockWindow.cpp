// This class implements SellStockWindow.h, see the header file for more info

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING AND INFORMATION
// POPULATION FOR THE AVAILABLE STOCKS TO SELL

#include "SellStockWindow.h"

SellStockWindow::SellStockWindow() {
  // Initialize the display componenets
  pSelectedStockDisplay = new QComboBox();
  pNumberToSellDisplay = new QSpinBox();
  pSellProfitOrLossDisplay = new QLabel(QString("SALE PROFIT/LOSS"));

  pConfirmSaleButton = new QPushButton(QString("Confirm Sale"));
  pCancelSaleButton = new QPushButton(QString("Cancel"));

  // Set up the layout. The input componenets are displayed from top to bottom
  // in the order they are added to the layout, with the two buttons at the
  // bottom
  pButtonLayout = new QHBoxLayout();
  pButtonLayout->addWidget(pConfirmSaleButton);
  pButtonLayout->addWidget(pCancelSaleButton);

  pMainLayout = new QVBoxLayout(this);
  pMainLayout->addWidget(pSelectedStockDisplay);
  pMainLayout->addWidget(pNumberToSellDisplay);
  pMainLayout->addWidget(pSellProfitOrLossDisplay);
  pMainLayout->addLayout(pButtonLayout);

  // Connections
  connect(pConfirmSaleButton, &QPushButton::clicked, this,
          &SellStockWindow::listenForConfirmSale);
  connect(pCancelSaleButton, &QPushButton::clicked, this,
          &SellStockWindow::listenForCancelSale);
}

SellStockWindow::~SellStockWindow() {}

//**********************************SLOTS***************************
// Called when the user clicks the ConfirmSale button, notifying the trader
// controller that a sale was made
void SellStockWindow::listenForConfirmSale() { emit notifyOfConfirmSale(); }

// Called when the user clicks the cancel button, notifying the trader
// controller that a sale was cancelled, causing the controller to hide the pop
// up window
void SellStockWindow::listenForCancelSale() { emit notifyOfCancelSale(); }
