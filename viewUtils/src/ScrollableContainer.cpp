#include "ScrollableContainer.h"
#include <qboxlayout.h>
#include <qcoreevent.h>

ScrollableContainer::ScrollableContainer(CONTAINER_TYPE containerType)
    : containerType(containerType) {
  pLayout = new QVBoxLayout(this);
}

ScrollableContainer::~ScrollableContainer() {}

void ScrollableContainer::addDisplayList(
    std::vector<CustomerDisplayItem *> *displayList) {
  this->pCustomerDisplayList = displayList;
  refreshDisplayList();
}

void ScrollableContainer::addDisplayList(
    std::vector<TraderDisplayItem *> *displayList) {
  this->pTraderDisplayList = displayList;
  refreshDisplayList();
}

void ScrollableContainer::addDisplayList(
    std::vector<StockDisplayItem *> *displayList) {
  this->pStockDisplayList = displayList;
  refreshDisplayList();
}

void ScrollableContainer::refreshDisplayList() {
  if (containerType == CUSTOMERS) {
    for (CustomerDisplayItem *displayItem : *pCustomerDisplayList) {
      pLayout->addWidget(displayItem);
      connect(displayItem, &CustomerDisplayItem::clicked, this,
              &ScrollableContainer::listenForCustomerItemSelection);
    }
  } else if (containerType == TRADERS) {
    for (TraderDisplayItem *displayItem : *pTraderDisplayList) {
      pLayout->addWidget(displayItem);
      connect(displayItem, &TraderDisplayItem::clicked, this,
              &ScrollableContainer::listenForEmployeeItemSelection);
    }
  } else {
    for (StockDisplayItem *displayItem : *pStockDisplayList) {
      pLayout->addWidget(displayItem);
      // TODO: stock display click connections
    }
  }
}

//****************************SLOTS*******************************************
void ScrollableContainer::listenForCustomerItemSelection(int id) {
  emit notifyOfCustomerItemSelection(id);
}

void ScrollableContainer::listenForEmployeeItemSelection(int id) {
  emit notifyOfEmployeeItemSelection(id);
}
