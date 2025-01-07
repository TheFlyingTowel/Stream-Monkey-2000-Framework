#pragma once


namespace SM2K
{
#define N_VA_ARGS_(_30,_29,_28,_27,_26,_25,_24,_23,_22,_21,_20,_19,_18,_17,_16,_15,_14,_13,_12,_11,_10,_9,_8,_7,_6,_5,_4,_3,_2,_1, N, ...) N
#define N_VA_ARGS(...) N_VA_ARGS_(__VA_ARGS__ __VA_OPT__(,) 30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define FOREACH_0(FN, ...) 
#define FOREACH_1(FN, E, ...)  FN(E) 
#define FOREACH_2(FN, E, ...)  FN(E) FOREACH_1(FN, __VA_ARGS__)
#define FOREACH_3(FN, E, ...)  FN(E) FOREACH_2(FN, __VA_ARGS__)
#define FOREACH_4(FN, E, ...)  FN(E) FOREACH_3(FN, __VA_ARGS__)
#define FOREACH_5(FN, E, ...)  FN(E) FOREACH_4(FN, __VA_ARGS__)
#define FOREACH_6(FN, E, ...)  FN(E) FOREACH_5(FN, __VA_ARGS__)
#define FOREACH_7(FN, E, ...)  FN(E) FOREACH_6(FN, __VA_ARGS__)
#define FOREACH_8(FN, E, ...)  FN(E) FOREACH_7(FN, __VA_ARGS__)
#define FOREACH_9(FN, E, ...)  FN(E) FOREACH_8(FN, __VA_ARGS__)
#define FOREACH_10(FN, E, ...)  FN(E) FOREACH_9(FN, __VA_ARGS__)
#define FOREACH_11(FN, E, ...)  FN(E) FOREACH_10(FN, __VA_ARGS__)
#define FOREACH_12(FN, E, ...)  FN(E) FOREACH_11(FN, __VA_ARGS__)
#define FOREACH_13(FN, E, ...)  FN(E) FOREACH_12(FN, __VA_ARGS__)
#define FOREACH_14(FN, E, ...)  FN(E) FOREACH_13(FN, __VA_ARGS__)
#define FOREACH_15(FN, E, ...)  FN(E) FOREACH_14(FN, __VA_ARGS__)
#define FOREACH_16(FN, E, ...)  FN(E) FOREACH_15(FN, __VA_ARGS__)
#define FOREACH_17(FN, E, ...)  FN(E) FOREACH_16(FN, __VA_ARGS__)
#define FOREACH_18(FN, E, ...)  FN(E) FOREACH_17(FN, __VA_ARGS__)
#define FOREACH_19(FN, E, ...)  FN(E) FOREACH_18(FN, __VA_ARGS__)
#define FOREACH_20(FN, E, ...)  FN(E) FOREACH_19(FN, __VA_ARGS__)
#define FOREACH_21(FN, E, ...)  FN(E) FOREACH_20(FN, __VA_ARGS__)
#define FOREACH_22(FN, E, ...)  FN(E) FOREACH_21(FN, __VA_ARGS__)
#define FOREACH_23(FN, E, ...)  FN(E) FOREACH_22(FN, __VA_ARGS__)
#define FOREACH_24(FN, E, ...)  FN(E) FOREACH_23(FN, __VA_ARGS__)
#define FOREACH_25(FN, E, ...)  FN(E) FOREACH_24(FN, __VA_ARGS__)
#define FOREACH_26(FN, E, ...)  FN(E) FOREACH_25(FN, __VA_ARGS__)
#define FOREACH_27(FN, E, ...)  FN(E) FOREACH_26(FN, __VA_ARGS__)
#define FOREACH_28(FN, E, ...)  FN(E) FOREACH_27(FN, __VA_ARGS__)
#define FOREACH_29(FN, E, ...)  FN(E) FOREACH_28(FN, __VA_ARGS__)
#define FOREACH_30(FN, E, ...)  FN(E) FOREACH_29(FN, __VA_ARGS__)

#define FOREACH__(FN, NARGS, ...) FOREACH_##NARGS(FN, __VA_ARGS__) 
#define FOREACH_(FN, NARGS, ...) FOREACH__(FN, NARGS, __VA_ARGS__)
#define FOREACH(FN, ...) FOREACH_(FN, N_VA_ARGS(__VA_ARGS__), __VA_ARGS__)

#define ADD_CMD_TOKEN(_cmdName)   Interpreter::s_cmdTable[#_cmdName] = Token(CMD, #_cmdName);
#define ADD_CMD_TOKENS(...)   FOREACH_(ADD_CMD_TOKEN, N_VA_ARGS(__VA_ARGS__), __VA_ARGS__)

#define TOKEN(_tokenName) HashingFunctions::fnv1a_hash(#_tokenName)
#define TOKEN_H(_tokenName) HashingFunctions::fnv1a_hash(_tokenName)


#define ____TOKEN(_tokenName) case TOKEN(_tokenName): 
#define TOKEN_CASE(...) FOREACH_(____TOKEN, N_VA_ARGS(__VA_ARGS__), __VA_ARGS__)

};

