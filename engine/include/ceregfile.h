/*
 *  This file is part of Cursed Earth
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands
 *  Copyright (C) 2009-2010 Yanis Kurganov
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

/*
 *  See also:
 *  1. doc/formats/regfile.txt
*/

#ifndef CE_REGFILE_H
#define CE_REGFILE_H

#include <stdint.h>

#include "cevector.h"
#include "ceobject.h"
#include "cememfile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	ce_vector* sections;
} ce_reg_file;

extern ce_reg_file* ce_reg_file_new(ce_mem_file* mem_file);
extern void ce_reg_file_del(ce_reg_file* reg_file);

extern ce_value* ce_reg_file_find(ce_reg_file* reg_file,
									const char* section_name,
									const char* option_name,
									size_t index);

#ifdef __cplusplus
}
#endif

#endif /* CE_REGFILE_H */