#include "CustomerDisplayItem.h"

CustomerDisplayItem::CustomerDisplayItem(QString name, QString id,
                                         QString currentWorth,
                                         QString availableFunds) {
  this->pName = new QLabel(name);
  this->pId = new QLabel(id);
  this->pCurrentWorth = new QLabel(currentWorth);
  this->pAvailableFunds = new QLabel(availableFunds);

  pNameAndIdLayout = new QVBoxLayout();
  pFundsLayout = new QVBoxLayout();
  pMainLayout = new QHBoxLayout(this);

  pNameAndIdLayout->addWidget(pName);
  pNameAndIdLayout->addWidget(pId);

  pFundsLayout->addWidget(pCurrentWorth);
  pFundsLayout->addWidget(pAvailableFunds);

  pMainLayout->addLayout(pNameAndIdLayout);
  pMainLayout->addLayout(pFundsLayout);
}

CustomerDisplayItem::~CustomerDisplayItem() {}
