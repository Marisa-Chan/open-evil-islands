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

#ifndef CE_SPHERE_H
#define CE_SPHERE_H

#include "cevec3.h"
#include "cequat.h"
#include "ceaabb.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct {
	ce_vec3 center;
	float radius;
} ce_sphere;

extern ce_sphere* ce_sphere_init(ce_sphere* sphere,
									const ce_vec3* center,
									float radius);
extern ce_sphere* ce_sphere_init_aabb(ce_sphere* sphere,
										const ce_aabb* aabb);

extern ce_sphere* ce_sphere_init_zero(ce_sphere* sphere);

extern ce_sphere* ce_sphere_copy(ce_sphere* sphere, const ce_sphere* other);

extern ce_sphere* ce_sphere_merge(ce_sphere* sphere,
									const ce_sphere* lhs,
									const ce_sphere* rhs);

extern ce_sphere* ce_sphere_transform(ce_sphere* sphere,
										const ce_sphere* other,
										const ce_vec3* translation,
										const ce_quat* rotation);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CE_SPHERE_H */