#ifndef SELL_STOCK_WINDOW_H
#define SELL_STOCK_WINDOW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class SellStockWindow : public QWidget {
  Q_OBJECT

private:
  QComboBox *pSelectedStockDisplay;
  QSpinBox *pNumberToSellDisplay;
  QLabel *pSellProfitOrLossDisplay;

  QPushButton *pConfirmSaleButton;
  QPushButton *pCancelSaleButton;

  QHBoxLayout *pButtonLayout;
  QVBoxLayout *pMainLayout;

public:
  SellStockWindow();
  ~SellStockWindow();

public slots:
  void listenForConfirmSale();
  void listenForCancelSale();

signals:
  void notifyOfConfirmSale();
  void notifyOfCancelSale();
};

#endif
