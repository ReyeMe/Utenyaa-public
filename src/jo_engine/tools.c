/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2020, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
** INCLUDES
*/
#include <stdbool.h>
#include "jo/sgl_prototypes.h"
#include "jo/conf.h"
#include "jo/types.h"
#include "jo/sega_saturn.h"
#include "jo/smpc.h"
#include "jo/time.h"
#include "jo/core.h"
#include "jo/tools.h"
#include "jo/math.h"
#include "jo/colors.h"
#include "jo/image.h"
#include "jo/vdp2.h"

/*
** GLOBALS
*/
char			            __jo_sprintf_buf[JO_PRINTF_BUF_SIZE];

#ifdef JO_COMPILE_WITH_PRINTF_SUPPORT

#if !JO_COMPILE_USING_SGL

unsigned char               __jo_printf_current_palette_index = JO_COLOR_INDEX_Black;

static unsigned char __jo_font[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E,
    0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E, 0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00,
    0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00, 0x38, 0x7C, 0x38, 0xFE, 0xFE, 0x7C, 0x38, 0x7C,
    0x10, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x7C, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00,
    0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF, 0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00,
    0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF, 0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78,
    0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18, 0x3F, 0x33, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0,
    0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0, 0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99,
    0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00, 0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00,
    0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00,
    0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00, 0x3E, 0x63, 0x38, 0x6C, 0x6C, 0x38, 0xCC, 0x78,
    0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF,
    0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00,
    0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00,
    0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00, 0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00,
    0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x78, 0x78, 0x78, 0x30, 0x00, 0x30, 0x00,
    0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00,
    0x30, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x30, 0x00, 0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00,
    0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00, 0x60, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00, 0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,
    0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00,
    0x7C, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0x7C, 0x00, 0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00,
    0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00, 0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00,
    0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00, 0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00,
    0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00, 0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00,
    0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00, 0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00,
    0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60,
    0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0xFC, 0x00, 0x00,
    0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x78, 0xCC, 0x0C, 0x18, 0x30, 0x00, 0x30, 0x00,
    0x7C, 0xC6, 0xDE, 0xDE, 0xDE, 0xC0, 0x78, 0x00, 0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00,
    0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00, 0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00,
    0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x7E, 0x00,
    0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00, 0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3E, 0x00,
    0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00, 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00, 0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00,
    0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00,
    0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00,
    0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0xDC, 0x78, 0x1C, 0x00,
    0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00, 0x78, 0xCC, 0xE0, 0x70, 0x1C, 0xCC, 0x78, 0x00,
    0xFC, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00,
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0x00,
    0xC6, 0xC6, 0x6C, 0x38, 0x38, 0x6C, 0xC6, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00,
    0xFE, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xFE, 0x00, 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,
    0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00,
    0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
    0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00,
    0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
    0x38, 0x6C, 0x60, 0xF0, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
    0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00, 0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00,
    0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xC6, 0x00,
    0x00, 0x00, 0xF8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00,
    0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E,
    0x00, 0x00, 0xDC, 0x76, 0x66, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x00,
    0x10, 0x30, 0x7C, 0x30, 0x30, 0x34, 0x18, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
    0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, 0x00, 0x00, 0xC6, 0xD6, 0xFE, 0xFE, 0x6C, 0x00,
    0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
    0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00, 0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00,
    0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00, 0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00,
    0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00
};

static              void jo_putchar(const int x, const int y, const char v, const jo_color text_color)
{
    unsigned char   b;
    int             r;
    int             c;

    for (r = 0; r < 8; ++r)
    {
        b = __jo_font[JO_MULT_BY_8(v) | r];
        for (c = 0; c < 8; c++)
            jo_vdp2_put_pixel_bitmap_nbg1(((x * 8) | c), ((y * 8) | r), (b & (0x80 >> c) ? text_color : JO_COLOR_Transparent));
    }
}

#endif

void            jo_print(int x, int y, char * str)
{
#ifdef JO_DEBUG
    if (str == JO_NULL)
    {
        jo_core_error("str is null");
        return;
    }
#endif
#if JO_COMPILE_USING_SGL
    slPrint(str, slLocate((x), (y)));
#else
    int         i;
    jo_color    text_color;

    text_color = *(((unsigned short *)JO_VDP2_CRAM) + 1 + JO_MULT_BY_256(__jo_printf_current_palette_index));
    for (JO_ZERO(i); str[i]; ++i)
        jo_putchar(x + i, y, str[i], text_color);
#endif
}

void                    jo_clear_screen_line(const int y)
{
    int        x;

    for (JO_ZERO(x); x < 40; ++x)
    {
#if JO_COMPILE_USING_SGL
        slPrint(" ", slLocate(x, y));
#else
        jo_putchar(x, y, ' ', JO_COLOR_Transparent);
#endif
    }
}

void                    jo_clear_screen(void)
{
    int        y;

    for (JO_ZERO(y); y < 30; ++y)
        jo_clear_screen_line(y);
}

#endif // JO_COMPILE_WITH_PRINTF_SUPPORT

#if JO_COMPILE_USING_SGL

void                    jo_getdate(jo_datetime *now)
{
    SmpcDateTime        *time;
#ifdef JO_DEBUG
   if (now == JO_NULL)
   {
	 	jo_core_error("now is null");
	 	return;
   }
#endif
	slGetStatus();
    time = &(Smpc_Status->rtc);
    now->year = (unsigned short)slDec2Hex(time->year);
    now->month = (unsigned char)(time->month & 0x0f);
    now->week = (unsigned char)(time->month >> 4);
    now->day = (unsigned char)slDec2Hex(time->date);
    now->hour = (unsigned char)slDec2Hex(time->hour);
    now->minute = (unsigned char)slDec2Hex(time->minute);
    now->second = (unsigned char)slDec2Hex(time->second);
}

