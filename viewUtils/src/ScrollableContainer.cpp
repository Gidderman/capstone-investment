#include "ScrollableContainer.h"
#include <qboxlayout.h>

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

void ScrollableContainer::refreshDisplayList() {
  if (containerType == CUSTOMERS) {
    for (CustomerDisplayItem *displayItem : *pCustomerDisplayList) {
      pLayout->addWidget(displayItem);
    }
  } else {
    for (TraderDisplayItem *displayItem : *pTraderDisplayList) {
      pLayout->addWidget(displayItem);
    }
  }
}
