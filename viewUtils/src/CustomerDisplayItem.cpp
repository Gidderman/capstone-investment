// This class implements CustomerDisplayItem.h, see header file for more info

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING

#include "CustomerDisplayItem.h"

CustomerDisplayItem::CustomerDisplayItem(QString name, QString id,
                                         QString currentWorth,
                                         QString availableFunds) {
  // Initialize the display variables
  this->pName = new QLabel(name);
  this->pId = new QLabel(id);
  this->pCurrentWorth = new QLabel(currentWorth);
  this->pAvailableFunds = new QLabel(availableFunds);

  // Set up the layout, it has two columns:
  // Left column from top to bottom:
  //   Customer full name
  //   Customer ID
  // Right column from top to bottom:
  //   Current customer worth
  //   Available funds for investment
  pNameAndIdLayout = new QVBoxLayout();
  pFundsLayout = new QVBoxLayout();
  pMainLayout = new QHBoxLayout(this);

  pNameAndIdLayout->addWidget(pName);
  pNameAndIdLayout->addWidget(pId);

  pFundsLayout->addWidget(pCurrentWorth);
  pFundsLayout->addWidget(pAvailableFunds);

  pMainLayout->addLayout(pNameAndIdLayout);
  pMainLayout->addLayout(pFundsLayout);

  // Write the passed in id to the member variable
  this->id = id.toInt();
}

CustomerDisplayItem::~CustomerDisplayItem() {}

void CustomerDisplayItem::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    // TODO: highlight this widget
  }
}

// When the item is double clicked, tell the scrollable container
void CustomerDisplayItem::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    emit clicked(this->id);
  }
}
