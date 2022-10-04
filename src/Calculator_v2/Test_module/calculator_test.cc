#include "calculator_test.h"

void run_test(char **oc_vals, double *ocres) {
  Calculator calc;
  double result = 0.0;

  for (int i = 0; i < 10; i++) {
    result = calc.calculate_expression(oc_vals[i], 0.0);
    if (result != INFINITY) {
      EXPECT_EQ((fabs(result - ocres[i]) < kACCURACUY), 1);
    } else {
      EXPECT_TRUE(result == INFINITY && ocres[i] == INFINITY);
    }
  }
}

void init_operators_calculations_vals(char **oc_vals) {
  oc_vals[0] = OC1;
  oc_vals[1] = OC2;
  oc_vals[2] = OC3;
  oc_vals[3] = OC4;
  oc_vals[4] = OC5;
  oc_vals[5] = OC6;
  oc_vals[6] = OC7;
  oc_vals[7] = OC8;
  oc_vals[8] = OC9;
  oc_vals[9] = OC10;
}

void init_operators_calculations_vals_BRACKETS(char **oc_vals) {
  oc_vals[0] = OCBR1;
  oc_vals[1] = OCBR2;
  oc_vals[2] = OCBR3;
  oc_vals[3] = OCBR4;
  oc_vals[4] = OCBR5;
  oc_vals[5] = OCBR6;
  oc_vals[6] = OCBR7;
  oc_vals[7] = OCBR8;
  oc_vals[8] = OCBR9;
  oc_vals[9] = OCBR10;
}

void init_functions_calculations_vals(char **oc_vals) {
  oc_vals[0] = FC1;
  oc_vals[1] = FC2;
  oc_vals[2] = FC3;
  oc_vals[3] = FC4;
  oc_vals[4] = FC5;
  oc_vals[5] = FC6;
  oc_vals[6] = FC7;
  oc_vals[7] = FC8;
  oc_vals[8] = FC9;
  oc_vals[9] = FC10;
}

TEST(test_calculator, operators_calculations) {
  char **oc_vals = new char *[10];

  init_operators_calculations_vals(oc_vals);
  run_test(oc_vals, OCRES);
  delete[] oc_vals;
}

TEST(test_calculator, operators_calculations_BRACKETS) {
  char **oc_vals = new char *[10];

  init_operators_calculations_vals_BRACKETS(oc_vals);
  run_test(oc_vals, OCBRRES);
  delete[] oc_vals;
}

TEST(test_calculator, functions_calculations) {
  char **oc_vals = new char *[10];

  init_functions_calculations_vals(oc_vals);
  run_test(oc_vals, FCRES);
  delete[] oc_vals;
}

TEST(test_calculator, range_calculations_01) {
  Calculator calc;
  calc.calculate_range(RC1, 0, 100);
}

TEST(test_calculator, range_calculations_02) {
  Calculator calc;
  calc.calculate_range(RC2, -200701, 200080);
}

TEST(test_calculator, range_calculations_03) {
  Calculator calc;
  calc.calculate_range(RC3, -440670, 500000);
}

TEST(test_calculator, range_calculations_04) {
  Calculator calc;
  calc.calculate_range(RC4, -1000000, 1000000);
  std::vector<double> vec_x = calc.get_x_values_vector();
  std::vector<double> vec_y = calc.get_y_values_vector();
}

TEST(test_calculator, range_calculations_05) {
  Calculator calc;
  calc.calculate_range(RC5, -1, 0);
}

TEST(test_calculator, build_expression_1) {
  Calculator calc;
  calc.add_expression_lexem("_");
  calc.add_expression_lexem("5");
  calc.add_expression_lexem("+");
  calc.add_expression_lexem("1");
  calc.add_expression_lexem("0");
  calc.calculate_expression(0.0);
  EXPECT_EQ(calc.calculate_expression(0.0), 5);
}

