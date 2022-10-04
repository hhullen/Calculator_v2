#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QMainWindow>
#include <QPropertyAnimation>
#include <QTimer>

#include "Controller_module/calculator_controller.h"
#include "calculator_mode.h"
#include "credit_mode.h"
#include "deposit_mode.h"
using s21::CalculatorController;
using s21::CalculatorModel;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(CalculatorController *controller, QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void update_time();
  void open_up_window();
  void main_menu(int index);
  double calculate_expression(list<std::string> *expression, double x);
  void calculate_range(list<std::string> *expression, int from, int to,
                       vector<double> &x, vector<double> &y);
  void calculate_credit(double total_in, double term_in, double rate_in,
                        double *mon_pay_high, double *mon_pay_low,
                        double *overpayment, double *total_payment, int mode);
  void add_withdrawal(double deposit, int dd, int mm, int yy);
  void add_replenish(double deposit, int dd, int mm, int yy, int periodicity);
  void set_deposit_fields_data(double deposit, int term, double interest_rate,
                               double tax_rate, int pay_frequency,
                               bool capitalization);
  void calculate_deposit(int dd, int mm, int yy, double &result_interest,
                         double &result_tax, double &result_end_deposit);
  void clear_deposit_data();

 private:
  Ui::MainWindow *ui_;
  CalculatorController *controller_;

  CalculatorMode *p_calculator_mode_;
  CreditMode *p_credit_mode_;
  DepositMode *p_deposit_mode_;
  QWidget *p_calculator_widget_;
  QWidget *p_credit_widget_;
  QWidget *p_deposit_widget_;

  QPropertyAnimation *p_calc_animation_;
  QPropertyAnimation *p_credit_animation_;
  QPropertyAnimation *p_deposit_animation_;
  int window_width_;
  int calculator_x_pos_;
  int credit_x_pos_;
  int deposit_x_pos_;
  QPoint zero_pos_;
  void animate_widget(QPropertyAnimation *widget, int *current, int target);

  QTimer *p_timer_;
  QDateTime *p_date_time_;

  int mouse_old_x_;
  int mouse_old_y_;
  int dx_;
  int dy_;
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

  void set_default_field_values();
  void setup_widgets();
  void setup_time_date();
  void setup_signal_slot_connections();
};
#endif  // MAINWINDOW_H
