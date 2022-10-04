#ifndef SRC_CALCULATOR_V2_MODEL_MODULE_DEPOSIT_CALCULATOR_H
#define SRC_CALCULATOR_V2_MODEL_MODULE_DEPOSIT_CALCULATOR_H

#include <cmath>

#include "date_calculator.h"

using std::round;

namespace s21 {

const double kCB_KEY_RATE = 0.075;

enum class PayFrequency { MONTHLY, QUARTERLY, ANNUALY };

enum class ActType { REPLENISH, WITHDRAW };

enum class ActPeriodicity { AT_ONCE, EVERY_MONTH, EVERY_YEAR };

class DepositCalculator {
 public:
  DepositCalculator();
  ~DepositCalculator();
  void calculate_deposit(const DateCalculator &start);
  void clear();
  void add_replenish(double deposit, int dd, int mm, int yy,
                     ActPeriodicity periodicity);
  void add_withdrawal(double deposit, int dd, int mm, int yy);

  void set_total_deposit(double value);
  void set_deposit_term(int months);
  void set_interest_rate(double unit_fraction);
  void set_tax_rate(double unit_fraction);
  void set_payments_frequency(PayFrequency value);
  void set_interest_capitalization(bool value);

  double get_accrued_interest();
  double get_accrued_tax();
  double get_end_deposit();

  bool is_actList_empty();

 private:
  double deposit_total_;
  int deposit_term_;
  double interest_rate_;
  double tax_rate_;
  PayFrequency payments_frequency_;
  bool interest_capitalization_;

  double accrued_interest_;
  double accrued_tax_;
  double end_deposit_;

  DateCalculator start_, current_, end_;

  class ActList {
   public:
    ActType act_type;
    ActPeriodicity periodicity;
    double amount;
    DateCalculator date;
    ActList *next;

    ActList(ActType act_type, ActPeriodicity periodicity, double amount, int dd,
            int mm, int yy, ActList *next)
        : act_type(act_type),
          periodicity(periodicity),
          amount(amount),
          next(next) {
      date.set_date(dd, mm, yy);
    }
  };
  ActList *deposit_action_;

  void pop_action();
  void periodic_payment(double *deposit);
  void extra_action(double *deposit);
  void apply_replenishment(ActList *action, double *deposit);
  void apply_withdrawal(ActList *action, double *deposit);
  bool is_any_quarter();
  void calculate_one_periodic_payment(double *deposit, const short int divider);
  int set_year_type(int year);
  double accrue_tax(DateCalculator &date);
};

}  // namespace s21

#endif  // SRC_CALCULATOR_V2_MODEL_MODULE_DEPOSIT_CALCULATOR_H
