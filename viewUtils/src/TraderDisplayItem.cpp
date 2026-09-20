// This class implements TraderDisplayItem.h, see header file for more info

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING

#include "TraderDisplayItem.h"
#include <qboxlayout.h>
#include <qevent.h>

TraderDisplayItem::TraderDisplayItem(QString name, QString id,
                                     QString numAccountsManaged) {
  // Initialize the display variables
  this->pName = new QLabel(name);
  this->pId = new QLabel(id);
  this->pNumAccountsManaged = new QLabel(numAccountsManaged);

  // Write the passed in id to the member variable
  this->id = id.toInt();

  // Set up the layout, it has two columns:
  // Left column from top to bottom:
  //   Employee full name
  //   Employee ID
  // Right column from top to bottom:
  //   Number of accounts managed
  pNameAndIdLayout = new QVBoxLayout();
  pMainLayout = new QHBoxLayout(this);

  pNameAndIdLayout->addWidget(pName);
  pNameAndIdLayout->addWidget(pId);

  pMainLayout->addLayout(pNameAndIdLayout);
  pMainLayout->addWidget(pNumAccountsManaged);
}

TraderDisplayItem::~TraderDisplayItem() {}

void TraderDisplayItem::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    // TODO: Change widget color
  }
}

// When the item is double clicked, tell the scrollable container
void TraderDisplayItem::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    emit clicked(this->id);
  }
}
