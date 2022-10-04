#ifndef CREDIT_MODE_H
#define CREDIT_MODE_H

#include <QRegularExpression>
#include <QWidget>

#include "ui_credit_mode.h"

enum CreditType { ANNUITY, DIFFERENTIATED };

namespace Ui {
class CreditMode;
}

class CreditMode : public QWidget {
  Q_OBJECT

 public:
  explicit CreditMode(QWidget *parent = nullptr);
  ~CreditMode();
  QWidget *get_widget_pointer();

 signals:
  void calculate_credit_singal(double total_in, double term_in, double rate_in,
                               double *mon_pay_high, double *mon_pay_low,
                               double *overpayment, double *total_payment,
                               int mode);

 private slots:
  void check_input_field();
  void get_input_data();

 private:
  Ui::CreditMode *ui_;
  QWidget *credit_mode_ = new QWidget();
  void write_values_to_fields(double mon_pay_high, double mon_pay_low,
                              double overpayment, double total_payment,
                              int mode);

  QRegularExpression valid_total_;
  QRegularExpression valid_term_;
  QRegularExpression valid_rate_;
  QRegularExpressionMatch matcher_total_;
  QRegularExpressionMatch matcher_term_;
  QRegularExpressionMatch matcher_rate_;
  double credit_total_;
  double credit_term_;
  double interest_rate_;
  double mon_pay_high_;
  double mon_pay_low_;
  double overpayment_;
  double total_payment_;
  QString buffer_;

  void setup_regular_expressions();
  void setup_defaul_values();
  void setup_signal_slot_connections();
};

#endif  // CREDIT_MODE_H
