#ifndef SRC_CALCULATOR_V2_MODEL_MODULE_DATE_CALCULATOR_H
#define SRC_CALCULATOR_V2_MODEL_MODULE_DATE_CALCULATOR_H

namespace s21 {

enum class CompareMode { FULL_CMP, MONTH_CMP, DAY_CMP };

enum class CompareResult { DATE_EQ, DATE_GT, DATE_LT };

class DateCalculator {
 public:
  DateCalculator();
  DateCalculator(const int dd, const int mm, const int yy);
  void set_date(const int dd, const int mm, const int yy);
  void add_to_date(const DateCalculator &other);
  void add_to_date(const int dd, const int mm, const int yy);
  int get_day();
  int get_month();
  int get_year();
  CompareResult compare(const DateCalculator &other, CompareMode mode);

  DateCalculator operator=(const DateCalculator &other);
  DateCalculator operator++();

 private:
  int dd_;
  int mm_;
  int yy_;

  void increase_date_to(const int dd, const int mm, const int yy);
  bool is_full_cmp(CompareMode mode);
  bool is_full_mm_cmp(CompareMode mode);
  bool is_full_mm_dd_cmp(CompareMode mode);
};

}  // namespace s21

#endif  // SRC_CALCULATOR_V2_MODEL_MODULE_DATE_CALCULATOR_H
