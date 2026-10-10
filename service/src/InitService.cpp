// This class implements InitService.h, see that header file for further
// information.

#include "InitService.h"

#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <stdexcept>

InitService::InitService(DataManager &dataManager)
    : dataManager(dataManager), apiEndpoint("https://finnhub.io/api/v1/quote"),
      networkManager(new QNetworkAccessManager(this)) {
  apiKey =
      readConfigFile("config.json").value("api_key").toString().toStdString();

  trackedStockCodes = {"AAL",  "AMC",  "BA",   "ETN",  "GOOGL", "GRAB", "GS",
                       "INTC", "MCD",  "MDB",  "NOK",  "NU",    "NVDA", "NVTS",
                       "ORCL", "SMCI", "SOFI", "SPCX", "TSLA",  "VRT"};

  outstandingRequests = trackedStockCodes.size();
  errorInfo = "";
}

InitService::~InitService() {}

void InitService::updateStockData() {
  outstandingRequests = trackedStockCodes.size();
  stockData.clear();
  errorInfo = "";

  for (std::string stockCode : trackedStockCodes) {
    runStockQuery(stockCode);
  }
}

void InitService::commitStockUpdates() {
  if (stockData.empty()) {
    errorInfo += "InitService::commitStockUpdates -> No data to write.\n";
  } else if (!dataManager.updateStoredStocks(stockData)) {
    errorInfo +=
        "InitService::commitStockUpdates -> " + dataManager.getErrorInfo();
  }

  if (errorInfo.size() != 0) {
    emit errorOccurred("InitService::commitStockUpdates -> " + errorInfo);
    errorInfo = "";
  }
}

//***************PRIVATE FUNCTIONS***********************************
void InitService::runStockQuery(std::string &stockCode) {
  QUrlQuery query;
  query.addQueryItem("symbol", QString::fromStdString(stockCode));
  query.addQueryItem("token", QString::fromStdString(apiKey));

  QUrl url = QUrl(QString::fromStdString(apiEndpoint));
  url.setQuery(query);

  QNetworkRequest request(url);
  request.setTransferTimeout(10000); // 10 seconds

  QNetworkReply *reply = networkManager->get(request);

  connect(reply, &QNetworkReply::finished, this,
          [this, stockCode, reply]() { handleReply(stockCode, reply); });
}

QJsonObject InitService::readConfigFile(std::string fileName) {
  QFile configFile;

  if (!configFile.exists(QString::fromStdString(fileName))) {
    throw std::logic_error("ERROR: Could not find " + fileName +
                           " file appears not to exist.");
  }

  configFile.setFileName(QString::fromStdString(fileName));

  if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::logic_error("ERROR: Could not open file " + fileName);
  }

  QByteArray unformatedFileContents;
  while (!configFile.atEnd()) {
    unformatedFileContents += configFile.readLine();
  }

  configFile.close();

  QJsonDocument jsonDocument;
  QJsonObject configObject =
      jsonDocument.fromJson(unformatedFileContents).object();

  return configObject;
}

void InitService::handleReply(std::string stockCode, QNetworkReply *reply) {
  const int httpStatus =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  const QNetworkReply::NetworkError error = reply->error();
  const std::string errorText = reply->errorString().toStdString();
  const QByteArray returnInfo = reply->readAll();

  reply->deleteLater();

  if (error != QNetworkReply::NoError) {
    if (httpStatus == 429) {
      errorInfo += "Stock " + stockCode + " hit the rate limit\n";
    } else {
      errorInfo +=
          "Stock " + stockCode + " experienced error " + errorText + "\n";
    }
    outstandingRequests--;

    if (outstandingRequests <= 0) {
      emit refreshFinished();
    }
    return;
  }

  QJsonDocument returnText;
  QJsonObject returnValue = returnText.fromJson(returnInfo).object();

  if (!returnValue.contains("c") || returnValue.value("c").toDouble() == 0) {
    errorInfo += "Stock " + stockCode + " no valid price returned\n";
    outstandingRequests--;

    if (outstandingRequests <= 0) {
      emit refreshFinished();
    }
    return;
  }

  stockData.push_back(
      {-1, "", stockCode, (float)returnValue.value("c").toDouble()});
  outstandingRequests--;

  if (outstandingRequests <= 0) {
    emit refreshFinished();
  }
}
