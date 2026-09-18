#include "StockDisplayItem.h"
#include <qboxlayout.h>

StockDisplayItem::StockDisplayItem(QString stockName, QString stockCode,
                                   QString numberHeld, QString pricePerStock) {
  this->pStockName = new QLabel(stockName);
  this->pStockCode = new QLabel(stockCode);
  this->pNumberHeld = new QLabel(numberHeld);
  this->pPricePerStock = new QLabel(pricePerStock);
  float totalWorth = numberHeld.toFloat() * pricePerStock.toFloat();
  this->pTotalWorth = new QLabel(QString::number(totalWorth));

  pLayout = new QHBoxLayout(this);
  pLayout->addWidget(pStockName);
  pLayout->addWidget(pStockCode);
  pLayout->addWidget(pNumberHeld);
  pLayout->addWidget(pPricePerStock);
  pLayout->addWidget(pTotalWorth);
}

StockDisplayItem::~StockDisplayItem() {}

void StockDisplayItem::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    // TODO: highlight the stock
  }
}