TEST(test_calculator, build_expression_2) {
  Calculator calc;
  calc.add_expression_lexem("_");
  calc.add_expression_lexem("1");
  calc.add_expression_lexem("+");
  calc.add_expression_lexem("2");
  calc.add_expression_lexem("+");
  calc.add_expression_lexem("3");
  calc.add_expression_lexem("+");
  calc.add_expression_lexem("4");
  calc.add_expression_lexem("+");
  calc.add_expression_lexem("5");
  calc.add_expression_lexem("+");
  calc.add_expression_lexem("6");
  calc.add_expression_lexem("-");
  calc.add_expression_lexem("7");
  calc.add_expression_lexem("*");
  calc.add_expression_lexem("8");
  calc.add_expression_lexem("/");
  calc.add_expression_lexem("9");
  calc.add_expression_lexem("mod");
  calc.add_expression_lexem("0");
  calc.add_expression_lexem("^");
  calc.add_expression_lexem("sin");
  calc.add_expression_lexem("(");
  calc.add_expression_lexem("cos");
  calc.add_expression_lexem("(");
  calc.add_expression_lexem("tan");
  calc.add_expression_lexem("(");
  calc.add_expression_lexem("asin");
  calc.add_expression_lexem("(");
  calc.add_expression_lexem("acos");
  calc.add_expression_lexem("(");
  calc.add_expression_lexem("atan");
  calc.add_expression_lexem("(");
  calc.add_expression_lexem("sqrt");
  calc.add_expression_lexem("(");
  calc.add_expression_lexem("ln");
  calc.add_expression_lexem("(");
  calc.add_expression_lexem("log");
  calc.add_expression_lexem("(");
  calc.add_expression_lexem("X");
  calc.add_expression_lexem("+");
  calc.add_expression_lexem("PI");
  calc.add_expression_lexem(")");
  calc.add_expression_lexem(")");
  calc.add_expression_lexem(")");
  calc.add_expression_lexem(")");
  calc.add_expression_lexem(")");
  calc.add_expression_lexem(")");
  calc.add_expression_lexem(")");
  calc.add_expression_lexem(")");
  calc.add_expression_lexem(")");

  char *expression = calc.get_expression();
  EXPECT_EQ(expression[0], s21::Lexems::UMINUS);
  EXPECT_EQ(expression[1], '1');
  EXPECT_EQ(expression[2], s21::Lexems::PLUS);
  EXPECT_EQ(expression[3], '2');
  EXPECT_EQ(expression[4], s21::Lexems::PLUS);
  EXPECT_EQ(expression[5], '3');
  EXPECT_EQ(expression[6], s21::Lexems::PLUS);
  EXPECT_EQ(expression[7], '4');
  EXPECT_EQ(expression[8], s21::Lexems::PLUS);
  EXPECT_EQ(expression[9], '5');
  EXPECT_EQ(expression[10], s21::Lexems::PLUS);
  EXPECT_EQ(expression[11], '6');
  EXPECT_EQ(expression[12], s21::Lexems::MINUS);
  EXPECT_EQ(expression[13], '7');
  EXPECT_EQ(expression[14], s21::Lexems::MULTIPLY);
  EXPECT_EQ(expression[15], '8');
  EXPECT_EQ(expression[16], s21::Lexems::DIVIDE);
  EXPECT_EQ(expression[17], '9');
  EXPECT_EQ(expression[18], s21::Lexems::MOD);
  EXPECT_EQ(expression[19], '0');
  EXPECT_EQ(expression[20], s21::Lexems::EXTENT);
  EXPECT_EQ(expression[21], s21::Lexems::SIN);
  EXPECT_EQ(expression[22], s21::Lexems::LBRACKET);
  EXPECT_EQ(expression[23], s21::Lexems::COS);
  EXPECT_EQ(expression[24], s21::Lexems::LBRACKET);
  EXPECT_EQ(expression[25], s21::Lexems::TAN);
  EXPECT_EQ(expression[26], s21::Lexems::LBRACKET);
  EXPECT_EQ(expression[27], s21::Lexems::ASIN);
  EXPECT_EQ(expression[28], s21::Lexems::LBRACKET);
  EXPECT_EQ(expression[29], s21::Lexems::ACOS);
  EXPECT_EQ(expression[30], s21::Lexems::LBRACKET);
  EXPECT_EQ(expression[31], s21::Lexems::ATAN);
  EXPECT_EQ(expression[32], s21::Lexems::LBRACKET);
  EXPECT_EQ(expression[33], s21::Lexems::SQRT);
  EXPECT_EQ(expression[34], s21::Lexems::LBRACKET);
  EXPECT_EQ(expression[35], s21::Lexems::LN);
  EXPECT_EQ(expression[36], s21::Lexems::LBRACKET);
  EXPECT_EQ(expression[37], s21::Lexems::LOG);
  EXPECT_EQ(expression[38], s21::Lexems::LBRACKET);
  EXPECT_EQ(expression[39], s21::Lexems::VARX);
  EXPECT_EQ(expression[40], s21::Lexems::PLUS);
  EXPECT_EQ(expression[41], s21::Lexems::PI);
  EXPECT_EQ(expression[42], s21::Lexems::RBRACKET);
  EXPECT_EQ(expression[43], s21::Lexems::RBRACKET);
  EXPECT_EQ(expression[44], s21::Lexems::RBRACKET);
  EXPECT_EQ(expression[45], s21::Lexems::RBRACKET);
  EXPECT_EQ(expression[46], s21::Lexems::RBRACKET);
  EXPECT_EQ(expression[47], s21::Lexems::RBRACKET);
  EXPECT_EQ(expression[48], s21::Lexems::RBRACKET);
  EXPECT_EQ(expression[49], s21::Lexems::RBRACKET);
  EXPECT_EQ(expression[50], s21::Lexems::RBRACKET);

  for (int i = 0; i < 51; ++i) {
    calc.erase_expression_lexem();
  }
  EXPECT_EQ(expression[0], s21::Lexems::END);
}

