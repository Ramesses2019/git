#pragma once
#include "../simple_c_core/simple_core_minimal.h"

typedef enum
{
	SIMPLE_BLACK = 0,               //黑色
	SIMPLE_BLUE,                    //蓝色
	SIMPLE_GREEN,                   //绿色
	SIMPLE_LIGHT_BLUE,				// 浅蓝色	
	SIMPLE_RED,						// 红色		
	SIMPLE_VIOLET,					// 紫色		
	SIMPLE_YELLOW,					// 黄色		
	SIMPLE_WHITE,					// 白色		
	SIMPLE_GREY,					// 灰色
	SIMPLE_PALE_BLUE,				// 淡蓝色
	SIMPLE_PALE_GREEN,				// 淡绿色
	SIMPLE_LIGHT_GREEN,				// 淡浅绿色
	SIMPLE_PALE_RED,				// 淡红色
	SIMPLE_LAVENDER,				// 淡紫色
	SIMPLE_CANARY_YELLOW,			// 淡黄色
	SIMPLE_BRIGHT_WHITE,			// 亮白色
}simple_consle_w_color;

void set_consle_w_color(simple_consle_w_color font_color, simple_consle_w_color background_color);