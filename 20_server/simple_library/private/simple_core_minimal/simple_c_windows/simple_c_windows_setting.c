#include "..\..\..\public\simple_core_minimal\simple_c_windows\simple_c_windows_setting.h"

//set_consle_windows_color
void set_consle_w_color(simple_consle_w_color font_color, simple_consle_w_color background_color)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, (unsigned short)font_color + (unsigned short)background_color * 0x10);
}