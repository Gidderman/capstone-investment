// This class is a way to display information within the ScrollableContainer.
// Each instance of the class is a singular Trader that needs to be displayed
// in a specific format.

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING

#ifndef TRADER_DISPLAY_ITEM_H
#define TRADER_DISPLAY_ITEM_H

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <qboxlayout.h>
#include <qevent.h>

class TraderDisplayItem : public QWidget {
  // Requried to use signals and slots
  Q_OBJECT

private:
  // Display information
  QLabel *pName;
  QLabel *pId;
  QLabel *pNumAccountsManaged;

  QHBoxLayout *pMainLayout;
  QVBoxLayout *pNameAndIdLayout;

  // This variable is used to be able to tie the display to the
  // customer data within the controller
  int id;

protected:
  // Overrided functions to make this widget clickable.
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;

public:
  // Pass in the information to display with the constructor
  TraderDisplayItem(QString name, QString id, QString numAccountsManaged);
  ~TraderDisplayItem();

signals:
  void clicked(int id); // Connected to the scrollable container.
};

#endif
