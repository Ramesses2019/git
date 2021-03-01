#pragma once
#include "../simple_c_core/simple_core_minimal.h"

//在内部获取文件路径
void get_path_directory_inline(char *path_buf);
//获取文件路径
void get_path_directory(char *buf, const char *path_buf);
//得到文件名
void get_path_clean_filename(char *buf, const char *path_buf);
//归一化文件路径  \\  /
void normalization_path(char *path_buf);
//初始化文件路径 / \\连
void normalization_directory(char *buf, const char *path_buf);