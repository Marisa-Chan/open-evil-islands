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

#ifndef CE_VEC4_H
#define CE_VEC4_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct {
	float x, y, z, w;
} ce_vec4;

extern ce_vec4* ce_vec4_init(ce_vec4* vec, float x, float y, float z, float w);
extern ce_vec4* ce_vec4_init_array(ce_vec4* vec, const float* array);

extern ce_vec4* ce_vec4_copy(ce_vec4* vec, const ce_vec4* other);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CE_VEC4_H */