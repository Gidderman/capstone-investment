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
  AdminService *pAdminService;

  std::vector<Customer> customerList;
  std::vector<Employee> employeeList;

  std::vector<CustomerDisplayItem *> customerDisplayList;
  std::vector<TraderDisplayItem *> employeeDisplayList;

  std::vector<Customer> getAllCustomers();
  std::vector<Employee> getAllEmployees();

  void formatCustomersForDisplay();
  void formatEmployeesForDisplay();

  // TODO: Create a function for refreshing displays. One for each list
public:
  AdminController();
  ~AdminController();
  void run(Authorizer *authorizer);
  void executeEmployeeCreation();
  void executeEmployeeEdit();
  void executeEmployeeDeletion();
  void executeCustomerCreation();
  void executeCustomerEdit();
  void executeCustomerDeletion();

public slots:
  void listenForLogOut();
  void listenForEmployeeCreation();
  void listenForEmployeeEdit();
  void listenForEmployeeDeletion();
  void listenForCustomerCreation();
  void listenForCustomerEdit();
  void listenForCustomerDeletion();

signals:
  void informMasterControllerOfLogOut();
};

#endif
