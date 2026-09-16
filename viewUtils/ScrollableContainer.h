#ifndef SCROLLABLE_CONTAINER_H
#define SCROLLABLE_CONTAINER_H

#include "CustomerDisplayItem.h"
#include "TraderDisplayItem.h"

#include <QVBoxLayout>
#include <QWidget>
#include <vector>

enum CONTAINER_TYPE { CUSTOMERS, TRADERS };

class ScrollableContainer : public QWidget {
  Q_OBJECT

private:
  CONTAINER_TYPE containerType;
  QVBoxLayout *pLayout;
  std::vector<CustomerDisplayItem *> *pCustomerDisplayList;
  std::vector<TraderDisplayItem *> *pTraderDisplayList;

public:
  ScrollableContainer(CONTAINER_TYPE containerType);
  ~ScrollableContainer();
  void addDisplayList(std::vector<CustomerDisplayItem *> *diplayList);
  void addDisplayList(std::vector<TraderDisplayItem *> *displayList);
  void refreshDisplayList();
};

#endif
