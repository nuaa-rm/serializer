//
// Created by bismarck on 22-7-2.
//

#ifndef MACROMAGIC_PP_H
#define MACROMAGIC_PP_H

#define PP_COMMA() ,
#define PP_EMPTY()

#define PP_CONCAT(A, B) PP_CONCAT_IMPL(A, B)
#define PP_CONCAT_IMPL(A, B) A##B

#define PP_NOT(N) PP_CONCAT(PP_NOT_, N)
#define PP_NOT_0 1
#define PP_NOT_1 0

#define PP_AND(A, B) PP_CONCAT(PP_AND_, PP_CONCAT(A, B))
#define PP_AND_00 0
#define PP_AND_01 0
#define PP_AND_10 0
#define PP_AND_11 1

#define PP_BOOL(N) PP_CONCAT(PP_BOOL_, N)
#define PP_BOOL_ 0
#define PP_BOOL_0 0
#define PP_BOOL_1 1
#define PP_BOOL_2 1
#define PP_BOOL_3 1
#define PP_BOOL_4 1
#define PP_BOOL_5 1
#define PP_BOOL_6 1
#define PP_BOOL_7 1
#define PP_BOOL_8 1
#define PP_BOOL_9 1
#define PP_BOOL_10 1
#define PP_BOOL_11 1
#define PP_BOOL_12 1
#define PP_BOOL_13 1
#define PP_BOOL_14 1
#define PP_BOOL_15 1
#define PP_BOOL_16 1

#define PP_IF(PRED, THEN, ELSE) PP_CONCAT(PP_IF_, PP_BOOL(PRED))(THEN, ELSE)
#define PP_IF_1(THEN, ELSE) THEN
#define PP_IF_01(THEN, ELSE) THEN
#define PP_IF_0(THEN, ELSE) ELSE
#define PP_IF_00(THEN, ELSE) ELSE

#define PP_COMMA_IF(N) PP_IF(N, PP_COMMA, PP_EMPTY)()
#define PP_VA_OPT_COMMA(...) PP_COMMA_IF(PP_NOT(PP_IS_EMPTY(__VA_ARGS__)))

#define PP_INC(N) PP_CONCAT(PP_INC_, N)
#define PP_INC_1 0
#define PP_INC_2 1
#define PP_INC_3 2
#define PP_INC_4 3
#define PP_INC_5 4
#define PP_INC_6 5
#define PP_INC_7 6
#define PP_INC_8 7
#define PP_INC_9 8
#define PP_INC_10 9
#define PP_INC_11 10
#define PP_INC_12 11
#define PP_INC_13 12
#define PP_INC_14 13
#define PP_INC_15 14
#define PP_INC_16 15
#define PP_INC_17 16
#define PP_INC_18 17
#define PP_INC_19 18
#define PP_INC_20 19

#define PP_GET_N(N, ...) PP_CONCAT(PP_GET_N_, N)(__VA_ARGS__)
#define PP_GET_N_0(_0, ...) _0
#define PP_GET_N_1(_0, _1, ...) _1
#define PP_GET_N_2(_0, _1, _2, ...) _2
#define PP_GET_N_3(_0, _1, _2, _3, ...) _3
#define PP_GET_N_4(_0, _1, _2, _3, _4, ...) _4
#define PP_GET_N_5(_0, _1, _2, _3, _4, _5, ...) _5
#define PP_GET_N_6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define PP_GET_N_7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define PP_GET_N_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define PP_GET_N_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define PP_GET_N_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define PP_GET_N_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define PP_GET_N_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define PP_GET_N_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define PP_GET_N_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14
#define PP_GET_N_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define PP_GET_N_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16

#define PP_IS_EMPTY(...)                                      \
  PP_AND(PP_AND(PP_NOT(PP_HAS_COMMA(__VA_ARGS__)),            \
                PP_NOT(PP_HAS_COMMA(__VA_ARGS__()))),         \
         PP_AND(PP_NOT(PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__)), \
                PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__())))
#define PP_HAS_COMMA(...) PP_GET_N_16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define PP_COMMA_V(...) ,

#define PP_NARG(...)                                                           \
  PP_GET_N(16, __VA_ARGS__ PP_VA_OPT_COMMA(__VA_ARGS__) 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, \
           0)

#define PP_FOR_EACH(DO, ...) \
  PP_CONCAT(PP_FOR_EACH_, PP_NARG(__VA_ARGS__))(DO, 0, __VA_ARGS__)
#define PP_FOR_EACH_0(DO, IDX, ...)
#define PP_FOR_EACH_1(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_0(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_2(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_1(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_3(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_2(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_4(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_3(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_5(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_4(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_6(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_5(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_7(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_6(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_8(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_7(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_9(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_8(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_10(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_9(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_11(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_10(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_12(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_11(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_13(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_12(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_14(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_13(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_15(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_14(DO, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_16(DO, IDX, VAR, ...) \
  DO(VAR, IDX)                           \
  PP_FOR_EACH_15(DO, PP_INC(IDX), __VA_ARGS__)

#define PP_EMPTY_V(...)
#define PP_IS_PARENS(SYMBOL) PP_IS_EMPTY(PP_EMPTY_V SYMBOL)


#endif //MACROMAGIC_PP_H
