// This class handles the display of the Sell Stock pop up window, as well as
// any input made, passing said input to the TraderController.

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING AND INFORMATION
// POPULATION FOR THE AVAILABLE STOCKS TO SELL

#ifndef SELL_STOCK_WINDOW_H
#define SELL_STOCK_WINDOW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class SellStockWindow : public QWidget {
  // This macro is requried to utilize slots and signals
  Q_OBJECT

private:
  // Display items
  QComboBox *pSelectedStockDisplay;
  QComboBox *pSelectedStockCodeDisplay;
  QSpinBox *pNumberToSellDisplay;
  QLabel *pInitialInvestmentDisplay;
  QLabel *pCurrentWorthDisplay;
  QLabel *pSellProfitOrLossDisplay;

  QPushButton *pConfirmSaleButton;
  QPushButton *pCancelSaleButton;

  // Layouts
  QHBoxLayout *pButtonLayout;
  QVBoxLayout *pMainLayout;

public:
  SellStockWindow(
      std::tuple<std::vector<QString>, std::vector<QString>> stockChoices);
  ~SellStockWindow();
  void run();
  void setDisplayInfo(std::vector<QString> displayData,
                      int maxNumberAllowedToSale);

public slots:
  void listenForConfirmSale(); // Connected to the confirm sell button in
                               // SellStockWindow
  void
  listenForCancelSale(); // Connected to the cancel button in SellStockWindow
  void listenForNumOfStockChange(int number); // Connected to the num of stocks
                                              // to sell spin box

  // The following two slots are used to match a stock code to the stock name
  // and vice versa following the user changing a selection.
  void matchStockCodeAfterChange(int index);
  void matchStockNameAfterChange(int index);

signals:
  void notifyOfConfirmSale(
      std::tuple<QString, int, QString>
          transaction); // Notifies the trader controller that a sale was made
  void notifyOfCancelSale(); // Notifies the trader controller that a sale was
                             // cancelled
  void notifyOfSaleCalculation(std::tuple<QString, int> stockAndNumber);
};

#endif
