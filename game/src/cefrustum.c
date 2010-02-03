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

/*
 *  Based on http://www.lighthouse3d.com/opengl/viewfrustum/.
*/

#include <stddef.h>
#include <math.h>

#include "cemath.h"
#include "cefrustum.h"

static vec3* get_box_vertex_positive(vec3* p, const vec3* n, const ce_aabb* b)
{
	p->x = n->x > 0.0f ? b->max.x : b->min.x;
	p->y = n->y > 0.0f ? b->max.y : b->min.y;
	p->z = n->z > 0.0f ? b->max.z : b->min.z;
	return p;
}

ce_frustum* ce_frustum_init(ce_frustum* f, float fov, float aspect,
							float near, float far,
							const vec3* eye, const vec3* forward,
							const vec3* right, const vec3* up)
{
	float tang = tanf(0.5f * ce_deg2rad(fov));
	float nh = tang * near;
	float nw = nh * aspect;
	float fh = tang * far;
	float fw = fh * aspect;

	vec3 nc, fc, xw, yh;
	vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;

	vec3_add(eye, vec3_scale(forward, near, &nc), &nc);
	vec3_add(eye, vec3_scale(forward, far, &fc), &fc);

	vec3_scale(right, nw, &xw);
	vec3_scale(up, nh, &yh);

	vec3_sub(vec3_add(&nc, &yh, &ntl), &xw, &ntl);
	vec3_add(vec3_add(&nc, &yh, &ntr), &xw, &ntr);
	vec3_sub(vec3_sub(&nc, &yh, &nbl), &xw, &nbl);
	vec3_add(vec3_sub(&nc, &yh, &nbr), &xw, &nbr);

	vec3_scale(right, fw, &xw);
	vec3_scale(up, fh, &yh);

	vec3_sub(vec3_add(&fc, &yh, &ftl), &xw, &ftl);
	vec3_add(vec3_add(&fc, &yh, &ftr), &xw, &ftr);
	vec3_sub(vec3_sub(&fc, &yh, &fbl), &xw, &fbl);
	vec3_add(vec3_sub(&fc, &yh, &fbr), &xw, &fbr);

	ce_plane_init_tri(&f->p[CE_FRUSTUM_PLANE_TOP], &ntr, &ntl, &ftl);
	ce_plane_init_tri(&f->p[CE_FRUSTUM_PLANE_BOTTOM], &nbl, &nbr, &fbr);
	ce_plane_init_tri(&f->p[CE_FRUSTUM_PLANE_LEFT], &ntl, &nbl, &fbl);
	ce_plane_init_tri(&f->p[CE_FRUSTUM_PLANE_RIGHT], &nbr, &ntr, &fbr);
	ce_plane_init_tri(&f->p[CE_FRUSTUM_PLANE_NEAR], &ntl, &ntr, &nbr);
	ce_plane_init_tri(&f->p[CE_FRUSTUM_PLANE_FAR], &ftr, &ftl, &fbl);

	return f;
}

bool ce_frustum_test_point(const ce_frustum* f, const vec3* p)
{
	for (size_t i = 0; i < CE_FRUSTUM_PLANE_COUNT; ++i) {
		if (ce_plane_dist(&f->p[i], p) < 0.0f) {
			return false;
		}
	}
	return true;
}

bool ce_frustum_test_sphere(const ce_frustum* f, const vec3* p, float r)
{
	for (size_t i = 0; i < CE_FRUSTUM_PLANE_COUNT; ++i) {
		if (ce_plane_dist(&f->p[i], p) < -r) {
			return false;
		}
	}
	return true;
}

bool ce_frustum_test_box(const ce_frustum* f, const ce_aabb* b)
{
	vec3 p;
	for (size_t i = 0; i < CE_FRUSTUM_PLANE_COUNT; ++i) {
		if (ce_plane_dist(&f->p[i],
				get_box_vertex_positive(&p, &f->p[i].n, b)) < 0.0f) {
			return false;
		}
	}
	return true;
}
