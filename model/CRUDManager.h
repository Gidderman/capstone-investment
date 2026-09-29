// The CRUDManager is the point of contact with the database. It handles all
// database interfacing and queries, utilizing Qt::SQL to perform those actions.

#ifndef CRUD_MANAGER_H
#define CRUD_MANAGER_H

#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <optional>
#include <unordered_map>

class CRUDManager {
private:
  QSqlDatabase db;

  // This class has lots of operations that are prone to failure due to outside
  // influences (such as network issues) As such, the runQuery function is
  // optional, and if something fails then it will return nothing and the
  // calling function can get the error code and pass that up inform the user of
  // the issue.
  std::string errorCode;

  // This is used to access our configuration file for database accessing
  std::optional<QJsonObject> readJsonData(QString fileName);

public:
  CRUDManager();
  ~CRUDManager();
  // Called to initialize the database
  bool init();
  // This is the main function of the CRUDManager. It accepts a QString as the
  // MySQL query with a unordered_map used to bind the arguments.
  std::optional<QSqlQuery>
  runQuery(QString queryText, std::unordered_map<QString, QVariant> queryArgs);
  // This function returns any error codes encountered during the runQuery
  // operation.
  std::string getErrorCode();
};

#endif
