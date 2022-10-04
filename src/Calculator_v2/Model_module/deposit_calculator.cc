#include "deposit_calculator.h"

namespace s21 {

DepositCalculator::DepositCalculator() {
  deposit_action_ = nullptr;
  clear();
}

DepositCalculator::~DepositCalculator() { clear(); }

void DepositCalculator::calculate_deposit(const DateCalculator &start) {
  double current_deposit = 0.0, one_end_tax = 0.0;
  int year_type = 365;

  start_ = start;
  end_ = start_;
  current_ = start_;
  end_.add_to_date(0, deposit_term_, 0);

  while (current_.compare(end_, CompareMode::FULL_CMP) ==
         CompareResult::DATE_LT) {
    one_end_tax += accrue_tax(current_);

    year_type = set_year_type(current_.get_year());
    current_deposit += deposit_total_ * interest_rate_ / year_type;

    if (interest_capitalization_) {
      periodic_payment(&current_deposit);
    }
    if (!is_actList_empty()) {
      extra_action(&current_deposit);
    }
    ++current_;
  }

  end_deposit_ = current_deposit + deposit_total_;
  accrued_tax_ = one_end_tax;
  accrued_interest_ = end_deposit_ / deposit_total_ - 1;
}

double DepositCalculator::accrue_tax(DateCalculator &date) {
  double returnable = 0.0, non_tax = deposit_total_ * kCB_KEY_RATE;
  DateCalculator tax_date(1, 12, 0);

  if (date.compare(tax_date, CompareMode::MONTH_CMP) ==
      CompareResult::DATE_EQ) {
    returnable = (deposit_total_ * interest_rate_ - non_tax) * tax_rate_;
    if (returnable < 0) {
      returnable = 0;
    }
  }
  return returnable;
}

void DepositCalculator::extra_action(double *deposit) {
  ActList *current_action = deposit_action_;

  while (current_action) {
    if (current_action->act_type == ActType::REPLENISH) {
      apply_replenishment(current_action, deposit);
    } else if (current_action->act_type == ActType::WITHDRAW) {
      apply_withdrawal(current_action, deposit);
    }
    current_action = current_action->next;
  }
}

void DepositCalculator::apply_replenishment(ActList *action, double *deposit) {
  if (action->periodicity == ActPeriodicity::AT_ONCE) {
    if (current_.compare(action->date, CompareMode::FULL_CMP) ==
        CompareResult::DATE_EQ) {
      *deposit += action->amount;
    }
  } else if (action->periodicity == ActPeriodicity::EVERY_MONTH) {
    if (current_.compare(action->date, CompareMode::DAY_CMP) ==
        CompareResult::DATE_EQ) {
      *deposit += action->amount;
    }
  } else if (action->periodicity == ActPeriodicity::EVERY_YEAR) {
    if (current_.compare(action->date, CompareMode::MONTH_CMP) ==
        CompareResult::DATE_EQ) {
      *deposit += action->amount;
    }
  }
}

void DepositCalculator::apply_withdrawal(ActList *action, double *deposit) {
  if (current_.compare(action->date, CompareMode::FULL_CMP) ==
      CompareResult::DATE_EQ) {
    *deposit -= action->amount;
    if (*deposit < 0) {
      *deposit = 0;
    }
  }
}

void DepositCalculator::periodic_payment(double *deposit) {
  if (payments_frequency_ == PayFrequency::MONTHLY &&
      start_.compare(current_, CompareMode::DAY_CMP) ==
          CompareResult::DATE_EQ) {
    calculate_one_periodic_payment(deposit, 12);
  } else if (payments_frequency_ == PayFrequency::ANNUALY) {
    DateCalculator end_of_year(1, 12, 0);
    if (end_of_year.compare(current_, CompareMode::MONTH_CMP) ==
        CompareResult::DATE_EQ) {
      calculate_one_periodic_payment(deposit, 1);
    }
  } else if (payments_frequency_ == PayFrequency::QUARTERLY) {
    if (is_any_quarter()) {
      calculate_one_periodic_payment(deposit, 4);
    }
  }
}

void DepositCalculator::calculate_one_periodic_payment(
    double *deposit, const short int divider) {
  *deposit += (*deposit * interest_rate_ / divider);
}

int DepositCalculator::set_year_type(int year) {
  int returnable = 365;
  if (year % 4 == 0) {
    returnable = 366;
  }
  return returnable;
}

bool DepositCalculator::is_any_quarter() {
  DateCalculator quart1(1, 3, 0);
  DateCalculator quart2(1, 6, 0);
  DateCalculator quart3(1, 9, 0);
  DateCalculator quart4(1, 12, 0);
  return quart1.compare(current_, CompareMode::MONTH_CMP) ==
             CompareResult::DATE_EQ ||
         quart2.compare(current_, CompareMode::MONTH_CMP) ==
             CompareResult::DATE_EQ ||
         quart3.compare(current_, CompareMode::MONTH_CMP) ==
             CompareResult::DATE_EQ ||
         quart4.compare(current_, CompareMode::MONTH_CMP) ==
             CompareResult::DATE_EQ;
}

void DepositCalculator::clear() {
  while (deposit_action_) {
    pop_action();
  }
  deposit_total_ = 0;
  deposit_term_ = 0;
  interest_rate_ = 0;
  tax_rate_ = 0;
  interest_capitalization_ = false;
  accrued_interest_ = 0;
  accrued_tax_ = 0;
  end_deposit_ = 0;
  payments_frequency_ = PayFrequency::MONTHLY;
}

void DepositCalculator::pop_action() {
  ActList *buffer = deposit_action_->next;
  delete deposit_action_;
  deposit_action_ = buffer;
}

void DepositCalculator::add_replenish(double deposit, int dd, int mm, int yy,
                                      ActPeriodicity periodicity) {
  ActList *buffer = new ActList(ActType::REPLENISH, periodicity, deposit, dd,
                                mm, yy, deposit_action_);
  deposit_action_ = buffer;
}

void DepositCalculator::add_withdrawal(double deposit, int dd, int mm, int yy) {
  ActList *buffer = new ActList(ActType::WITHDRAW, ActPeriodicity::AT_ONCE,
                                deposit, dd, mm, yy, deposit_action_);
  deposit_action_ = buffer;
}

void DepositCalculator::set_total_deposit(double value) {
  deposit_total_ = value;
}

void DepositCalculator::set_deposit_term(int months) { deposit_term_ = months; }

void DepositCalculator::set_interest_rate(double unit_fraction) {
  interest_rate_ = unit_fraction;
}

void DepositCalculator::set_tax_rate(double unit_fraction) {
  tax_rate_ = unit_fraction;
}

void DepositCalculator::set_payments_frequency(PayFrequency value) {
  payments_frequency_ = value;
}

void DepositCalculator::set_interest_capitalization(bool value) {
  interest_capitalization_ = value;
}

double DepositCalculator::get_accrued_interest() { return accrued_interest_; }

double DepositCalculator::get_accrued_tax() { return accrued_tax_; }

double DepositCalculator::get_end_deposit() { return end_deposit_; }

bool DepositCalculator::is_actList_empty() {
  return deposit_action_ == nullptr;
}

}  // namespace s21
