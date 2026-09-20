// This class handles the display for the Customer Creation Window. It accepts
// input for the customer information.

// TODO:
// THIS CLASS IS YET INCOMPLETE, REQUIRING MORE FORMATING AND PROPER
// FUNCTIONALITY FOR DISPLAYING CUSTOMER INFORMATION WHEN IT IS OPENED TO EDIT A
// CUSTOMER

#ifndef CUSTOMER_CREATION_VIEW_H
#define CUSTOMER_CREATION_VIEW_H

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class CustomerCreationView : public QWidget {
  // Required macro for using SIGNALS and SLOTS
  Q_OBJECT

private:
  // The componenets to be displayed on screen
  QLineEdit *pFirstNameEntry;
  QLineEdit *pLastNameEntry;
  QLineEdit *pPhoneNumberEntry;
  QLineEdit *pEmailEntry;
  QLineEdit *pInitialInvestmentEntry;
  QComboBox *pAccountTypeEntry;

  QPushButton *pCreateCustomerButton;
  QPushButton *pCancelCreationButton;

  QVBoxLayout *layout;

public:
  CustomerCreationView();
  ~CustomerCreationView();
  // This is the primary entry point for this view
  void run();
  // Called when the view no longer needs to be displayed
  void end();

public slots:
  void listenForCustomerCreation(); // Connected to the Create Customer button
                                    // on CustomerCreationView
  void listenForCreationCancel(); // Connected to the cancel creation button on
                                  // CustomerCreationView

signals:
  void notifyOfCustomerCreation(); // Connected to the AdminController to notify
                                   // that the user has confirmed creation/edit
  void notifyOfCreationCancel();   // Connected to the AdminController to notify
                                   // that the user has cancelled creation
};

#endif
