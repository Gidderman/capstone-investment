// This class implements StockDisplayItem.h, see header for more information

#include "InvestmentDisplayItem.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

InvestmentDisplayItem::InvestmentDisplayItem(int investmentId,
                                             QString stockName,
                                             QString stockCode,
                                             QString numberHeld,
                                             QString pricePerStock) {
  this->setObjectName("DisplayItem");

  this->investmentId = investmentId;

  // Initialize the display variables
  pStockNameLabel = new QLabel("Stock Name");
  pStockNameLabel->setObjectName("DisplayItemLabel");

  pStockCodeLabel = new QLabel("Stock Code");
  pStockCodeLabel->setObjectName("DisplayItemLabel");

  pNumberHeldLabel = new QLabel("Number Held");
  pNumberHeldLabel->setObjectName("DisplayItemLabel");

  pPricePerStockLabel = new QLabel("Price Per Stock ($)");
  pPricePerStockLabel->setObjectName("DisplayItemLabel");

  pTotalWorthLabel = new QLabel("Total Investment Value");
  pTotalWorthLabel->setObjectName("DisplayItemLabel");

  this->pStockName = new QLabel(stockName);
  this->pStockName->setObjectName("DisplayItemContent");

  this->pStockCode = new QLabel(stockCode);
  this->pStockCode->setObjectName("DisplayItemContent");

  this->pNumberHeld = new QLabel(numberHeld);
  this->pNumberHeld->setObjectName("DisplayItemContent");

  this->pPricePerStock = new QLabel(pricePerStock);
  this->pPricePerStock->setObjectName("DisplayItemContent");

  float totalWorth = numberHeld.toFloat() * pricePerStock.toFloat();
  this->pTotalWorth = new QLabel(QString::number(totalWorth));
  this->pTotalWorth->setObjectName("DisplayItemContent");

  // Set up the layout. Items are displayed from left to right
  // in the order they are added

  pStockNameAndCodeLayout = new QVBoxLayout();
  pStockNameAndCodeLayout->addWidget(pStockNameLabel, 0, Qt::AlignLeft);
  pStockNameAndCodeLayout->addWidget(pStockName, 0, Qt::AlignLeft);
  pStockNameAndCodeLayout->addWidget(pStockCodeLabel, 0, Qt::AlignLeft);
  pStockNameAndCodeLayout->addWidget(pStockCode, 0, Qt::AlignLeft);

  pNumberHeldAndPriceLayout = new QVBoxLayout();
  pNumberHeldAndPriceLayout->addWidget(pNumberHeldLabel, 0, Qt::AlignLeft);
  pNumberHeldAndPriceLayout->addWidget(pNumberHeld, 0, Qt::AlignLeft);
  pNumberHeldAndPriceLayout->addWidget(pPricePerStockLabel, 0, Qt::AlignLeft);
  pNumberHeldAndPriceLayout->addWidget(pPricePerStock, 0, Qt::AlignLeft);

  pTotalWorthLayout = new QVBoxLayout();
  pTotalWorthLayout->addWidget(pTotalWorthLabel, 0, Qt::AlignLeft);
  pTotalWorthLayout->addWidget(pTotalWorth, 0, Qt::AlignLeft);
  pTotalWorthLayout->addStretch(1);

  pLayout = new QHBoxLayout(this);
  pLayout->addLayout(pStockNameAndCodeLayout);
  pLayout->addLayout(pNumberHeldAndPriceLayout);
  pLayout->addLayout(pTotalWorthLayout);
}

InvestmentDisplayItem::~InvestmentDisplayItem() {}

// This is necessarcy to use style sheets.
void InvestmentDisplayItem::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// When the item is double clicked, tell the scrollable container
void InvestmentDisplayItem::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    // TODO: highlight the stock
  }
}
