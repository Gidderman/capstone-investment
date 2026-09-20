// This class implements StockDisplayItem.h, see header for more information

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING AND
// COMMUNICATION VIA SLOTS AND SIGNALS

#include "StockDisplayItem.h"

StockDisplayItem::StockDisplayItem(QString stockName, QString stockCode,
                                   QString numberHeld, QString pricePerStock) {
  // Initialize the display variables
  this->pStockName = new QLabel(stockName);
  this->pStockCode = new QLabel(stockCode);
  this->pNumberHeld = new QLabel(numberHeld);
  this->pPricePerStock = new QLabel(pricePerStock);
  float totalWorth = numberHeld.toFloat() * pricePerStock.toFloat();
  this->pTotalWorth = new QLabel(QString::number(totalWorth));

  // Set up the layout. Items are displayed from left to right
  // in the order they are added
  pLayout = new QHBoxLayout(this);
  pLayout->addWidget(pStockName);
  pLayout->addWidget(pStockCode);
  pLayout->addWidget(pNumberHeld);
  pLayout->addWidget(pPricePerStock);
  pLayout->addWidget(pTotalWorth);
}

StockDisplayItem::~StockDisplayItem() {}

// When the item is double clicked, tell the scrollable container
void StockDisplayItem::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    // TODO: highlight the stock
  }
}
