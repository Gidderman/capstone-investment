#include "CRUDManager.h"

#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QSqlError>

#include <iostream>

CRUDManager::CRUDManager() {
  std::cout << "CRUDManager::CRUDManager - entering/exiting" << std::endl;
  init();
}

CRUDManager::~CRUDManager() {}

bool CRUDManager::init() {
  std::cout << "CRUDManager::init - entering" << std::endl;

  std::optional<QJsonObject> databaseLogInInfo = readJsonData("config.json");

  if (!databaseLogInInfo.has_value()) {
    return false;
  }

  db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName(databaseLogInInfo.value().value("host").toString());
  db.setDatabaseName(
      databaseLogInInfo.value().value("database_name").toString());
  db.setUserName(
      databaseLogInInfo.value().value("database_username").toString());
  db.setPassword(
      databaseLogInInfo.value().value("database_password").toString());

  std::cout << "CRUDManager::init - exiting" << std::endl;

  if (!db.open()) {
    errorCode += "ERROR: could not open database. Database reports: \n" +
                 db.lastError().text().toStdString() + "\n";

    std::cout << errorCode << std::endl;

    return false;
  }
  return true;
}

std::optional<QSqlQuery>
CRUDManager::runQuery(QString queryText,
                      std::unordered_map<QString, QVariant> queryArgs) {
  std::cout << "CRUDManager::runQuery - entering" << std::endl;

  QSqlQuery query;
  query.prepare(queryText);
  for (auto &qArg : queryArgs) {
    query.bindValue(qArg.first, qArg.second);
  }

  std::cout << "CRUDManager::runQuery - exiting" << std::endl;

  if (!query.exec()) {
    errorCode +=
        "ERROR: Could not complete database query. Database reports: \n" +
        query.lastError().text().toStdString() + "\n";
    return {};
  }
  return query;
}

std::string CRUDManager::getErrorCode() {
  if (errorCode.size() != 0) {
    return errorCode;
  }
  return "No errors";
}

//********************PRIVATE FUNCITONS**************************************

std::optional<QJsonObject> CRUDManager::readJsonData(QString fileName) {
  std::cout << "CRUDManager::readJsonData - entering" << std::endl;

  QFile configFile;

  if (!configFile.exists(fileName)) {
    errorCode += "ERROR: Could not find " + fileName.toStdString() +
                 " file appears not to exist.\n";

    std::cout << errorCode << std::endl;

    return {};
  }

  configFile.setFileName(fileName);

  if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    errorCode += "ERROR: Could not open file " + fileName.toStdString() + "\n";

    std::cout << errorCode << std::endl;

    return {};
  }

  QByteArray unformatedFileContents;
  while (!configFile.atEnd()) {
    unformatedFileContents += configFile.readLine();
  }

  configFile.close();

  QJsonDocument jsonDocument;
  QJsonObject configObject =
      jsonDocument.fromJson(unformatedFileContents).object();

  std::cout << "CRUDManager::readJsonData - exiting" << std::endl;

  return configObject;
}
