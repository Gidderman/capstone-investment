#ifndef INVESTMENT_H
#define INVESTMENT_H

#include "Stock.h"

struct Investment {
  int investmentID;
  Stock stock;
  int numHeld;
  float initialInvestment;
  int customerID;
  float currentInvestmentWorth = numHeld * stock.stockPrice;
};

#endif
