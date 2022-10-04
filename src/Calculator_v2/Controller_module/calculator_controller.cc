#include "calculator_controller.h"

#include <iostream>

namespace s21 {

CalculatorController::CalculatorController(CalculatorModel *model)
    : model_(model) {}

double CalculatorController::calculate_expression(list<string> *expression,
                                                  double x) {
  model_->calculator.clear_expression();
  for (auto iter = expression->begin(); iter != expression->end(); ++iter) {
    model_->calculator.add_expression_lexem(*iter);
  }

  return model_->calculator.calculate_expression(x);
}

void CalculatorController::calculate_range(std::list<std::string> *expression,
                                           int from, int to,
                                           std::vector<double> &x,
                                           std::vector<double> &y) {
  model_->calculator.clear_expression();
  for (auto iter = expression->begin(); iter != expression->end(); ++iter) {
    model_->calculator.add_expression_lexem(*iter);
  }
  model_->calculator.calculate_range(model_->calculator.get_expression(),
                                     (double)from, (double)to);
  x = model_->calculator.get_x_values_vector();
  y = model_->calculator.get_y_values_vector();
}

void CalculatorController::calculate_credit(
    double total_in, double term_in, double rate_in, double *mon_pay_high,
    double *mon_pay_low, double *overpayment, double *total_payment,
    CreditMode mode) {
  model_->credit.calculate_credit(total_in, term_in, rate_in, mode);
  *mon_pay_high = model_->credit.get_monthly_payment_high();
  *mon_pay_low = model_->credit.get_monthly_payment_low();
  *overpayment = model_->credit.get_overpayment();
  *total_payment = model_->credit.get_total_payment();
}

void CalculatorController::add_withdrawal(double deposit, int dd, int mm,
                                          int yy) {
  model_->deposit.add_withdrawal(deposit, dd, mm, yy);
}

void CalculatorController::add_replenish(double deposit, int dd, int mm, int yy,
                                         ActPeriodicity periodicity) {
  model_->deposit.add_replenish(deposit, dd, mm, yy, periodicity);
}

void CalculatorController::set_deposit_fields_data(double deposit, int term,
                                                   double interest_rate,
                                                   double tax_rate,
                                                   PayFrequency pay_frequency,
                                                   bool capitalization) {
  model_->deposit.set_total_deposit(deposit);
  model_->deposit.set_deposit_term(term);
  model_->deposit.set_interest_rate(interest_rate);
  model_->deposit.set_tax_rate(tax_rate);
  model_->deposit.set_payments_frequency(pay_frequency);
  model_->deposit.set_interest_capitalization(capitalization);
}

void CalculatorController::calculate_deposit(int dd, int mm, int yy,
                                             double &result_interest,
                                             double &result_tax,
                                             double &result_end_deposit) {
  DateCalculator start_date(dd, mm, yy);
  model_->deposit.calculate_deposit(start_date);
  result_interest = model_->deposit.get_accrued_interest();
  result_tax = model_->deposit.get_accrued_tax();
  result_end_deposit = model_->deposit.get_end_deposit();
}

void CalculatorController::clear_deposit_data() { model_->deposit.clear(); }

}  // namespace s21
