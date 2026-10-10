// This class handles the various tasks necessary upon initiation of the
// application, primarily connecting to the Stock API and pulling up to date
// stock information into the database.

#ifndef INIT_SERVICE_H
#define INIT_SERVICE_H

#include "DataManager.h"

#include <QNetworkAccessManager>

class InitService : public QObject {
  Q_OBJECT

private:
  DataManager &dataManager;

  std::string apiEndpoint;
  std::string apiKey;
  QNetworkAccessManager *networkManager;

  std::string errorInfo;
  int outstandingRequests;

  std::vector<std::string> trackedStockCodes;
  std::vector<Stock> stockData; // Used to accrue all stock updates

  void runStockQuery(std::string &stockCode); // runs the stock query
  QJsonObject readConfigFile(std::string fileName);
  void handleReply(std::string stockCode, QNetworkReply *reply);

public:
  InitService(DataManager &dataManager);
  ~InitService();
  void updateStockData();
  void commitStockUpdates();

signals:
  void updateStockPrice(std::string stockCode, float stockPrice);
  void notifyOfError(std::string stockCode, std::string error);
  void refreshFinished();
  void errorOccurred(std::string errorInfo);
};

#endif
