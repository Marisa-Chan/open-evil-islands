/*
 *  This file is part of Cursed Earth.
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands.
 *  Copyright (C) 2009-2010 Yanis Kurganov.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "cemath.h"
#include "cecolor.h"

const ce_color CE_COLOR_BLACK = { .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
const ce_color CE_COLOR_WHITE = { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
const ce_color CE_COLOR_RED = { .r = 1.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
const ce_color CE_COLOR_GREEN = { .r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f };
const ce_color CE_COLOR_BLUE = { .r = 0.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f };
const ce_color CE_COLOR_YELLOW = { .r = 1.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f };
const ce_color CE_COLOR_CYAN = { .r = 0.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
const ce_color CE_COLOR_MAGENTA = { .r = 1.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f };
const ce_color CE_COLOR_GRAY = { .r = 160.0f / 255.0f, .g = 160.0f / 255.0f,
									.b = 164.0f / 255.0f, .a = 1.0f };
const ce_color CE_COLOR_CORNFLOWER = { .r = 100.0f / 255.0f, .g = 149.0f / 255.0f,
										.b = 237.0f / 255.0f, .a = 1.0f };
const ce_color CE_COLOR_CRIMSON = { .r = 220.0f / 255.0f, .g = 20.0f / 255.0f,
									.b = 60.0f / 255.0f, .a = 1.0f };
const ce_color CE_COLOR_GOLD = { .r = 1.0f, .g = 215.0f / 255.0f, .b = 0.0f, .a = 1.0f };
const ce_color CE_COLOR_GOLDENROD = { .r = 218.0f / 255.0f, .g = 165.0f / 255.0f,
										.b = 32.0f / 255.0f, .a = 1.0f };
const ce_color CE_COLOR_INDIGO = { .r = 75.0f / 255.0f, .g = 0.0f,
									.b = 130.0f / 255.0f, .a = 1.0f };
const ce_color CE_COLOR_ROYAL_BLUE = { .r = 65.0f / 255.0f, .g = 105.0f / 255.0f,
										.b = 1.0f, .a = 1.0f };
const ce_color CE_COLOR_SILVER = { .r = 192.0f / 255.0f, .g = 192.0f / 255.0f,
									.b = 192.0f / 255.0f, .a = 1.0f };
const ce_color CE_COLOR_TOMATO = { .r = 1.0f, .g = 99.0f / 255.0f,
									.b = 71.0f / 255.0f, .a = 1.0f };
const ce_color CE_COLOR_WHEAT = { .r = 245.0f / 255.0f, .g = 222.0f / 255.0f,
									.b = 179.0f / 255.0f, .a = 1.0f };
const ce_color CE_COLOR_TRANSPARENT;

ce_color* ce_color_init(ce_color* color, float r, float g, float b, float a)
{
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;
	return color;
}

ce_color* ce_color_init_array(ce_color* color, const float* array)
{
	color->r = *array++;
	color->g = *array++;
	color->b = *array++;
	color->a = *array;
	return color;
}

ce_color* ce_color_init_black(ce_color* color)
{
	color->r = 0.0f;
	color->g = 0.0f;
	color->b = 0.0f;
	color->a = 1.0f;
	return color;
}

ce_color* ce_color_init_white(ce_color* color)
{
	color->r = 1.0f;
	color->g = 1.0f;
	color->b = 1.0f;
	color->a = 1.0f;
	return color;
}

ce_color* ce_color_init_red(ce_color* color)
{
	color->r = 1.0f;
	color->g = 0.0f;
	color->b = 0.0f;
	color->a = 1.0f;
	return color;
}

ce_color* ce_color_init_green(ce_color* color)
{
	color->r = 0.0f;
	color->g = 1.0f;
	color->b = 0.0f;
	color->a = 1.0f;
	return color;
}

ce_color* ce_color_init_blue(ce_color* color)
{
	color->r = 0.0f;
	color->g = 0.0f;
	color->b = 1.0f;
	color->a = 1.0f;
	return color;
}

ce_color* ce_color_copy(ce_color* color, const ce_color* other)
{
	color->r = other->r;
	color->g = other->g;
	color->b = other->b;
	color->a = other->a;
	return color;
}

ce_color* ce_color_lerp(ce_color* color, float u, const ce_color* lhs,
													const ce_color* rhs)
{
	color->r = ce_lerp(u, lhs->r, rhs->r);
	color->g = ce_lerp(u, lhs->g, rhs->g);
	color->b = ce_lerp(u, lhs->b, rhs->b);
	color->a = ce_lerp(u, lhs->a, rhs->a);
	return color;
}
