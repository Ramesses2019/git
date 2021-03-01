#pragma once
#include "../simple_c_core/simple_core_minimal.h"

typedef struct 
{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
}simple_c_guid;

void nomalization_guid(simple_c_guid *c_guid);

bool create_guid(simple_c_guid *c_guid);

void guid_to_string(char *buf, const simple_c_guid *c_guid);

void string_to_guid(const char *buf, simple_c_guid *c_guid);

bool guid_equal(const simple_c_guid *c_guid_a, const simple_c_guid *c_guid_b);

bool guid_equal_str(const char *guid_string, const simple_c_guid *c_guid);