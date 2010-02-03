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

#include "ceplane.h"

ce_plane* ce_plane_init(ce_plane* r, float a, float b, float c, float d)
{
	vec3_init(a, b, c, &r->n);
	r->d = d;
	return r;
}

ce_plane* ce_plane_init_tri(ce_plane* r, const vec3* a,
							const vec3* b, const vec3* c)
{
	vec3 e1, e2;
	vec3_sub(b, a, &e1);
	vec3_sub(c, a, &e2);
	vec3_cross(&e1, &e2, &r->n);
	vec3_normalise(&r->n, &r->n);
	r->d = -vec3_dot(&r->n, a);
	return r;
}

ce_plane* ce_plane_normalise(ce_plane* r, const ce_plane* a)
{
	const float s = 1.0f / vec3_abs(&a->n);
	vec3_scale(&a->n, s, &r->n);
	r->d = a->d * s;
	return r;
}

float ce_plane_dist(const ce_plane* a, const vec3* b)
{
	return vec3_dot(&a->n, b) + a->d;
}