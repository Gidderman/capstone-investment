// This class implements PurchaseStockWindow.h, see the header file for more
// info

#include "PurchaseStockWindow.h"

#include <QPainter>
#include <qnamespace.h>

PurchaseStockWindow::PurchaseStockWindow(
    std::vector<std::tuple<QString, QString, QString>> stockList) {

  this->setObjectName("PurchaseStockWindow");

  // Initialize display items
  pStockNameLabel = new QLabel("Stock Name");
  pStockNameLabel->setObjectName("PurchaseStockLabel");

  pStockCodeLabel = new QLabel("Stock Code");
  pStockCodeLabel->setObjectName("PurchaseStockLabel");

  pNumToPurchaseLabel = new QLabel("Number to Buy");
  pNumToPurchaseLabel->setObjectName("PurchaseStockLabel");

  pPriceOfPurchaseLabel = new QLabel("Total Cost of Purchase ($)");
  pPriceOfPurchaseLabel->setObjectName("PurchaseStockLabel");

  pSelectedStockDisplay = new QComboBox();
  pSelectedStockCodeDisplay = new QComboBox();

  for (std::tuple<QString, QString, QString> stock : stockList) {
    pSelectedStockDisplay->addItem(std::get<0>(stock));
    pSelectedStockCodeDisplay->addItem(std::get<1>(stock));
  }

  pNumberOfStockToPurchaseDisplay = new QSpinBox();
  pNumberOfStockToPurchaseDisplay->setMinimum(1);

  pTotalPriceOfPurchase = new QLabel();
  pTotalPriceOfPurchase->setObjectName("PurchaseStockContent");

  pConfirmPurchaseButton = new QPushButton(QString("Confirm Purchase"));
  pCancelPurchaseButton = new QPushButton(QString("Cancel"));

  // Set up the Layout. The entry items are displayed in the order they
  // are added to the layout, with the buttons at the bottom
  pButtonLayout = new QHBoxLayout();
  pButtonLayout->addStretch(1);
  pButtonLayout->addWidget(pConfirmPurchaseButton, 0);
  pButtonLayout->addStretch(1);
  pButtonLayout->addWidget(pCancelPurchaseButton, 0);
  pButtonLayout->addStretch(1);

  pMainLayout = new QVBoxLayout(this);
  pMainLayout->addWidget(pStockNameLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pSelectedStockDisplay);
  pMainLayout->addSpacing(20);

  pMainLayout->addWidget(pStockCodeLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pSelectedStockCodeDisplay);
  pMainLayout->addSpacing(20);

  pMainLayout->addWidget(pNumToPurchaseLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pNumberOfStockToPurchaseDisplay);
  pMainLayout->addSpacing(20);

  pMainLayout->addWidget(pPriceOfPurchaseLabel, 0, Qt::AlignLeft);
  pMainLayout->addWidget(pTotalPriceOfPurchase, 0, Qt::AlignCenter);
  pMainLayout->addSpacing(20);

  pMainLayout->addLayout(pButtonLayout, 1);
  pMainLayout->addStretch(5);

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
  pTotalPriceOfPurchase->setText(price);
}

// This is necessarcy to use style sheets.
void PurchaseStockWindow::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
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
