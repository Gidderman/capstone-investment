#include "TraderDisplayItem.h"
#include <qboxlayout.h>

TraderDisplayItem::TraderDisplayItem(QString name, QString id,
                                     QString numAccountsManaged) {
  this->pName = new QLabel(name);
  this->pId = new QLabel(id);
  this->pNumAccountsManaged = new QLabel(numAccountsManaged);

  pNameAndIdLayout = new QVBoxLayout();
  pMainLayout = new QHBoxLayout(this);

  pNameAndIdLayout->addWidget(pName);
  pNameAndIdLayout->addWidget(pId);

  pMainLayout->addLayout(pNameAndIdLayout);
  pMainLayout->addWidget(pNumAccountsManaged);
}

TraderDisplayItem::~TraderDisplayItem() {}
