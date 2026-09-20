// This class is a way to display information within the ScrollableContainer.
// Each instance of the class is a singular Stock that needs to be displayed
// in a specific format.

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING AND
// COMMUNICATION VIA SLOTS AND SIGNALS

#ifndef STOCK_DISPLAY_ITEM_H
#define STOCK_DISPLAY_ITEM_H

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QString>
#include <QWidget>

class StockDisplayItem : public QWidget {
  // Requried to use signals and slots
  Q_OBJECT

private:
  // Display information
  QLabel *pStockName;
  QLabel *pStockCode;
  QLabel *pNumberHeld;
  QLabel *pPricePerStock;
  QLabel *pTotalWorth;

  QHBoxLayout *pLayout;

protected:
  // Overrided function to make this widget clickable.
  void mousePressEvent(QMouseEvent *event) override;

public:
  // Pass in the information to display with the constructor
  StockDisplayItem(QString stockName, QString stockCode, QString numberHeld,
                   QString pricePerStock);
  ~StockDisplayItem();

signals:
  void clicked();
};

#endif
