#ifndef SRC_CALCULATOR_V2_CONTROLLER_MODULE_CALCULATOR_CONTROLLER_H_
#define SRC_CALCULATOR_V2_CONTROLLER_MODULE_CALCULATOR_CONTROLLER_H_

#include <list>

#include "../Model_module/calculator_model.h"
using std::list;
using std::string;

namespace s21 {

class CalculatorController {
 public:
  CalculatorController(CalculatorModel *model);
  double calculate_expression(list<string> *expression, double x);
  void calculate_range(std::list<std::string> *expression, int from, int to,
                       std::vector<double> &x, std::vector<double> &y);

  void calculate_credit(double total_in, double term_in, double rate_in,
                        double *mon_pay_high, double *mon_pay_low,
                        double *overpayment, double *total_payment,
                        CreditMode mode);

  void add_withdrawal(double deposit, int dd, int mm, int yy);
  void add_replenish(double deposit, int dd, int mm, int yy,
                     ActPeriodicity periodicity);
  void set_deposit_fields_data(double deposit, int term, double interest_rate,
                               double tax_rate, PayFrequency pay_frequency,
                               bool capitalization);
  void calculate_deposit(int dd, int mm, int yy, double &result_interest,
                         double &result_tax, double &result_end_deposit);
  void clear_deposit_data();

 private:
  CalculatorModel *model_;
};

}  // namespace s21

#endif  // SRC_CALCULATOR_V2_CONTROLLER_MODULE_CALCULATOR_CONTROLLER_H_
