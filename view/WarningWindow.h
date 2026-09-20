// This class is a warning window that is displayed from the admin screen
// whenever a user attempts to delete a customer or trader, giving them a chance
// to cancel that action.

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
