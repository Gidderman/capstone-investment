// This class handles the display of the pop up window for purchasing stocks.
// It handls the input necessary for that function as well, passing information
// to the TraderController.

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATING AND
// FUNCTIONALITY FOR SEEING AVAILABLE STOCKS TO BUY

#ifndef PURCHASE_STOCK_WINDOW_H
#define PURCHASE_STOCK_WINDOW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class PurchaseStockWindow : public QWidget {
  // Requred macro for using signals and slots
  Q_OBJECT

private:
  // Display items
  QComboBox *pSelectedStockDisplay;
  QComboBox *pSelectedStockCodeDisplay;
  QSpinBox *pNumberOfStockToPurchaseDisplay;
  QLabel *pTotalPriceOfPurchase;

  QPushButton *pConfirmPurchaseButton;
  QPushButton *pCancelPurchaseButton;

  // Layouts
  QHBoxLayout *pButtonLayout;
  QVBoxLayout *pMainLayout;

public:
  PurchaseStockWindow(
      std::vector<std::tuple<QString, QString, QString>> stockList);
  ~PurchaseStockWindow();
  void run();
  void setDisplayPrice(QString price);

public slots:
  void listenForConfirmPurchase(); // Connected to the confirm purchase button
  void listenForCancelPurchase();  // Connected to the cancel puchase button
  void
  listenForTotalPriceChange(); // Connected to spin box, as well as the stock
                               // code for calculating the price of the purchase

  void matchStockCodeAfterChange(int index);
  void matchStockNameAfterChange(int index);

signals:
  void notifyOfConfirmPurchase(); // Lets the TraderController know that the
                                  // user has confirmed a purchase
  void notifyOfCancelPurchase(); // Lets the TraderController know that the user
                                 // has cancelled a purchase
  void notifyOfPriceCalculation(
      std::tuple<QString, int>
          stockAndNumber); // Used to let the controller know that
                           // new display price needs to be calculated
};

#endif
