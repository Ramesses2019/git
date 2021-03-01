#include "..\..\..\public\simple_core_minimal\simple_c_core\simple_c_array\simple_c_array_string.h"
#include "..\..\..\public\simple_core_minimal\simple_c_helper_file\simple_c_file_helper.h"

void init_def_paths(def_paths *c_paths)
{
	c_paths->index = 0;
	memset(c_paths->paths, 0, sizeof(c_paths->paths) - 1);
}

bool copy_file(const char * src, const char *dest)
{
	FILE *f_src = NULL;
	FILE *f_dest = NULL;
	char buf[1024 * 1024] = { 0 };
	int file_size = 0;
	if ((f_src = fopen(src, "rb")) != NULL)
	{
		if ((f_dest = fopen(dest, "wb")) != NULL)
		{
			while ((file_size = fread(buf, sizeof(char), sizeof(buf), f_src)) > 0)
			{
				fwrite(buf, sizeof(char), file_size, f_dest);
				memset(buf, 0, sizeof(buf));
			}
		}
		fclose(f_dest);
		fclose(f_src);

		return true;
	}
	return false;
}

void find_files(char const *in_path, def_paths *str, bool b_recursion)
{
	struct _finddata_t finddata;
	long hfile = 0;   //句柄
	char tmp_path[8196] = { 0 };//存放临时的地址
	strcpy(tmp_path, in_path);
	strcat(tmp_path, "\\*"); //通配符 遍历整个文件夹使用

	//寻找文件 将第一个文件放入到句柄中
	if ((hfile = _findfirst(tmp_path, &finddata)) != -1)
	{
		do
		{
			//判断该文件是不是有子文件
			if (finddata.attrib & _A_SUBDIR)
			{
				if (b_recursion)
				{
					//排除掉这级的文件夹和上一级文件夹的干扰
					if (strcmp(finddata.name, ".") == 0 ||
						strcmp(finddata.name, "..") == 0)
					{
						continue;
					}

					char new_path[8196] = { 0 };
					strcpy(new_path, in_path);
					strcat(new_path, "\\");
					strcat(new_path, finddata.name);

					//递归的查找该路径下的文件
					find_files(new_path, str, b_recursion);
				}
			}
			else
			{
				strcpy(str->paths[str->index], in_path);
				strcat(str->paths[str->index], "\\");
				strcat(str->paths[str->index++], finddata.name);
			}
		} while (_findnext(hfile, &finddata) == 0);
		_findclose(hfile);
	}
}

bool create_file(char const *filename)
{
	FILE *f = NULL;
	if ((f = fopen(filename, "w+")) != NULL)
	{
		fclose(f);

		return true;
	}
	return false;
}

bool create_file_directory(char const *in_path)
{
	simple_c_string c_file;
	if (strstr(in_path, "\\"))
	{
		dismantling_string(in_path, "\\", &c_file);
	}
	else if (strstr(in_path, "/"))
	{
		dismantling_string(in_path, "/", &c_file);
	}

	char *path[260] = { 0 };
	for (int i = 0; i < c_file.size; i++)
	{
		char *value = get_string(i, &c_file);
		strcat(value, "\\");
		strcat(path, value);
		if (_access(path, 0) == -1)
		{
			_mkdir(path);
		}
	}

	destroy_string(&c_file);

	//等于0表示最后一个文件存在 即创建成功
	return _access(path, 0) == 0;
}

bool get_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "r")) != NULL)
	{
		char buf_tmp[512] = { 0 };
		int file_size = 0;
		while ((file_size = fread(buf_tmp, 1, 512, f)) > 0)
		{
			strcat(buf, buf_tmp);
			memset(buf_tmp, 0, 512);
		}

		fclose(f);

		return buf[0] != '\0';
	}

	return false;
}

bool add_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "a+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool add_new_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "w+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

unsigned int get_file_size_by_filename(const char *filename)
{
	unsigned int file_size = 0;

	FILE *f = NULL;
	if ((f = fopen(filename, "a+")) != NULL)
	{
		file_size = get_file_size(f);

		fclose(f);
	}

	return file_size;
}

unsigned int get_file_size(FILE *file_handle)
{
	unsigned int file_size = 0;

	unsigned int current_read_position = ftell(file_handle);
	fseek(file_handle, 0, SEEK_END);
	file_size = ftell(file_handle);
	fseek(file_handle, current_read_position, SEEK_SET);

	return file_size;
}
