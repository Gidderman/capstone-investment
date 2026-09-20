// This is a custom data structure that holds the information for a single
// investment. It has an associated stock, but contains some additional
// amplifying information such as the number of that stock held, and what the
// total worth of the investment is.

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
