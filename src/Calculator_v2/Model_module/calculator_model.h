#ifndef SRC_CALCULATOR_V2_MODEL_MODULE_CALCULATOR_MODEL_H
#define SRC_CALCULATOR_V2_MODEL_MODULE_CALCULATOR_MODEL_H

#include "calculator.h"
#include "credit_calculator.h"
#include "deposit_calculator.h"
using s21::Calculator;
using s21::CreditCalculator;
using s21::DateCalculator;
using s21::DepositCalculator;

namespace s21 {

class CalculatorModel {
 public:
  Calculator calculator;
  CreditCalculator credit;
  DepositCalculator deposit;
};

}  // namespace s21

#endif  // SRC_CALCULATOR_V2_MODEL_MODULE_CALCULATOR_MODEL_H
