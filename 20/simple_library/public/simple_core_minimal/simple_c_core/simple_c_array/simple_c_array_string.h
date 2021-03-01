#pragma once
#include "Core/simple_array_c_macro.h"

typedef struct
{
	char data[256];
}str_node;

SIMPLE_ARRAY_C_STRUCT(simple_c_string, str_node)

void init_string(simple_c_string *array_c);

void destroy_string(simple_c_string *array_c);

void add_string(char const *in_data, simple_c_string *array_c);

void printf_string(simple_c_string *array_c);

char *get_string(int in_index, simple_c_string *array_c);

void dismantling_string(const char *in_data, const char *str_sub, simple_c_string *array_c);

char *dismantling_string_by_index(int index, const char *split_str, const char *sub_str, char *buf);

bool is_exist(char const *in_data, simple_c_string *array_c);