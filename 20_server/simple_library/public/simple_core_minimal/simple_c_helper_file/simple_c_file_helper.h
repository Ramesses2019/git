#pragma once
#include "../simple_c_core/simple_core_minimal.h"

typedef struct
{
	int index;
	char paths[1024][260];  
}def_paths;

void init_def_paths(def_paths *c_paths);

bool copy_file(const char * src, const char *dest);

//参数3表示是否可以递归
void find_files(char const *in_path, def_paths *str,bool b_recursion);

bool create_file(char const *filename);

bool create_file_directory(char const *in_path);

bool get_file_buf(const char *path, char *buf);

bool add_file_buf(const char *path, char *buf);

bool add_new_file_buf(const char *path, char *buf);

unsigned int get_file_size_by_filename(const char *filename);

unsigned int get_file_size(FILE *file_handle);