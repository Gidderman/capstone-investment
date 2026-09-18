#ifndef SCROLLABLE_CONTAINER_H
#define SCROLLABLE_CONTAINER_H

#include "CustomerDisplayItem.h"
#include "StockDisplayItem.h"
#include "TraderDisplayItem.h"

#include <QVBoxLayout>
#include <QWidget>
#include <vector>

enum CONTAINER_TYPE { CUSTOMERS, TRADERS, STOCKS };

class ScrollableContainer : public QWidget {
  Q_OBJECT

private:
  CONTAINER_TYPE containerType;
  QVBoxLayout *pLayout;
  std::vector<CustomerDisplayItem *> *pCustomerDisplayList;
  std::vector<TraderDisplayItem *> *pTraderDisplayList;
  std::vector<StockDisplayItem *> *pStockDisplayList;

public:
  ScrollableContainer(CONTAINER_TYPE containerType);
  ~ScrollableContainer();
  void addDisplayList(std::vector<CustomerDisplayItem *> *diplayList);
  void addDisplayList(std::vector<TraderDisplayItem *> *displayList);
  void addDisplayList(std::vector<StockDisplayItem *> *displayList);
  void refreshDisplayList();

public slots:
  void listenForCustomerItemSelection();
  void listenForEmployeeItemSelection();
  // TODO: Stock display items

signals:
  void notifyOfCustomerItemSelection();
  void notifyOfEmployeeItemSelection();
  // TODO: stock display items
};

#endif
