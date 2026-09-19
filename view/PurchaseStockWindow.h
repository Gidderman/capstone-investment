#ifndef PURCHASE_STOCK_WINDOW_H
#define PURCHASE_STOCK_WINDOW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class PurchaseStockWindow : public QWidget {
  Q_OBJECT

private:
  QComboBox *pSelectedStockDisplay;
  QComboBox *pSelectedStockCodeDisplay;
  QSpinBox *pNumberOfStockToPurchaseDisplay;
  QLabel *pTotalPriceOfPurchase;

  QPushButton *pConfirmPurchaseButton;
  QPushButton *pCancelPurchaseButton;

  QHBoxLayout *pButtonLayout;
  QVBoxLayout *pMainLayout;

public:
  PurchaseStockWindow();
  ~PurchaseStockWindow();

public slots:
  void listenForConfirmPurchase();
  void listenForCancelPurchase();

signals:
  void notifyOfConfirmPurchase();
  void notifyOfCancelPurchase();
};

#endif
