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

#ifndef CE_MPRHLP_H
#define CE_MPRHLP_H

#include <stdbool.h>
#include <stdint.h>

#include "cevec3.h"
#include "ceaabb.h"
#include "cevector.h"
#include "cethread.h"
#include "cemmpfile.h"
#include "cemprfile.h"
#include "cematerial.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
	CE_MPRHLP_MMPFILE_VERSION = 1
};

extern const float CE_MPR_OFFSET_XZ_COEF;
extern const float CE_MPR_HEIGHT_Y_COEF;

extern ce_aabb* ce_mprhlp_get_aabb(ce_aabb* aabb, const ce_mprfile* mprfile,
									int sector_x, int sector_z, bool water);

static inline float* ce_mprhlp_normal2vector(float* vector, uint32_t normal)
{
	vector[0] = (((normal >> 11) & 0x7ff) - 1000.0f) / 1000.0f;
	vector[1] = (normal >> 22) / 1000.0f;
	vector[2] = ((normal & 0x7ff) - 1000.0f) / 1000.0f;
	return vector;
}

static inline int ce_mprhlp_texture_index(uint16_t texture)
{
	return texture & 0x003f;
}

static inline int ce_mprhlp_texture_number(uint16_t texture)
{
	return (texture & 0x3fc0) >> 6;
}

static inline int ce_mprhlp_texture_angle(uint16_t texture)
{
	return (texture & 0xc000) >> 14;
}

extern float ce_mprhlp_get_height(const ce_mprfile* mprfile, float x, float z);

extern ce_material*
ce_mprhlp_create_material(const ce_mprfile* mprfile, bool water);

extern ce_mmpfile* ce_mprhlp_generate_mmpfile(const ce_mprfile* mprfile,
	const ce_vector* tile_mmpfiles, int x, int z, bool water);

#ifdef __cplusplus
}
#endif

#endif /* CE_MPRHLP_H */