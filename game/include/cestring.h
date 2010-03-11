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

#ifndef CE_STRING_H
#define CE_STRING_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct {
	int capacity;
	int length;
	char* str;
} ce_string;

extern ce_string* ce_string_new(void);
extern ce_string* ce_string_new_reserved(int capacity);
extern ce_string* ce_string_new_cstr(const char* cstr);
extern ce_string* ce_string_new_cstr_n(const char* cstr, int n);
extern void ce_string_del(ce_string* str);

extern void ce_string_reserve(ce_string* str, int capacity);

extern int ce_string_length(const ce_string* str);
extern bool ce_string_empty(const ce_string* str);

extern const char* ce_string_cstr(const ce_string* str);

extern int ce_string_cmp(const ce_string* str1, const ce_string* str2);
extern int ce_string_cmp_cstr(const ce_string* str1, const char* str2);

extern ce_string* ce_string_dup(const ce_string* str);
extern ce_string* ce_string_dup_n(const ce_string* str, int n);

extern void ce_string_assign(ce_string* str, const char* cstr);
extern void ce_string_assign_n(ce_string* str, const char* cstr, int n);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CE_STRING_H */
