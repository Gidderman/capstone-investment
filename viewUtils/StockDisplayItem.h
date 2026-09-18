#ifndef STOCK_DISPLAY_ITEM_H
#define STOCK_DISPLAY_ITEM_H

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QString>
#include <QWidget>

class StockDisplayItem : public QWidget {
  Q_OBJECT

private:
  QLabel *pStockName;
  QLabel *pStockCode;
  QLabel *pNumberHeld;
  QLabel *pPricePerStock;
  QLabel *pTotalWorth;

  QHBoxLayout *pLayout;

protected:
  void mousePressEvent(QMouseEvent *event) override;

public:
  StockDisplayItem(QString stockName, QString stockCode, QString numberHeld,
                   QString pricePerStock);
  ~StockDisplayItem();

signals:
  void clicked();
};

#endif
