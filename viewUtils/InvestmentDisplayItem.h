// This class is a way to display information within the ScrollableContainer.
// Each instance of the class is a singular Stock that needs to be displayed
// in a specific format.

#ifndef INVESTMENT_DISPLAY_ITEM_H
#define INVESTMENT_DISPLAY_ITEM_H

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class InvestmentDisplayItem : public QWidget {
  // Requried to use signals and slots
  Q_OBJECT

private:
  // Display information

  QLabel *pStockNameLabel;
  QLabel *pStockCodeLabel;
  QLabel *pNumberHeldLabel;
  QLabel *pPricePerStockLabel;
  QLabel *pTotalWorthLabel;

  QLabel *pStockName;
  QLabel *pStockCode;
  QLabel *pNumberHeld;
  QLabel *pPricePerStock;
  QLabel *pTotalWorth;

  QVBoxLayout *pStockNameAndCodeLayout;
  QVBoxLayout *pNumberHeldAndPriceLayout;
  QVBoxLayout *pTotalWorthLayout;

  QHBoxLayout *pLayout;

  int investmentId;

protected:
  // Overrided function to make this widget clickable.
  void mousePressEvent(QMouseEvent *event) override;

public:
  // Pass in the information to display with the constructor
  InvestmentDisplayItem(int investmentId, QString stockName, QString stockCode,
                        QString numberHeld, QString pricePerStock);
  ~InvestmentDisplayItem();

  void paintEvent(QPaintEvent *) override;

signals:
  void clicked();
};

#endif
