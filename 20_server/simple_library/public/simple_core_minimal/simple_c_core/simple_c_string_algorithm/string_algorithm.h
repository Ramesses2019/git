#pragma once
#include "../simple_core_minimal.h"
#include <no_sal2.h>

void remove_char_start(char *str, char sub_str);

void remove_char_end(char *str, char sub_str);

void remove_all_char_end(char *str, char sub_str);

void replace_char_inline(char *str, char const* sub_char_a, char const* sub_char_b);

int find_string(char *str, char const* sub_str);

void remove_string_start(char *str, char const* sub_str);

//_Out_ 空宏 用于提示程序员
int get_printf_s(_Out_ char* out_buf, _In_z_ _Printf_format_string_ char *format,...);

int get_printf(_Out_ char *buf, _In_z_ _Printf_format_string_ char *format,...);

char *string_mid(_In_ const char *mid_buf, _Out_ char *out_buf, int start, int count);

