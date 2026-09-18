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
  Q_OBJECT

private:
  QLabel *pName;
  QLabel *pId;
  QLabel *pCurrentWorth;
  QLabel *pAvailableFunds;

  QHBoxLayout *pMainLayout;
  QVBoxLayout *pNameAndIdLayout;
  QVBoxLayout *pFundsLayout;

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;

public:
  CustomerDisplayItem(QString name, QString id, QString currentWorth,
                      QString availableFunds);
  ~CustomerDisplayItem();

signals:
  void clicked();
};

#endif
