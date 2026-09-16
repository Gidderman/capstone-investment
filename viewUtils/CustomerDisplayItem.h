#ifndef CUSTOMER_DISPLAY_ITEM_H
#define CUSTOMER_DISPLAY_ITEM_H

#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class CustomerDisplayItem : public QWidget {
  Q_OBJECT

private:
  QLabel *pName;
  QLabel *pId;
  QLabel *pCurrentWorth;
  QLabel *pAvailableFunds;

  QHBoxLayout *pMainLayout;
  QVBoxLayout *pNameAndIdLayout;
  QVBoxLayout *pFundsLayout;

public:
  CustomerDisplayItem(QString name, QString id, QString currentWorth,
                      QString availableFunds);
  ~CustomerDisplayItem();
};

#endif
