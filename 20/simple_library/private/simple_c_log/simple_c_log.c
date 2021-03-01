#include "../../public/simple_c_log/simple_c_log.h"
#include "../../public/simple_core_minimal/simple_c_windows/simple_c_windows_setting.h"
#include "../../public/simple_core_minimal/simple_c_core/simple_c_string_algorithm/string_algorithm.h"

char log_path[MAX_PATH] = { 0 };//储存路径
char log_filename[MAX_PATH] = { 0 };//具体文件

void init_log_system(const char *path)
{
	strcpy(path, log_path);
}

//获取日志
const char *get_log_filename()
{
	if (log_filename[0] == '\0')
	{
		if (log_path[0] == '\0')
		{
			set_consle_w_color(SIMPLE_RED, SIMPLE_BLACK);
			printf("\n You need to add the log path in init_log_system(...) first.  \n");
			//assert(0);
			set_consle_w_color(SIMPLE_WHITE, SIMPLE_BLACK);

			return NULL;
		}

		const char *p = log_path;

		char tmp_path[MAX_PATH] = { 0 };
		strcpy(tmp_path, p);
		strcat(tmp_path, "\\");
		_mkdir(tmp_path);

		//获取本地时间
		/*time_t now_time = time(NULL);
		struct tm *t_tm = localtime(&now_time);
		time_t mk_time = mktime(t_tm);*/

		//char buf[256] = { 0 };
		char *p_time = ctime(__TIME__);
		remove_char_end(p_time, '\n'); //移除末尾的'\n'
		remove_char_end(p_time, '\:'); //移除所有的':'
		const char file_suffix[] = ".txt";

		strcat(tmp_path, p_time, strlen(p_time));
		strcat(tmp_path, file_suffix, strlen(file_suffix));

		FILE *hFile = NULL;
		if ((hFile = fopen(tmp_path, "a+")) != NULL)
		{
			strcpy(log_filename, tmp_path);
			fclose(hFile);
		}
	}

	return log_filename;
}

const char * get_log_path()
{
	if (log_path[0] == '\0')
	{
		set_consle_w_color(SIMPLE_RED, SIMPLE_BLACK);
		printf("\n You need to add the log path in init_log_system(...) first.  \n");
		assert(0);
		set_consle_w_color(SIMPLE_WHITE, SIMPLE_BLACK);
	}

	return log_path;
}

bool log_write(enum e_error error, char *format, ...)
{
	char error_str[64] = { 0 };
	switch (error)
	{
	case SIMPLE_C_SUCCESS:
		strcpy(error_str, "SUCCESS");
		break;
	case SIMPLE_C_LOG:
		strcpy(error_str, "LOG");
		break;
	case SIMPLE_C_WARNING:
		strcpy(error_str, "WARNING");
		break;
	case SIMPLE_C_ERROR:
		strcpy(error_str, "ERROR");
		break;
	}

    const char *p = get_log_filename();

	if (p != NULL)
	{
		FILE *hfile = NULL;
		if ((hfile = fopen(p, "a+")) != NULL)
		{
			char buf[1024] = { 0 };
			ZeroMemory(buf, sizeof(char) * 1024);
			va_list args;
			va_start(args, format);
			_vsnprintf_s(buf, 1024 - 1, 1024, format, args);
			va_end(args);
			buf[1024 - 1] = 0;

			/*
			[SUCCESS] [12:23:34] 当前的操作成功\n
			*/

			char *p_time = get_current_time();
			remove_char_end(p_time, '\n'); //移除末尾的'\n'

			char text_buf[1024] = { 0 };
			get_printf(text_buf, "[%s] [%s] %s", error_str, p_time, buf);

			switch (error)
			{
			case SIMPLE_C_SUCCESS:
				set_consle_w_color(SIMPLE_PALE_GREEN, SIMPLE_BLACK);
				break;
			case SIMPLE_C_LOG:
				set_consle_w_color(SIMPLE_WHITE, SIMPLE_BLACK);
				break;
			case SIMPLE_C_WARNING:
				set_consle_w_color(SIMPLE_YELLOW, SIMPLE_BLACK);
				break;
			case SIMPLE_C_ERROR:
				set_consle_w_color(SIMPLE_RED, SIMPLE_BLACK);
				break;
			}

			//打印日志 并把日志输出到文件
			printf(text_buf);
			fprintf(hfile, text_buf);

			fclose(hfile);
		}
		return true;
	}
	return false;
}