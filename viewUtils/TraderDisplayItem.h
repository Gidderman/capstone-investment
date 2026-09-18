#ifndef TRADER_DISPLAY_ITEM_H
#define TRADER_DISPLAY_ITEM_H

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <qboxlayout.h>
#include <qevent.h>

class TraderDisplayItem : public QWidget {
  Q_OBJECT

private:
  QLabel *pName;
  QLabel *pId;
  QLabel *pNumAccountsManaged;

  QHBoxLayout *pMainLayout;
  QVBoxLayout *pNameAndIdLayout;

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;

public:
  TraderDisplayItem(QString name, QString id, QString numAccountsManaged);
  ~TraderDisplayItem();

signals:
  void clicked();
};

#endif
