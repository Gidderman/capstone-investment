// This class is a way to display information within the ScrollableContainer.
// Each instance of the class is a singular Customer that needs to be displayed
// in a specific format.

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING

#ifndef CUSTOMER_DISPLAY_ITEM_H
#define CUSTOMER_DISPLAY_ITEM_H

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <qevent.h>

class CustomerDisplayItem : public QWidget {
  // Requried to use signals and slots
  Q_OBJECT

private:
  // Display information
  QLabel *pName;
  QLabel *pId;
  QLabel *pCurrentWorth;
  QLabel *pAvailableFunds;

  QHBoxLayout *pMainLayout;
  QVBoxLayout *pNameAndIdLayout;
  QVBoxLayout *pFundsLayout;

  // This variable is used to be able to tie the display to the
  // customer data within the controller
  int id;

protected:
  // Overrided functions to make this widget clickable.
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;

public:
  // Pass in the information to display with the constructor
  CustomerDisplayItem(QString name, QString id, QString currentWorth,
                      QString availableFunds);
  ~CustomerDisplayItem();

signals:
  void clicked(int id); // Connected to the scrollable container.
};

#endif
