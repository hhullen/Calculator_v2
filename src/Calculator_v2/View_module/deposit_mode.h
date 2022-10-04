#ifndef DEPOSIT_MODE_H
#define DEPOSIT_MODE_H

#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QWidget>

#include "replenishment.h"
#include "withdrawal.h"

enum Periodicity { AT_ONCE, EVERY_MONTH, EVERY_YEAR };
enum PaymentsFrequency { MONTHLY, QUARTERLY, ANNUALY };

namespace Ui {
class DepositMode;
}

class DepositMode : public QWidget {
  Q_OBJECT

 public:
  explicit DepositMode(QWidget *parent = nullptr);
  ~DepositMode();
  QWidget *get_widget_pointer();

 signals:
  void add_withdrawal_signal(double deposit, int dd, int mm, int yy);
  void add_replenish_signal(double deposit, int dd, int mm, int yy,
                            int periodicity);
  void set_deposit_fields_data_signal(double deposit, int term,
                                      double interest_rate, double tax_rate,
                                      int pay_frequency, bool capitalization);
  void calculate_deposit_signal(int dd_now, int mm_now, int yy_now,
                                double &result_interest, double &result_tax,
                                double &result_end_deposit);
  void clear_deposit_data_signal();

 private slots:
  void replenish_button();
  void withdrawal_button();
  void check_input_field();
  void get_input_data();
  void remove_widget(QObject *obj);
  void switch_interest_capitalization(int state);

 private:
  Ui::DepositMode *ui_;
  QWidget *deposit_mode_ = new QWidget();
  QGridLayout *replenish_layout_;
  QGridLayout *withdrawal_layout_;
  Replenishment *new_replenish_form_;
  Withdrawal *new_withdrawal_form_;
  QRegularExpression valid_total_;
  QRegularExpression valid_term_;
  QRegularExpression valid_rate_;
  QRegularExpressionMatch matcher_total_;
  QRegularExpressionMatch matcher_term_;
  QRegularExpressionMatch matcher_pec_rate_;
  QRegularExpressionMatch matcher_tax_rate_;

  QWidget *tmp_widget_;
  QList<QLineEdit *> form_amount_;
  QList<QDateEdit *> form_term_;
  QList<QComboBox *> form_switch_;
  double result_interest_;
  double result_tax_;
  double result_end_deposit_;

  void get_deposit_fields_data();
  void get_replenish_form_data();
  void get_withdraw_form_data();
  void output_deposit_calculation_result();

  void setup_regular_expressions();
  void setup_defaul_values();
  void setup_replenish_withdrawal_layout();
  void setup_signal_slot_connections();

  Replenishment *replenish;
};

#endif  // DEPOSIT_MODE_H
