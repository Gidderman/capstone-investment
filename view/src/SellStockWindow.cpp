// This class implements SellStockWindow.h, see the header file for more info

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING AND INFORMATION
// POPULATION FOR THE AVAILABLE STOCKS TO SELL

#include "SellStockWindow.h"
#include <qcombobox.h>

SellStockWindow::SellStockWindow(
    std::tuple<std::vector<QString>, std::vector<QString>> stockChoices) {
  // Initialize the display componenets
  pSelectedStockDisplay = new QComboBox();
  for (QString stockName : std::get<0>(stockChoices)) {
    pSelectedStockDisplay->addItem(stockName);
  }

  pSelectedStockCodeDisplay = new QComboBox();
  for (QString stockCode : std::get<1>(stockChoices)) {
    pSelectedStockCodeDisplay->addItem(stockCode);
  }

  pNumberToSellDisplay = new QSpinBox();
  pNumberToSellDisplay->setMinimum(1);

  pInitialInvestmentDisplay = new QLabel(QString("Initial investment"));
  pCurrentWorthDisplay = new QLabel(QString("Current Investment Worth"));
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
  pMainLayout->addWidget(pSelectedStockCodeDisplay);
  pMainLayout->addWidget(pNumberToSellDisplay);
  pMainLayout->addWidget(pInitialInvestmentDisplay);
  pMainLayout->addWidget(pCurrentWorthDisplay);
  pMainLayout->addWidget(pSellProfitOrLossDisplay);
  pMainLayout->addLayout(pButtonLayout);

  // Connections
  connect(pConfirmSaleButton, &QPushButton::clicked, this,
          &SellStockWindow::listenForConfirmSale);
  connect(pCancelSaleButton, &QPushButton::clicked, this,
          &SellStockWindow::listenForCancelSale);
  connect(pSelectedStockDisplay, &QComboBox::currentIndexChanged, this,
          &SellStockWindow::matchStockCodeAfterChange);
  connect(pSelectedStockCodeDisplay, &QComboBox::currentIndexChanged, this,
          &SellStockWindow::matchStockNameAfterChange);
  connect(pNumberToSellDisplay, &QSpinBox::valueChanged, this,
          &SellStockWindow::listenForNumOfStockChange);
}

SellStockWindow::~SellStockWindow() {}

void SellStockWindow::run() {
  emit notifyOfSaleCalculation({pSelectedStockCodeDisplay->currentText(),
                                pNumberToSellDisplay->value()});
  this->show();
}

void SellStockWindow::setDisplayInfo(std::vector<QString> displayData,
                                     int maxNumberAllowedToSell) {
  pInitialInvestmentDisplay->setText(QString("Initial Amount Invested: $") +
                                     displayData.at(0));
  pCurrentWorthDisplay->setText(QString("Current Investment Worth: $") +
                                displayData.at(1));
  pSellProfitOrLossDisplay->setText(QString("Transaction Result: $") +
                                    displayData.at(2));
  pNumberToSellDisplay->setMaximum(maxNumberAllowedToSell);
}

//**********************************SLOTS***************************
// Called when the user clicks the ConfirmSale button, notifying the trader
// controller that a sale was made
void SellStockWindow::listenForConfirmSale() {
  emit notifyOfConfirmSale({pSelectedStockCodeDisplay->currentText(),
                            pNumberToSellDisplay->value(),
                            pSellProfitOrLossDisplay->text()});
}

// Called when the user clicks the cancel button, notifying the trader
// controller that a sale was cancelled, causing the controller to hide the pop
// up window
void SellStockWindow::listenForCancelSale() { emit notifyOfCancelSale(); }

void SellStockWindow::listenForNumOfStockChange(int number) {
  emit notifyOfSaleCalculation(
      {pSelectedStockCodeDisplay->currentText(), number});
}

void SellStockWindow::matchStockCodeAfterChange(int index) {
  pSelectedStockCodeDisplay->setCurrentIndex(index);
  emit notifyOfSaleCalculation({pSelectedStockCodeDisplay->currentText(),
                                pNumberToSellDisplay->value()});
}

void SellStockWindow::matchStockNameAfterChange(int index) {
  pSelectedStockDisplay->setCurrentIndex(index);
}
