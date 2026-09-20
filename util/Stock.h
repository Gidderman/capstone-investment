// This class is a custom data structure for a given stock. It is typically held
// by an investment, or listed as a potential investment.

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
