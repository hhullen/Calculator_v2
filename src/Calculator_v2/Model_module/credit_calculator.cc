#include "credit_calculator.h"

namespace s21 {

void CreditCalculator::calculate_credit(double total_in, double term_in,
                                        double rate_in, CreditMode mode) {
  rate_in /= 100;
  if (mode == CreditMode::ANNUITY) {
    rate_in /= 12;
    monthly_pay_high_ =
        total_in * (rate_in + (rate_in / (pow(1 + rate_in, term_in) - 1)));
    monthly_pay_low_ = monthly_pay_high_;
    total_payment_ = monthly_pay_high_ * term_in;
  } else if (mode == CreditMode::DIFFERENTIATED) {
    double main_debt = 0.0;
    main_debt = total_in / term_in;
    total_payment_ = 0;

    monthly_pay_high_ = main_debt + total_in * rate_in / 12;
    monthly_pay_low_ =
        main_debt + (total_in - main_debt * (term_in - 1)) * rate_in / 12;
    for (int i = 0; i < term_in; i++) {
      total_payment_ += main_debt + (total_in - main_debt * i) * rate_in / 12;
    }
  }
  overpayment_ = total_payment_ - total_in;
}

double CreditCalculator::get_monthly_payment_high() {
  return monthly_pay_high_;
}

double CreditCalculator::get_monthly_payment_low() { return monthly_pay_low_; }

double CreditCalculator::get_overpayment() { return overpayment_; }

double CreditCalculator::get_total_payment() { return total_payment_; }

}  // namespace s21
