#ifndef WARNING_WINDOW_H
#define WARNING_WINDOW_H

#include <QMessageBox>
#include <QString>

class WarningWindow : public QMessageBox {
private:
  QString warningText;

public:
  WarningWindow();
  ~WarningWindow();
};

#endif
