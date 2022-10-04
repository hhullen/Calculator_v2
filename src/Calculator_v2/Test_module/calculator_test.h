#ifndef SRC_CALCULATOR_V2_TEST_MODULE_CALCULATOR_TEST_H_
#define SRC_CALCULATOR_V2_TEST_MODULE_CALCULATOR_TEST_H_

#include <gtest/gtest.h>

#include "../Model_module/calculator.h"
#include "../Model_module/credit_calculator.h"
#include "../Model_module/deposit_calculator.h"

using s21::Calculator;
using s21::CreditCalculator;
using s21::DateCalculator;
using s21::DepositCalculator;

const double kACCURACUY = 0.0000001;

// operators_calculations
char OC1[] = {'5',           s21::POINT, '5',         s21::PLUS, '6',
              s21::MULTIPLY, '2',        s21::DIVIDE, '3',       s21::EXTENT,
              '4',           s21::MINUS, '1',         '6',       s21::END};
char OC2[] = {'2', s21::EXTENT, '4', s21::EXTENT, '2', s21::END};
char OC3[] = {'0', s21::POINT,    '1', '2',       '9',
              '6', s21::MULTIPLY, '2', s21::PLUS, '1',
              '7', s21::POINT,    '3', '4',       s21::END};
char OC4[] = {'1', s21::PLUS,     '7',      s21::MINUS,
              '3', s21::MULTIPLY, s21::EXP, s21::END};
char OC5[] = {'8',         s21::DIVIDE, '4',       s21::MULTIPLY, '2',
              s21::EXTENT, '3',         s21::PLUS, s21::PI,       s21::END};
char OC6[] = {'2', '4', s21::MOD, '7', s21::END};
char OC7[] = {'2',       s21::EXTENT, '3',        s21::DIVIDE, '2',
              s21::PLUS, '4',         s21::MINUS, '7',         s21::END};
char OC8[] = {'5',        s21::PLUS,     '3',         s21::EXTENT, '2',
              s21::MINUS, '4',           s21::DIVIDE, '1',         s21::POINT,
              '1',        s21::MULTIPLY, '1',         '0',         s21::END};
char OC9[] = {'5', s21::DIVIDE, '0', s21::END};
char OC10[] = {'2', s21::EXTENT,   s21::LBRACKET, s21::UMINUS,
               '2', s21::RBRACKET, s21::END};

double OCRES[] = {-10.35185185, 65536.0, 17.5992, -0.154845485,
                  19.141592653, 3.0,     1.0,     -22.36363636,
                  INFINITY,     0.25};

// operators_calculations_BRACKETS
char OCBR1[] = {s21::UMINUS, '1',           '6',           s21::POINT,
                '4',         '4',           s21::PLUS,     s21::LBRACKET,
                s21::COS,    s21::LBRACKET, '1',           '5',
                s21::PLUS,   '1',           s21::RBRACKET, s21::MULTIPLY,
                '7',         s21::RBRACKET, s21::END};
char OCBR2[] = {s21::LBRACKET, '1',         '0',           s21::POINT,
                '1',           '2',         s21::PLUS,     '1',
                '9',           '8',         s21::RBRACKET, s21::MULTIPLY,
                s21::LBRACKET, s21::UMINUS, '6',           s21::MINUS,
                '1',           '0',         s21::RBRACKET, s21::END};
char OCBR3[] = {s21::LBRACKET, '2',         s21::EXTENT, '4',
                s21::RBRACKET, s21::EXTENT, '2',         s21::END};
char OCBR4[] = {'4',       s21::DIVIDE,   '5',     s21::EXTENT, '6',
                s21::PLUS, s21::LBRACKET, '1',     '0',         s21::MOD,
                '3',       s21::RBRACKET, s21::END};
char OCBR5[] = {s21::LBRACKET, '2', s21::EXTENT,   s21::LBRACKET, '1',
                s21::PLUS,     '3', s21::RBRACKET, s21::RBRACKET, s21::MINUS,
                '1',           '0', s21::END};
char OCBR6[] = {s21::LBRACKET, s21::LBRACKET, s21::LBRACKET, '1',
                '0',           s21::PLUS,     '5',           s21::RBRACKET,
                s21::DIVIDE,   '3',           s21::RBRACKET, s21::MULTIPLY,
                '8',           s21::RBRACKET, s21::PLUS,     '5',
                s21::END};
