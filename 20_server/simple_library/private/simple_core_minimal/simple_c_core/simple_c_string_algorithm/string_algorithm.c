#include "..\..\..\..\public\simple_core_minimal\simple_c_core\simple_c_array\simple_c_array_string.h"
#include "..\..\..\..\public\simple_core_minimal\simple_c_core\simple_c_string_algorithm\string_algorithm.h"

//从头部移除单个字符
void remove_char_start(char *str, char sub_str)
{
	int len = strlen(str) + 1;

	//从头开始检查 然后删除对象
	for (int i = 0; i < len; i++)
	{
		if (str[i] == sub_str)
		{
			do
			{
				str[i] = str[i + 1];
				i++;
			} while (str[i + 1] != '\0');
			str[i] = '\0';

			break;
		}
	}
}

//从尾部移除单个字符
void remove_char_end(char *str, char sub_str)
{
	int len = strlen(str);

	for (int i = len; i >= 0; i--)
	{
		if (str[i] == sub_str)
		{
			strcpy(&str[i], &str[i + 1]);
			break;
		}
	}
}

//从尾部移除所有的sub_str字符
void remove_all_char_end(char *str, char sub_str)
{
	int len = strlen(str);

	for (int i = len; i >= 0; i--)
	{
		if (str[i] == sub_str)
		{
			strcpy(&str[i], &str[i + 1]);
		}
	}
}

//从内部把字符串的所有a字符替换为b字符
void replace_char_inline(char *str, const char sub_char_a, const char sub_char_b)
{
	int index = find_string(str, &sub_char_a);
	if (index != -1)
	{
		str[index] = sub_char_b;
		//递归的找到每一次最先出现的字符a并替换为字符b
		replace_char_inline(str, sub_char_a, sub_char_b);
	}
}

int find_string(char *str, char const* sub_str)
{
	int index = -1;
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (sub_str[0] == str[i])
		{
			int j = 0;
			for (; j < strlen(sub_str); j++)
			{
				//排除末尾的干扰 保证两者相等
				if (sub_str[j] != str[j + i] && 
					sub_str[j] != '\0')
				{
					break;
				}
			}

			//如果相等说明找到了sub_str长度的字符串
			if (j == strlen(sub_str))
			{
				index = i;
				break;
			}
		}
	}
	return index;
}

void remove_string_start(char *str, char const* sub_str)
{
	int index = find_string(str, sub_str);
	if (index != -1)
	{
		//把sub_str长度的地址移动到index处（覆盖了str中的sub_str字符串）
		strcpy(&str[index], &str[index + strlen(sub_str)]);
	}
}

int get_printf_s(_Out_ char* out_buf, _In_z_ _Printf_format_string_ char *format, ...)
{
	char buf[1024] = { 0 };
	memset(buf, 0, sizeof(char) * 1024);
	va_list args;
	va_start(args, format);
	_vsnprintf_s(buf, 1024 - 1, 1024, format, args);
	va_end(args);
	buf[1024 - 1] = 0;

	strcpy(out_buf, buf);
	return strlen(out_buf) + 1;
}

int get_printf(char *buf, char *format, ...)
{
	va_list param_list;
	//va_start 获取可变参数列表的第一个参数的地址
	va_start(param_list, format);
	//先用变量接收第一个字符值 再把指针向后移动一位
	char param_char = *format++;

	for (int i = 0; param_char != '\0'; i++)
	{
		//不是%或前一位也不能是%，才进行该字符的记录 例：%s %S
		if (param_char != '\%'&& *(format - 1) != '\%')
		{
			buf[i] = param_char;
		}
		else
		{
			//往%之后移动一位判断是哪种方式的
			param_char = *format++;
			switch (param_char)
			{
			case 'c':
			case 'C':
			{
				//va_arg宏，获取可变参数的当前参数，返回指定类型并将指针指向下一参数
				buf[i] = va_arg(param_list, char);
				break;
			}
			case 's':
			case 'S':
			{
				char *p = va_arg(param_list, char*);
				for (int j = 0; j < strlen(p);)
				{
					buf[i++] = p[j++];
				}
				i--; //排除字符串末尾\0的干扰
				break;
			}
			case 'i':
			case 'I':
			case 'd':
			case 'D':
			{
				int new_int = va_arg(param_list, int);
				char buf_int[8] = { 0 };
				char *p = _itoa(new_int, buf_int, 10);
				int len = strlen(p);
				for (int j = 0; j < len;)
				{
					buf[i++] = p[j++];
				}
				i--;
				break;
			}
			}
		}

		param_char = *format++;
	}
	return strlen(buf) + 1;
}

char * string_mid(_In_ const char *mid_buf, _Out_ char *out_buf, int start, int count)
{
	char *p = &mid_buf[start];
	memcpy_s(out_buf, count, p, count);

	return out_buf;
}

