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
  QSpinBox *pNumberToSellDisplay;
  QLabel *pSellProfitOrLossDisplay;

  QPushButton *pConfirmSaleButton;
  QPushButton *pCancelSaleButton;

  // Layouts
  QHBoxLayout *pButtonLayout;
  QVBoxLayout *pMainLayout;

public:
  SellStockWindow();
  ~SellStockWindow();

public slots:
  void listenForConfirmSale(); // Connected to the confirm sell button in
                               // SellStockWindow
  void
  listenForCancelSale(); // Connected to the cancel button in SellStockWindow

signals:
  void
  notifyOfConfirmSale(); // Notifies the trader controller that a sale was made
  void notifyOfCancelSale(); // Notifies the trader controller that a sale was
                             // cancelled
};

#endif