#else

void                        jo_getdate(jo_datetime *now)
{
#ifdef JO_DEBUG
    if (now == JO_NULL)
    {
        jo_core_error("now is null");
        return;
    }
#endif

    jo_smpc_begin_command();
    jo_smpc_write_byte(InputRegister0, 0x01);
    jo_smpc_write_byte(InputRegister1, 0x08);
    jo_smpc_write_byte(InputRegister2, 0xF0);
    jo_smpc_end_command(FetchStatusAndPeripheralData);
    now->day = (JO_BCD_INT(jo_smpc_read_byte(OutputRegister4) & 0xF0)) +
               (JO_BCD_INT(jo_smpc_read_byte(OutputRegister4) & 0x0F));
    now->month = (JO_BCD_INT(jo_smpc_read_byte(OutputRegister3) & 0x0F));
    now->year = (JO_BCD_INT(jo_smpc_read_byte(OutputRegister1)& 0x0F)) * 1000 +
                (JO_BCD_INT(jo_smpc_read_byte(OutputRegister1) & 0xF0)) * 100 +
                (JO_BCD_INT(jo_smpc_read_byte(OutputRegister2)& 0x0F)) +
                (JO_BCD_INT(jo_smpc_read_byte(OutputRegister2) & 0xF0));
    now->week = (unsigned char)(now->month & 0x0F); //FIX ME
    now->hour = JO_BCD_INT(jo_smpc_read_byte(OutputRegister5));
    now->minute = JO_BCD_INT(jo_smpc_read_byte(OutputRegister6));
    now->second = JO_BCD_INT(jo_smpc_read_byte(OutputRegister7));
}

#endif

jo_language     jo_get_current_language(void)
{
#if JO_COMPILE_USING_SGL
    switch (slGetLanguage())
    {
    case SMPC_ITALIANO:
        return Italiano;
    case SMPC_DEUTSCH:
        return Deutsch;
    case SMPC_JAPAN:
        return Japanese;
    case SMPC_FRANCAIS:
        return French;
    case SMPC_ESPANOL:
        return Espanol;
    default:
        return English;
    }
#else
    return English;
#endif
}

/*
** This is a fast unsafe(doesn't check JO_NULL param, etc.) implementation of atoi
*/
inline int				jo_tools_atoi(const char * str)
{
    int					val;
    bool		        is_negative;

    JO_ZERO(val);
    is_negative = false;
#ifdef JO_DEBUG
    if (str == JO_NULL)
    {
        jo_core_error("str is null");
        return (0);
    }
#endif
    if (*str == '-')
    {
        ++str;
        is_negative = true;
    }
    while (*str)
        val = val * 10 + (*str++ - '0');
    if (is_negative)
        return (-val);
    return (val);
}

inline int				jo_strlen(const char * str)
{
    char                *begin;

#ifdef JO_DEBUG
    if (str == JO_NULL)
    {
        jo_core_error("str is null");
        return (0);
    }
#endif
    for (begin = (char *)str; *str; ++str)
        ;
    return (str - begin);
}

inline bool             jo_endwith(const char * str, const char * end)
{
    int                 str_len;
    int                 end_len;
    int        i;

#ifdef JO_DEBUG
    if (str == JO_NULL)
    {
        jo_core_error("str is null");
        return (0);
    }
    if (end == JO_NULL)
    {
        jo_core_error("end is null");
        return (0);
    }
#endif
    str_len = jo_strlen(str);
    end_len = jo_strlen(end);
    if (str_len < end_len)
        return (false);
    str += (str_len - end_len);
    for (JO_ZERO(i); i < end_len; ++i)
    {
        if (str[i] != end[i])
            return (false);
    }
    return (true);
}

int                                 jo_strcmp(const char * p1, const char * p2)
{
    register const unsigned char    *s1 = (const unsigned char *)p1;
    register const unsigned char    *s2 = (const unsigned char *)p2;
    unsigned char                   c1;
    unsigned char                   c2;

    do
    {
        c1 = (unsigned char)*s1++;
        c2 = (unsigned char)*s2++;
        if (!c1)
            return (c1 - c2);
    }
    while (c1 == c2);
    return (c1 - c2);
}

void                        jo_memset(const void * const ptr, const int value, unsigned int num)
{
    register unsigned int   * p;
    register unsigned char  * pp;
    unsigned int            tail;
    unsigned int            x;
    unsigned char           xx;
	
    if(num == 0)
        return;

    x = value & 0xff;
    xx = value & 0xff;
    pp = (unsigned char*)ptr;
    while (((unsigned int)pp & 3) && num--)
        *pp++ = xx;
    p = (unsigned int*)pp;
    tail = num & 3;
    x |= x << 8;
    x |= x << 16;
    num >>= 2;
    while (num--)
        *p++ = x;
    pp = (unsigned char*)p;
    while (tail--)
        *pp++ = xx;
}

void                    jo_map_to_vram(unsigned short * data, unsigned short * vram_addr,
                                       unsigned short suuj, unsigned short suui, unsigned short palnum, unsigned int mapoff)
{
    unsigned short      j;
    unsigned short      paloff;

    paloff = palnum << 12;
    while (suui-- > 0)
    {
        for (JO_ZERO(j); j < suuj; ++j, ++data)
            *vram_addr++ = (*data | paloff) + mapoff;
        vram_addr += (64 - suuj);
    }
}

/*
** END OF FILE
*/
