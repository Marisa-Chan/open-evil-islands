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

#ifndef CE_FIGPROTO_H
#define CE_FIGPROTO_H

#include <stdbool.h>

#include "cestring.h"
#include "cevector.h"
#include "ceresfile.h"
#include "cefigfile.h"
#include "cebonfile.h"
#include "ceanmfile.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct {
	ce_string* name;
	bool has_morphing;
	ce_figfile* figfile;
	ce_bonfile* bonfile;
	ce_vector* anmfiles;
	ce_vector* child_nodes;
} ce_figproto_node;

typedef struct {
	ce_string* name;
	ce_figproto_node* root_node;
	int ref_count;
} ce_figproto;

extern ce_figproto* ce_figproto_new(const char* figure_name,
									ce_resfile* resfile);
extern void ce_figproto_del(ce_figproto* figproto);

extern ce_figproto* ce_figproto_copy(ce_figproto* figproto);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CE_FIGPROTO_H */