TEST(test_credit_calculator, credit_mode_01) {
  CreditCalculator calc;
  calc.calculate_credit(120000.0, 24, 13.9, s21::CreditMode::ANNUITY);

  EXPECT_TRUE(abs(calc.get_monthly_payment_high() - 5755.88) <=
              s21::kMONEY_ACCURACY);
  EXPECT_TRUE(abs(calc.get_overpayment() - 18141.09) <= s21::kMONEY_ACCURACY);
  EXPECT_TRUE(abs(calc.get_total_payment() - 138141.09) <=
              s21::kMONEY_ACCURACY);
}

TEST(test_credit_calculator, credit_mode_02) {
  CreditCalculator calc;
  calc.calculate_credit(120000.0, 24, 13.9, s21::CreditMode::DIFFERENTIATED);

  EXPECT_TRUE(abs(calc.get_monthly_payment_high() - 6390.00) <=
              s21::kMONEY_ACCURACY);
  EXPECT_TRUE(abs(calc.get_monthly_payment_low() - 5057.92) <=
              s21::kMONEY_ACCURACY);
  EXPECT_TRUE(abs(calc.get_overpayment() - 17375.00) <= s21::kMONEY_ACCURACY);
  EXPECT_TRUE((calc.get_total_payment() - 137375.00) <= s21::kMONEY_ACCURACY);
}

