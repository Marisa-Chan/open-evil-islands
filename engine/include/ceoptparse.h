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

#ifndef CE_OPTPARSE_H
#define CE_OPTPARSE_H

#include "cevector.h"
#include "ceobject.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct {
	ce_vector* objects;
} ce_optparse;

extern ce_optparse* ce_optparse_new(void);
extern void ce_optparse_del(ce_optparse* optparse);

extern void ce_optparse_add(ce_optparse* optparse, const char* name,
													const char* shortopt,
													const char* longopt,
													const char* glossary,
													void* defaultval);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CE_OPTPARSE_H */
