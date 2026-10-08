// This class implements TraderDisplayItem.h, see header file for more info

#include "TraderDisplayItem.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <qboxlayout.h>
#include <qnamespace.h>

TraderDisplayItem::TraderDisplayItem(QString name, QString id,
                                     QString numAccountsManaged) {
  this->setObjectName("DisplayItem");

  // Initialize the display variables
  pNameLabel = new QLabel("Employee Name");
  pNameLabel->setObjectName("DisplayItemLabel");

  pIdLabel = new QLabel("Employee ID");
  pIdLabel->setObjectName("DisplayItemLabel");

  pNumAccountsManagedLabel = new QLabel("Number of Accounts Managed");
  pNumAccountsManagedLabel->setObjectName("DisplayItemLabel");

  this->pName = new QLabel(name);
  pName->setObjectName("DisplayItemContent");

  if (id.size() < 8) {
    for (unsigned int i = 0; i < 8 - id.size(); i++) {
      id = "0" + id;
    }
  }

  this->pId = new QLabel(id);
  pId->setObjectName("DisplayItemContent");

  this->pNumAccountsManaged = new QLabel(numAccountsManaged);
  pNumAccountsManaged->setObjectName("DisplayItemContent");

  // Write the passed in id to the member variable
  this->id = id.toInt();

  // Set up the layout, it has two columns:
  // Left column from top to bottom:
  //   Employee full name
  //   Employee ID
  // Right column from top to bottom:
  //   Number of accounts managed
  pNameLayout = new QVBoxLayout();
  pNameLayout->addWidget(pNameLabel, 0, Qt::AlignLeft);
  pNameLayout->addWidget(pName, 0, Qt::AlignLeft);

  pIdLayout = new QVBoxLayout();
  pIdLayout->addWidget(pIdLabel, 0, Qt::AlignLeft);
  pIdLayout->addWidget(pId, 0, Qt::AlignLeft);

  pNumAccountsManagedLayout = new QVBoxLayout();
  pNumAccountsManagedLayout->addWidget(pNumAccountsManagedLabel, 0,
                                       Qt::AlignLeft);
  pNumAccountsManagedLayout->addWidget(pNumAccountsManaged, 0, Qt::AlignLeft);
  pNumAccountsManagedLayout->addStretch(1);

  pMainLayout = new QHBoxLayout(this);
  pNameAndIdLayout = new QVBoxLayout();

  pNameAndIdLayout->addLayout(pNameLayout);
  pNameAndIdLayout->addLayout(pIdLayout);

  pMainLayout->addLayout(pNameAndIdLayout);
  pMainLayout->addLayout(pNumAccountsManagedLayout);
}

TraderDisplayItem::~TraderDisplayItem() {}

// This is necessarcy to use style sheets.
void TraderDisplayItem::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TraderDisplayItem::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    emit clicked(this->id);
  }
}

// When the item is double clicked, tell the scrollable container
void TraderDisplayItem::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    emit doubleClicked(this->id);
  }
}
