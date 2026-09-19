#include "CustomerDisplayItem.h"
#include <qevent.h>
#include <qnamespace.h>

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

  this->id = id.toInt();
}

CustomerDisplayItem::~CustomerDisplayItem() {}

void CustomerDisplayItem::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    // TODO: highlight this widget
  }
}

void CustomerDisplayItem::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    emit clicked(this->id);
  }
}