char OCBR7[] = {s21::LBRACKET, '1',           s21::PLUS,     s21::LBRACKET,
                '3',           s21::MINUS,    s21::LBRACKET, s21::UMINUS,
                '6',           s21::MULTIPLY, '2',           s21::DIVIDE,
                s21::LBRACKET, '1',           s21::PLUS,     '1',
                s21::RBRACKET, s21::RBRACKET, s21::RBRACKET, s21::RBRACKET,
                s21::END};
char OCBR8[] = {'1', '2',           s21::POINT, '9',           '7',
                '8', '9',           '1',        s21::MULTIPLY, s21::LBRACKET,
                '0', s21::RBRACKET, s21::END};
char OCBR9[] = {'1',           s21::PLUS,     s21::LBRACKET, s21::LBRACKET,
                s21::LBRACKET, '1',           '9',           s21::RBRACKET,
                s21::RBRACKET, s21::RBRACKET, s21::END};
char OCBR10[] = {'1',           '9',           s21::PLUS,     s21::LBRACKET,
                 s21::LBRACKET, '1',           '0',           s21::MOD,
                 '6',           s21::RBRACKET, s21::PLUS,     s21::LBRACKET,
                 s21::UMINUS,   '5',           s21::RBRACKET, s21::RBRACKET,
                 s21::END};

double OCBRRES[] = {-9.71116813, -3329.92, 256, 1.000256, 6, 45, 10, 0, 20, 18};

// functions_calculations
char FC1[] = {s21::COS,      s21::LBRACKET, '1', '5',
              s21::RBRACKET, s21::PLUS,     '1', s21::END};
char FC2[] = {s21::SIN,      s21::LBRACKET, '1', '5',
              s21::RBRACKET, s21::PLUS,     '1', s21::END};
char FC3[] = {s21::TAN,      s21::LBRACKET, '1', '5',
              s21::RBRACKET, s21::PLUS,     '1', s21::END};
char FC4[] = {s21::ACOS,     s21::LBRACKET, s21::UMINUS, '1',
              s21::RBRACKET, s21::PLUS,     '1',         s21::END};
char FC5[] = {s21::ASIN,     s21::LBRACKET, s21::UMINUS, '1',
              s21::RBRACKET, s21::PLUS,     '1',         s21::END};
char FC6[] = {s21::ATAN,     s21::LBRACKET, '1', '5',
              s21::RBRACKET, s21::PLUS,     '1', s21::END};
char FC7[] = {s21::SQRT, s21::LBRACKET, '2', '9',           '0',     '0',
              '2',       '0',           '9', s21::RBRACKET, s21::END};
char FC8[] = {s21::LN,       s21::LBRACKET, '1', '5',
              s21::RBRACKET, s21::PLUS,     '1', s21::END};
char FC9[] = {s21::LOG,      s21::LBRACKET, '1', '5',
              s21::RBRACKET, s21::PLUS,     '1', s21::END};
char FC10[] = {s21::TAN, s21::LBRACKET, '8',       '9', s21::POINT, '9',
               '9',      s21::RBRACKET, s21::PLUS, '1', s21::END};

double FCRES[] = {1.96592583, 1.25881905,   1.267949192, 181,
                  -89,        87.18592517,  1703,        3.70805020,
                  2.17609126, 5730.57789312};

char RC1[] = {s21::COS, s21::LBRACKET, s21::VARX, s21::RBRACKET, s21::END};
char RC2[] = {s21::COS,  s21::LBRACKET, s21::SIN,      s21::LBRACKET,
              s21::VARX, s21::RBRACKET, s21::RBRACKET, s21::END};
char RC3[] = {s21::TAN, s21::LBRACKET, s21::VARX, s21::RBRACKET, s21::END};
char RC4[] = {s21::UMINUS,   '1',           s21::POINT, '6',
              '4',           '4',           s21::EXP,   '1',
              s21::PLUS,     s21::LBRACKET, s21::COS,   s21::LBRACKET,
              '1',           '5',           s21::PLUS,  s21::VARX,
              s21::RBRACKET, s21::MULTIPLY, '7',        s21::RBRACKET,
              s21::END};
char RC5[] = {'1',       s21::PLUS,   s21::VARX,     s21::PLUS, '3',
              s21::PLUS, s21::VARX,   s21::MULTIPLY, '5',       s21::MULTIPLY,
              '6',       s21::EXTENT, '7',           s21::END};

#endif  // SRC_CALCULATOR_V2_TEST_MODULE_CALCULATOR_TEST_H_
