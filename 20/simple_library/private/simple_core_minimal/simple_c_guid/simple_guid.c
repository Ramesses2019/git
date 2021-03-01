#include "../../../public/simple_core_minimal/simple_c_guid/simple_guid.h"

//把单个字符传化为16进制数字
int hex_digit(char c)
{
	int ret = 0;
	if (c > '0'&&c <= '9')
	{
		ret = c - '0';
	}
	else if (c >= 'A'&&c <= 'F')
	{
		ret = 10 + c - 'A';
	}
	else if (c >= 'a'&&c <= 'f')
	{
		ret = 10 + c - 'a';
	}
	return ret;
}

unsigned int hex_number(char *buf)
{
	unsigned int ret = 0;

	while (*buf)
	{
		ret *= 16;
		ret += hex_digit(*buf++);
	}

	return ret;
}

void nomalization_guid(simple_c_guid *c_guid)
{
	c_guid->a = 0;
	c_guid->b = 0;
	c_guid->c = 0;
	c_guid->d = 0;
}

bool create_guid(simple_c_guid *c_guid)
{
	nomalization_guid(c_guid);
	return CoCreateGuid((GUID*)c_guid) == S_OK;
}

void guid_to_string(char *buf, const simple_c_guid *c_guid)
{
	get_printf_s(buf, "%08X%08X%08X%08X", c_guid->a, c_guid->b, c_guid->c, c_guid->d);
}

void string_to_guid(const char *buf, simple_c_guid *c_guid)
{
	if (strlen(buf) + 1 >= 32)
	{
		char byte_buf[MAX_PATH] = { 0 };
		c_guid->a = hex_number(string_mid(buf, byte_buf, 0, 8));
		c_guid->b = hex_number(string_mid(buf, byte_buf, 8, 8));
		c_guid->c = hex_number(string_mid(buf, byte_buf, 16, 8));
		c_guid->d = hex_number(string_mid(buf, byte_buf, 24, 8));
	}
}

bool guid_equal(const simple_c_guid *c_guid_a, const simple_c_guid *c_guid_b)
{
	return c_guid_a->a == c_guid_b->a&&
		c_guid_a->b == c_guid_b->b&&
		c_guid_a->c == c_guid_b->c&&
		c_guid_a->d == c_guid_b->d;
}

bool guid_equal_str(const char *guid_string, const simple_c_guid *c_guid)
{
	simple_c_guid id;
	string_to_guid(guid_string, &id);

	guid_equal(&id, c_guid);
}