TEST(test_deposit_calculator, deposit_mode_01) {
  DepositCalculator calc;

  calc.set_total_deposit(1000.0);
  calc.set_deposit_term(12);
  calc.set_interest_rate(0.05);
  calc.set_tax_rate(0.13);
  calc.set_interest_capitalization(true);
  calc.set_payments_frequency(s21::PayFrequency::MONTHLY);
  calc.add_replenish(100, 14, 9, 22, s21::ActPeriodicity::AT_ONCE);
  calc.add_withdrawal(1, 1, 9, 22);
  DateCalculator now(14, 7, 22);
  calc.calculate_deposit(now);

  EXPECT_EQ(abs(calc.get_accrued_interest() - 0.1539) <= s21::kMONEY_ACCURACY,
            true);
  EXPECT_EQ(abs(calc.get_accrued_tax()) <= s21::kMONEY_ACCURACY, true);
  EXPECT_EQ(abs(calc.get_end_deposit() - 1153.94) <= s21::kMONEY_ACCURACY,
            true);
  calc.clear();
}

TEST(test_deposit_calculator, deposit_mode_02) {
  DepositCalculator calc;

  calc.set_total_deposit(1000.0);
  calc.set_deposit_term(12);
  calc.set_interest_rate(0.05);
  calc.set_tax_rate(0.13);
  calc.set_interest_capitalization(true);
  calc.set_payments_frequency(s21::PayFrequency::QUARTERLY);
  calc.add_replenish(100, 14, 9, 22, s21::ActPeriodicity::EVERY_MONTH);
  calc.add_withdrawal(1, 1, 9, 22);
  DateCalculator now(14, 7, 22);
  calc.calculate_deposit(now);

  EXPECT_EQ(abs(calc.get_accrued_interest() - 1.28) <= s21::kMONEY_ACCURACY,
            true);
  EXPECT_EQ(abs(calc.get_accrued_tax()) <= s21::kMONEY_ACCURACY, true);
  EXPECT_EQ(abs(calc.get_end_deposit() - 2283.13) <= s21::kMONEY_ACCURACY,
            true);
  calc.clear();
}

TEST(test_deposit_calculator, deposit_mode_03) {
  DepositCalculator calc;

  calc.set_total_deposit(1000.0);
  calc.set_deposit_term(12);
  calc.set_interest_rate(0.05);
  calc.set_tax_rate(0.13);
  calc.set_interest_capitalization(true);
  calc.set_payments_frequency(s21::PayFrequency::QUARTERLY);
  calc.add_replenish(100, 14, 9, 22, s21::ActPeriodicity::EVERY_YEAR);
  calc.add_withdrawal(1, 1, 9, 22);
  DateCalculator now(14, 7, 22);
  calc.calculate_deposit(now);

  EXPECT_EQ(abs(calc.get_accrued_interest() - 0.1540) <= s21::kMONEY_ACCURACY,
            true);
  EXPECT_EQ(abs(calc.get_accrued_tax()) <= s21::kMONEY_ACCURACY, true);
  EXPECT_EQ(abs(calc.get_end_deposit() - 1154.05) <= s21::kMONEY_ACCURACY,
            true);
  calc.clear();
}

TEST(test_deposit_calculator, deposit_mode_04) {
  DepositCalculator calc;

  calc.set_total_deposit(1000.0);
  calc.set_deposit_term(12);
  calc.set_interest_rate(0.05);
  calc.set_tax_rate(0.13);
  calc.set_interest_capitalization(true);
  calc.set_payments_frequency(s21::PayFrequency::ANNUALY);
  calc.add_replenish(100, 14, 9, 22, s21::ActPeriodicity::EVERY_YEAR);
  calc.add_withdrawal(1, 1, 9, 22);
  DateCalculator now(14, 7, 22);
  calc.calculate_deposit(now);

  EXPECT_EQ(abs(calc.get_accrued_interest() - 0.1549) <= s21::kMONEY_ACCURACY,
            true);
  EXPECT_EQ(abs(calc.get_accrued_tax()) <= s21::kMONEY_ACCURACY, true);
  EXPECT_EQ(abs(calc.get_end_deposit() - 1154.92) <= s21::kMONEY_ACCURACY,
            true);
  calc.clear();
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
