#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include "AdminMainView.h"
#include "AdminService.h"
#include "Authorizer.h"
#include "CustomerCreationView.h"
#include "TraderCreationView.h"

#include <QObject>

class AdminController : public QObject {
  Q_OBJECT

private:
  AdminMainView *pAdminMainView;
  CustomerCreationView *pCustomerCreationView;
  TraderCreationView *pTraderCreationView;

  WarningWindow *pDeleteWarning;

  AdminService adminService;

  std::vector<Customer> customerList;
  std::vector<Employee> employeeList;

  std::vector<CustomerDisplayItem *> customerDisplayList;
  std::vector<TraderDisplayItem *> employeeDisplayList;

  std::vector<Customer> getAllCustomers();
  std::vector<Employee> getAllEmployees();

  void formatCustomersForDisplay();
  void formatEmployeesForDisplay();

  std::vector<QString> formatIndividualEmployeeForDisplay(Employee employee);

  // TODO: Create a function for refreshing displays. One for each list
public:
  AdminController();
  ~AdminController();
  void run(Authorizer *authorizer);
  void executeEmployeeCreation();
  void executeEmployeeEdit(Employee employeeToBeEdited);
  void executeEmployeeDeletion();
  void executeCustomerCreation();
  void executeCustomerEdit();
  void executeCustomerDeletion();

  void executeEmployeeAction(Employee employee);
  void executeEmployeeAccountUnlock();
  void cancelEmployeeAction();

  void executeCustomerAction();
  void cancelCustomerAction();

public slots:
  void listenForLogOut();
  void listenForEmployeeCreation();
  void listenForEmployeeEdit(int id);
  void listenForEmployeeDeletion();
  void listenForCustomerCreation();
  void listenForCustomerEdit();
  void listenForCustomerDeletion();

  void listenForEmployeeActionConfirmation(std::vector<QString> employee);
  void listenForEmployeeActionCancel();
  void listenForEmployeeAccountUnlock();

  void listenForCustomerActionConfirmation();
  void listenForCustomerActionCancel();

signals:
  void informMasterControllerOfLogOut();
};

#endif
