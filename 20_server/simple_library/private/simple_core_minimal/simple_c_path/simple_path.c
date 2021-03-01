#include "..\..\..\public\simple_core_minimal\simple_c_core\simple_c_array\simple_c_array_string.h"
#include "..\..\..\public\simple_core_minimal\simple_c_path\simple_path.h"

void get_path_directory_inline(char *path_buf)
{
	normalization_path(path_buf);

	char filename[260] = { 0 };
	get_path_clean_filename(filename, path_buf);

	//从指定的路径中移除文件名
	remove_string_start(path_buf, filename);
}

void get_path_directory(char *buf, const char *path_buf)
{
	strcpy(buf, path_buf);
	get_path_directory_inline(buf);
}

void get_path_clean_filename(char *buf, const char *path_buf)
{
	simple_c_string c_string;
	dismantling_string(path_buf, "/", &c_string);
	char *value = get_string(c_string.size - 1, &c_string);
	strcpy(buf, value);
	destroy_string(&c_string);
}

void normalization_path(char *path_buf)
{
	replace_char_inline(path_buf,'\\', '/');
}

void normalization_directory(char *buf, const char *path_buf)
{
	simple_c_string c_string;
	dismantling_string(path_buf, "/", &c_string);

	for (int i = 0; i < c_string.size - 1; i++)
	{
		char buf_tmp[260] = { 0 };
		char *value = get_string(i, &c_string);

		strcpy(buf_tmp, value);
		strcat(buf_tmp, "\\");
		strcat(buf, buf_tmp);
	}

	destroy_string(&c_string);
}
