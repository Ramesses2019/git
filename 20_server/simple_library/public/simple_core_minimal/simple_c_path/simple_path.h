#pragma once
#include "../simple_c_core/simple_core_minimal.h"

//���ڲ���ȡ�ļ�·��
void get_path_directory_inline(char *path_buf);
//��ȡ�ļ�·��
void get_path_directory(char *buf, const char *path_buf);
//�õ��ļ���
void get_path_clean_filename(char *buf, const char *path_buf);
//��һ���ļ�·��  \\  /
void normalization_path(char *path_buf);
//��ʼ���ļ�·�� / \\��
void normalization_directory(char *buf, const char *path_buf);