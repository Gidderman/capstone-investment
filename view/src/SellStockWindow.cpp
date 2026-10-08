// This class implements SellStockWindow.h, see the header file for more info

#include "SellStockWindow.h"

#include <QPainter>

SellStockWindow::SellStockWindow(
    std::tuple<std::vector<QString>, std::vector<QString>> stockChoices) {

  this->setObjectName("SellStockWindow");

  pStockNameLabel = new QLabel("Stock Name");
  pStockNameLabel->setObjectName("SellStockLabel");

  pStockCodeLabel = new QLabel("Stock Code");
  pStockCodeLabel->setObjectName("SellStockLabel");

  pNumToSellLabel = new QLabel("Number to Sell");
  pNumToSellLabel->setObjectName("SellStockLabel");

  pInitialInvestmentLabel = new QLabel("Intial Investment ($)");
  pInitialInvestmentLabel->setObjectName("SellStockLabel");

  pCurrentInvestmentWorthLabel = new QLabel("Current Worth ($)");
  pCurrentInvestmentWorthLabel->setObjectName("SellStockLabel");

  pProfitOrLossLabel = new QLabel("Result of Sell ($)");
  pProfitOrLossLabel->setObjectName("SellStockLabel");

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

  pInitialInvestmentDisplay = new QLabel();
  pInitialInvestmentDisplay->setObjectName("SellStockContent");

  pCurrentWorthDisplay = new QLabel();
  pCurrentWorthDisplay->setObjectName("SellStockContent");

  pSellProfitOrLossDisplay = new QLabel();
  pSellProfitOrLossDisplay->setObjectName("SellStockContent");

  pConfirmSaleButton = new QPushButton(QString("Confirm Sale"));
  pCancelSaleButton = new QPushButton(QString("Cancel"));

  // Set up the layout. The input componenets are displayed from top to bottom
  // in the order they are added to the layout, with the two buttons at the
  // bottom
  pButtonLayout = new QHBoxLayout();
  pButtonLayout->addStretch(1);
  pButtonLayout->addWidget(pConfirmSaleButton, 0);
  pButtonLayout->addStretch(1);
  pButtonLayout->addWidget(pCancelSaleButton, 0);
  pButtonLayout->addStretch(1);

  pMainLayout = new QVBoxLayout(this);
  pMainLayout->addWidget(pStockNameLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pSelectedStockDisplay);
  pMainLayout->addSpacing(20);

  pMainLayout->addWidget(pStockCodeLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pSelectedStockCodeDisplay);
  pMainLayout->addSpacing(20);

  pMainLayout->addWidget(pNumToSellLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pNumberToSellDisplay);
  pMainLayout->addSpacing(20);

  pMainLayout->addWidget(pInitialInvestmentLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pInitialInvestmentDisplay, 0, Qt::AlignCenter);
  pMainLayout->addSpacing(20);

  pMainLayout->addWidget(pCurrentInvestmentWorthLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pCurrentWorthDisplay, 0, Qt::AlignCenter);
  pMainLayout->addSpacing(20);

  pMainLayout->addWidget(pProfitOrLossLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pSellProfitOrLossDisplay, 0, Qt::AlignCenter);
  pMainLayout->addSpacing(20);

  pMainLayout->addLayout(pButtonLayout);
  pMainLayout->addStretch(5);

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
  pInitialInvestmentDisplay->setText(displayData.at(0));
  pCurrentWorthDisplay->setText(displayData.at(1));
  pSellProfitOrLossDisplay->setText(displayData.at(2));
  pNumberToSellDisplay->setMaximum(maxNumberAllowedToSell);
}

// This is necessarcy to use style sheets.
void SellStockWindow::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
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
