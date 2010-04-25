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

#ifndef CE_OCCLUSION_H
#define CE_OCCLUSION_H

#include <stdbool.h>

#include "cebbox.h"
#include "cerendersystem.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct ce_occlusion ce_occlusion;

extern ce_occlusion* ce_occlusion_new(void);
extern void ce_occlusion_del(ce_occlusion* occlusion);

extern bool ce_occlusion_query(ce_occlusion* occlusion,
	const ce_bbox* bbox, ce_rendersystem* rendersystem);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CE_OCCLUSION_H */
