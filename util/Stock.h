#ifndef STOCK_H
#define STOCK_H

#include <string>

struct Stock {
  int stockID;
  std::string stockName;
  std::string stockCode;
  float stockPrice;
};

#endif
