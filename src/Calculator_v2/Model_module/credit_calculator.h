#ifndef SRC_CALCULATOR_V2_MODEL_MODULE_CREDIT_CALCULATOR_H
#define SRC_CALCULATOR_V2_MODEL_MODULE_CREDIT_CALCULATOR_H

#include <cmath>

namespace s21 {

enum class CreditMode { ANNUITY, DIFFERENTIATED };

const double kMONEY_ACCURACY = 0.01;

class CreditCalculator {
 public:
  void calculate_credit(double total_in, double term_in, double rate_in,
                        CreditMode mode);
  double get_monthly_payment_high();
  double get_monthly_payment_low();
  double get_overpayment();
  double get_total_payment();

 private:
  double monthly_pay_high_;
  double monthly_pay_low_;
  double overpayment_;
  double total_payment_;
};

}  // namespace s21

#endif  // SRC_CALCULATOR_V2_MODEL_MODULE_CREDIT_CALCULATOR_H
