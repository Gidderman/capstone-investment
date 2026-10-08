// This class handles the display for the employee creation/edit window. It also
// handles the input and output for that window, reporting results to the admin
// controller.

#ifndef TRADER_CREATION_VIEW_H
#define TRADER_CREATION_VIEW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class TraderCreationView : public QWidget {
  // Required to utilize signals and slots
  Q_OBJECT

private:
  // Display componenets
  QLabel *pCreateEmployeeTitle;

  QLabel *pFirstNameLabel;
  QLabel *pLastNameLabel;
  QLabel *pRoleSelectionLabel;

  QLineEdit *pFirstNameEntry;
  QLineEdit *pLastNameEntry;
  QComboBox *pRoleSelection;

  QPushButton *pCreateEmployeeButton;
  QPushButton *pCancelCreationButton;

  QPushButton *pUnlockAccountButton;
  QPushButton *pResetPasswordButton;

  QVBoxLayout *layout;

  // This is used to track a employee or indicate that the
  // user is creating a new employee (when it equals -1)
  int id;

  // Clears all inputs
  void clear();

public:
  TraderCreationView();
  ~TraderCreationView();
  // Main entry point into this class
  void run();
  // Overload run function is called when editing an existing employee
  void run(std::vector<QString> employee);
  void end();

  void paintEvent(QPaintEvent *);

public slots:
  void listenForEmployeeCreation();     // Connected to the create employee/Save
                                        // changes button
  void listenForCreationCancellation(); // connected to the cancel button
  void listenForAccountUnlock(); // Connected to the unlock account button
  void listenForPasswordReset(); // Connected to the reset password button

signals:
  void notifyOfEmployeeCreation(
      std::vector<QString> employee); // Lets AdminController know that some
                                      // sort of action has taken place
  void notifyOfCancellation();        // Lets AdminController know that the user
                                      // cancelled an action
  void notifyOfAccountUnlock(int id); // Lets the AdminController know that the
                                      // user is trying to unlock an account
  void notifyOfPasswordReset(int id);
};

#endif
