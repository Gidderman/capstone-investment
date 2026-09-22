// This class implements PurchaseStockWindow.h, see the header file for more
// info

#include "PurchaseStockWindow.h"
#include <qcombobox.h>
#include <qsizepolicy.h>
#include <qspinbox.h>

PurchaseStockWindow::PurchaseStockWindow(
    std::vector<std::tuple<QString, QString, QString>> stockList) {
  // Initialize display items
  pSelectedStockDisplay = new QComboBox();
  pSelectedStockCodeDisplay = new QComboBox();

  for (std::tuple<QString, QString, QString> stock : stockList) {
    pSelectedStockDisplay->addItem(std::get<0>(stock));
    pSelectedStockCodeDisplay->addItem(std::get<1>(stock));
  }

  pNumberOfStockToPurchaseDisplay = new QSpinBox();
  pNumberOfStockToPurchaseDisplay->setMinimum(1);

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

  // Connect the price change slot to the spin box for the number of stocks to
  // purchase
  connect(pNumberOfStockToPurchaseDisplay, &QSpinBox::valueChanged, this,
          &PurchaseStockWindow::listenForTotalPriceChange);
  // Connect to the stock code, so a new price will be calculated when a stock
  // is selected
  connect(pSelectedStockCodeDisplay, &QComboBox::currentIndexChanged, this,
          &PurchaseStockWindow::listenForTotalPriceChange);

  // These connections relate to when the user changes the selected stock by
  // name or code, and will update the code or name to match.
  connect(pSelectedStockDisplay, &QComboBox::currentIndexChanged, this,
          &PurchaseStockWindow::matchStockCodeAfterChange);
  connect(pSelectedStockCodeDisplay, &QComboBox::currentIndexChanged, this,
          &PurchaseStockWindow::matchStockNameAfterChange);
}

PurchaseStockWindow::~PurchaseStockWindow() {}

void PurchaseStockWindow::run() {
  emit notifyOfPriceCalculation({pSelectedStockCodeDisplay->currentText(),
                                 pNumberOfStockToPurchaseDisplay->value()});

  this->show();
}

void PurchaseStockWindow::setDisplayPrice(QString price) {
  pTotalPriceOfPurchase->setText(QString("Total Price: $") + price);
}

//**********************SLOTS**************************
// This is called when the user clicks the confirm purchase button,
// which then notifies the TraderController that a stock purchase occured
void PurchaseStockWindow::listenForConfirmPurchase() {
  emit notifyOfConfirmPurchase({pSelectedStockCodeDisplay->currentText(),
                                pNumberOfStockToPurchaseDisplay->value(),
                                pTotalPriceOfPurchase->text()});
}

// This is called when the user clicks the cancel button, which then
// notifies the TraderController to hid this window
void PurchaseStockWindow::listenForCancelPurchase() {
  emit notifyOfCancelPurchase();
}

// This is called whenever the user changes the number of stocks they are
// looking to purchase or the stock that they want to purchase.
void PurchaseStockWindow::listenForTotalPriceChange() {
  std::tuple<QString, int> stockAndNumberForPurchase;
  stockAndNumberForPurchase = {pSelectedStockCodeDisplay->currentText(),
                               pNumberOfStockToPurchaseDisplay->value()};

  emit notifyOfPriceCalculation(stockAndNumberForPurchase);
}

void PurchaseStockWindow::matchStockCodeAfterChange(int index) {
  pSelectedStockCodeDisplay->setCurrentIndex(index);
}

void PurchaseStockWindow::matchStockNameAfterChange(int index) {
  pSelectedStockDisplay->setCurrentIndex(index);
}
