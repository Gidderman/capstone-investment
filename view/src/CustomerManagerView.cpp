// This class implements CustomerManagerView.h, see that header file for more
// information

#include "CustomerManagerView.h"
#include "ScrollableContainer.h"

#include <QPainter>

CustomerManagerView::CustomerManagerView(std::vector<QString> customerInfo,
                                         std::vector<Investment> stocks) {

  this->setObjectName("CustomerManagerView");

  this->setMinimumSize(1024, 640);

  QSize MIN_BUTTON_SIZE = {185, 30};

  // Initialize the display components with the appropriate information
  pCustomerFullName = new QLabel(customerInfo.at(0));
  pCustomerFullName->setObjectName("CustomerManagerTitle");

  pCustomerPhoneNumberLabel = new QLabel("Phone Number");
  pCustomerPhoneNumberLabel->setObjectName("CustomerManagerLabel");

  pCustomerEmailLabel = new QLabel("E-Mail Address");
  pCustomerEmailLabel->setObjectName("CustomerManagerLabel");

  pDateAccountedOpenedLabel = new QLabel("Date Account Opened");
  pDateAccountedOpenedLabel->setObjectName("CustomerManagerLabel");

  pAccountTypeLabel = new QLabel("Account Type");
  pAccountTypeLabel->setObjectName("CustomerManagerLabel");

  pUninvestedFundsLabel = new QLabel("Available Funds");
  pUninvestedFundsLabel->setObjectName("CustomerManagerLabel");

  pCustomerPhoneNumber = new QLabel(customerInfo.at(1));
  pCustomerPhoneNumber->setObjectName("CustomerManagerContent");

  pCustomerEmail = new QLabel(customerInfo.at(2));
  pCustomerEmail->setObjectName("CustomerManagerContent");

  pDateAccountedOpened = new QLabel(customerInfo.at(3));
  pDateAccountedOpened->setObjectName("CustomerManagerContent");

  pAccountType = new QLabel(customerInfo.at(4));
  pAccountType->setObjectName("CustomerManagerContent");

  pUninvestedFunds = new QLabel(customerInfo.at(5));
  pUninvestedFunds->setObjectName("CustomerManagerContent");

  pInvestmentsLabel = new QLabel("Customer Investments");
  pInvestmentsLabel->setObjectName("CustomerManagerLabel");

  // After declaring the type of data to be displayed, the list of items
  // to display must be passed in
  pHeldStocks = new ScrollableContainer(STOCKS);
  pHeldStocks->addDisplayList(stocks);

  pBuyStockButton = new QPushButton("Buy Stock");
  pBuyStockButton->setMinimumSize(MIN_BUTTON_SIZE);

  pSellStockButton = new QPushButton("Sell Stock");
  pSellStockButton->setMinimumSize(MIN_BUTTON_SIZE);

  pBackButton = new QPushButton("Back");
  pBackButton->setMinimumSize(MIN_BUTTON_SIZE);

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
  pHeaderLayout->addWidget(pCustomerFullName, 0, Qt::AlignLeft);
  pHeaderLayout->addStretch(5);
  pHeaderLayout->addWidget(pBackButton, 0, Qt::AlignRight);

  pCustomerInformationLayout = new QVBoxLayout();
  pCustomerInformationLayout->addWidget(pCustomerPhoneNumberLabel, 0,
                                        Qt::AlignLeft);
  pCustomerInformationLayout->addWidget(pCustomerPhoneNumber, 0,
                                        Qt::AlignCenter);
  pCustomerInformationLayout->addStretch(1);

  pCustomerInformationLayout->addWidget(pCustomerEmailLabel, 0, Qt::AlignLeft);
  pCustomerInformationLayout->addWidget(pCustomerEmail, 0, Qt::AlignCenter);
  pCustomerInformationLayout->addStretch(1);

  pCustomerInformationLayout->addWidget(pDateAccountedOpenedLabel, 0,
                                        Qt::AlignLeft);
  pCustomerInformationLayout->addWidget(pDateAccountedOpened, 0,
                                        Qt::AlignCenter);
  pCustomerInformationLayout->addStretch(1);

  pCustomerInformationLayout->addWidget(pAccountTypeLabel, 0, Qt::AlignLeft);
  pCustomerInformationLayout->addWidget(pAccountType, 0, Qt::AlignCenter);
  pCustomerInformationLayout->addStretch(1);

  pCustomerInformationLayout->addWidget(pUninvestedFundsLabel, 0,
                                        Qt::AlignLeft);
  pCustomerInformationLayout->addWidget(pUninvestedFunds, 0, Qt::AlignCenter);

  pCustomerInformationLayout->addStretch(5);

  pBuyAndSellStockButtonLayout = new QHBoxLayout();
  pBuyAndSellStockButtonLayout->addStretch(1);
  pBuyAndSellStockButtonLayout->addWidget(pBuyStockButton, 0);
  pBuyAndSellStockButtonLayout->addStretch(1);
  pBuyAndSellStockButtonLayout->addWidget(pSellStockButton, 0);
  pBuyAndSellStockButtonLayout->addStretch(1);

  pButtonAndStockDisplayLayout = new QVBoxLayout();
  pButtonAndStockDisplayLayout->addWidget(pInvestmentsLabel, 0, Qt::AlignLeft);
  pButtonAndStockDisplayLayout->addWidget(pHeldStocks);
  pButtonAndStockDisplayLayout->addLayout(pBuyAndSellStockButtonLayout);

  pBodyLayout = new QHBoxLayout();
  pBodyLayout->addLayout(pCustomerInformationLayout);
  pBodyLayout->addSpacing(10);
  pBodyLayout->addLayout(pButtonAndStockDisplayLayout);

  pMainLayout = new QVBoxLayout(this);
  pMainLayout->addLayout(pHeaderLayout);
  pMainLayout->addSpacing(20);
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

void CustomerManagerView::refreshPage(std::vector<QString> customerDisplayInfo,
                                      std::vector<Investment> stocks) {

  pCustomerFullName->setText(customerDisplayInfo.at(0));
  pCustomerPhoneNumber->setText(customerDisplayInfo.at(1));
  pCustomerEmail->setText(customerDisplayInfo.at(2));
  pDateAccountedOpened->setText(customerDisplayInfo.at(3));
  pAccountType->setText(customerDisplayInfo.at(4));
  pUninvestedFunds->setText(customerDisplayInfo.at(5));

  pHeldStocks->refreshDisplayList(stocks);
}

// This is necessarcy to use style sheets.
void CustomerManagerView::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

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
