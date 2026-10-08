// This class implements CustomerDisplayItem.h, see header file for more info

#include "CustomerDisplayItem.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <qboxlayout.h>
#include <qnamespace.h>

CustomerDisplayItem::CustomerDisplayItem(QString name, QString id,
                                         QString currentWorth,
                                         QString availableFunds) {

  this->setObjectName("DisplayItem");

  pNameLabel = new QLabel("Name");
  pNameLabel->setObjectName("DisplayItemLabel");
  pIdLabel = new QLabel("ID Number");
  pIdLabel->setObjectName("DisplayItemLabel");
  pCurrentWorthLabel = new QLabel("Current Worth ($)");
  pCurrentWorthLabel->setObjectName("DisplayItemLabel");
  pAvailableFundsLabel = new QLabel("Funds Available ($)");
  pAvailableFundsLabel->setObjectName("DisplayItemLabel");

  // Initialize the display variables
  this->pName = new QLabel(name);
  pName->setObjectName("DisplayItemContent");

  // Append zeros to the displayed id
  if (id.size() < 8) {
    for (unsigned int i = 0; i < 8 - id.size(); i++) {
      id = "0" + id;
    }
  }

  this->pId = new QLabel(id);
  pId->setObjectName("DisplayItemContent");
  this->pCurrentWorth = new QLabel(currentWorth);
  pCurrentWorth->setObjectName("DisplayItemContent");
  this->pAvailableFunds = new QLabel(availableFunds);
  pAvailableFunds->setObjectName("DisplayItemContent");

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

  pNameLayout = new QVBoxLayout();
  pNameLayout->addWidget(pNameLabel, 0, Qt::AlignLeft);
  pNameLayout->addWidget(pName, 0, Qt::AlignLeft);

  pIdLayout = new QVBoxLayout();
  pIdLayout->addWidget(pIdLabel, 0, Qt::AlignLeft);
  pIdLayout->addWidget(pId, 0, Qt::AlignLeft);

  pCurrentWorthLayout = new QVBoxLayout();
  pCurrentWorthLayout->addWidget(pCurrentWorthLabel, 0, Qt::AlignLeft);
  pCurrentWorthLayout->addWidget(pCurrentWorth, 0, Qt::AlignLeft);

  pAvailableFundsLayout = new QVBoxLayout();
  pAvailableFundsLayout->addWidget(pAvailableFundsLabel, 0, Qt::AlignLeft);
  pAvailableFundsLayout->addWidget(pAvailableFunds, 0, Qt::AlignLeft);

  pNameAndIdLayout->addLayout(pNameLayout);
  pNameAndIdLayout->addLayout(pIdLayout);

  pFundsLayout->addLayout(pCurrentWorthLayout);
  pFundsLayout->addLayout(pAvailableFundsLayout);

  pMainLayout->addLayout(pNameAndIdLayout);
  pMainLayout->addLayout(pFundsLayout);

  // Write the passed in id to the member variable
  this->id = id.toInt();
}

CustomerDisplayItem::~CustomerDisplayItem() {}

// This is necessarcy to use style sheets.
void CustomerDisplayItem::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CustomerDisplayItem::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    emit clicked(this->id);
  }
}

// When the item is double clicked, tell the scrollable container
void CustomerDisplayItem::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    emit doubleClicked(this->id);
  }
}
