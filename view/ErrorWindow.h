// This class is an error window that is displayed to communicate
// error messages to the user.

#ifndef ERROR_WINDOW_H
#define ERROR_WINDOW_H

#include <QMessageBox>
#include <QString>

class ErrorWindow : public QMessageBox {
private:
public:
  ErrorWindow(QString errorMessage);
  ~ErrorWindow();
};

#endif
