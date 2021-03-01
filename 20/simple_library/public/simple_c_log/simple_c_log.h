#pragma once
#include "../simple_core_minimal/simple_c_core/simple_core_minimal.h"

//»’÷æ
///////////////////////////////////////////////////////////
typedef enum e_error
{
	SIMPLE_C_SUCCESS = 0,
	SIMPLE_C_LOG,
	SIMPLE_C_WARNING,
	SIMPLE_C_ERROR,
};
_CRT_BEGIN_C_HEADER

#define log_success(...) log_write(SIMPLE_C_SUCCESS,__VA_ARGS__)
#define log_log(...) log_write(SIMPLE_C_LOG,__VA_ARGS__)
#define log_warning(...) log_write(SIMPLE_C_WARNING,__VA_ARGS__)
#define log_error(...) log_write(SIMPLE_C_ERROR,__VA_ARGS__)

void init_log_system(const char *path);

const char *get_log_path();
const char *get_log_filename();
bool log_write(enum e_error error, char *format, ...);

_CRT_END_C_HEADER