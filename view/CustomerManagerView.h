#ifndef CUSTOMER_MANAGER_VIEW_H
#define CUSTOMER_MANAGER_VIEW_H

#include "PurchaseStockWindow.h"
#include "ScrollableContainer.h"
#include "SellStockWindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

class CustomerManagerView : public QWidget {
  Q_OBJECT

private:
  QLabel *pCustomerFullName;
  QLabel *pCustomerPhoneNumber;
  QLabel *pCustomerEmail;
  QLabel *pDateAccountedOpened;
  QLabel *pAccountType;
  QLabel *pUninvestedFunds;

  ScrollableContainer *pHeldStocks;

  QPushButton *pBuyStockButton;
  QPushButton *pSellStockButton;
  QPushButton *pBackButton;

  QHBoxLayout *pHeaderLayout;
  QHBoxLayout *pBodyLayout;
  QVBoxLayout *pCustomerInformationLayout;
  QVBoxLayout *pButtonAndStockDisplayLayout;
  QHBoxLayout *pBuyAndSellStockButtonLayout;
  QVBoxLayout *pMainLayout;

public:
  CustomerManagerView(std::vector<StockDisplayItem *> *stocks);
  ~CustomerManagerView();

public slots:
  void listenForBackButton();
  void listenForBuyStockButton();
  void listenForSellStockButton();

signals:
  void notifyOfBackButton();
  void notifyOfBuyStockButton();
  void notifyOfSellStockButton();
};

#endif
