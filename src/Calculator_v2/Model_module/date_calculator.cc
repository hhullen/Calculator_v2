#include "date_calculator.h"

namespace s21 {

DateCalculator::DateCalculator() {
  dd_ = 1;
  mm_ = 1;
  yy_ = 1970;
}

DateCalculator::DateCalculator(const int dd, const int mm, const int yy)
    : dd_(dd), mm_(mm), yy_(yy) {}

void DateCalculator::set_date(const int dd, const int mm, const int yy) {
  dd_ = dd;
  mm_ = mm;
  yy_ = yy;
}

void DateCalculator::add_to_date(const DateCalculator &other) {
  increase_date_to(other.dd_, other.mm_, other.yy_);
}

void DateCalculator::add_to_date(const int dd, const int mm, const int yy) {
  increase_date_to(dd, mm, yy);
}

int DateCalculator::get_day() { return dd_; }
int DateCalculator::get_month() { return mm_; }
int DateCalculator::get_year() { return yy_; }

CompareResult DateCalculator::compare(const DateCalculator &other,
                                      CompareMode mode) {
  CompareResult returnable = CompareResult::DATE_EQ;

  if (yy_ > other.yy_ && is_full_cmp(mode)) {
    returnable = CompareResult::DATE_GT;
  } else if (yy_ < other.yy_ && is_full_cmp(mode)) {
    returnable = CompareResult::DATE_LT;
  } else if (mm_ > other.mm_ && is_full_mm_cmp(mode)) {
    returnable = CompareResult::DATE_GT;
  } else if (mm_ < other.mm_ && is_full_mm_cmp(mode)) {
    returnable = CompareResult::DATE_LT;
  } else if (dd_ > other.dd_ && is_full_mm_dd_cmp(mode)) {
    returnable = CompareResult::DATE_GT;
  } else if (dd_ < other.dd_ && is_full_mm_dd_cmp(mode)) {
    returnable = CompareResult::DATE_LT;
  }

  return returnable;
}

DateCalculator DateCalculator::operator=(const DateCalculator &other) {
  dd_ = other.dd_;
  mm_ = other.mm_;
  yy_ = other.yy_;

  return *this;
}

DateCalculator DateCalculator::operator++() {
  increase_date_to(1, 0, 0);
  return *this;
}

void DateCalculator::increase_date_to(const int dd, const int mm,
                                      const int yy) {
  int leap_year_days_[] = {31, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int common_year_days_[] = {31, 31, 28, 31, 30, 31, 30,
                             31, 31, 30, 31, 30, 31};
  int *monthDays = &common_year_days_[0];

  yy_ += yy;
  mm_ += mm;
  dd_ += dd;

  if (yy_ % 4 == 0) {
    monthDays = &leap_year_days_[0];
  }

  while (dd_ > monthDays[mm_ % 12]) {
    dd_ -= monthDays[mm_ % 12];
    mm_ += 1;
  }
  if (dd_ == 0) dd_ = 1;
  while (mm_ > 12) {
    mm_ -= 12;
    yy_ += 1;
  }
  if (mm_ == 0) mm_ = 1;
}

bool DateCalculator::is_full_cmp(CompareMode mode) {
  return mode == CompareMode::FULL_CMP;
}

bool DateCalculator::is_full_mm_cmp(CompareMode mode) {
  return (mode == CompareMode::FULL_CMP || mode == CompareMode::MONTH_CMP);
}

bool DateCalculator::is_full_mm_dd_cmp(CompareMode mode) {
  return (mode == CompareMode::FULL_CMP || mode == CompareMode::MONTH_CMP ||
          mode == CompareMode::DAY_CMP);
}

}  // namespace s21
