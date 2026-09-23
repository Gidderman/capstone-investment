// This class implements ScrollableContainer.h, see header file for more info

// TODO:
// THIS CLASS IS NOT YET FINISHED, REQUIRING FORMATING AND SCROLL FUNCTIONALITY.
// Additionally considering turning this into a template class

#include "ScrollableContainer.h"

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
    std::vector<InvestmentDisplayItem *> *displayList) {
  this->pInvestmentDisplayList = displayList;
  refreshDisplayList();
}

void ScrollableContainer::refreshDisplayList() {
  clearDisplay();
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
    for (InvestmentDisplayItem *displayItem : *pInvestmentDisplayList) {
      pLayout->addWidget(displayItem);
      // TODO: stock display click connections
    }
  }
}

//****************************PRIVATE FUNCTIONS*******************************
void ScrollableContainer::clearDisplay() {
  QLayoutItem *item;
  // We always take item 0 because as we remove one, the next one slides into
  // index 0.
  while ((item = pLayout->takeAt(0)) != nullptr) {
    if (item->layout()) {
      delete item->layout();
    } else if (item->widget()) {
      // It's a widget, so we delete it
      delete item->widget();
    }

    // Finally, delete the QLayoutItem wrapper itself
    delete item;
  }
}

//****************************SLOTS*******************************************
// Called when a customer item has been double clicked
void ScrollableContainer::listenForCustomerItemSelection(int id) {
  emit notifyOfCustomerItemSelection(id);
}

// Called when an employee item has been double clicked
void ScrollableContainer::listenForEmployeeItemSelection(int id) {
  emit notifyOfEmployeeItemSelection(id);
}
