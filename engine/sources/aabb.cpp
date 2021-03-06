/*
 *  This file is part of Cursed Earth.
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands.
 *  Copyright (C) 2009-2017 Yanis Kurganov <ykurganov@users.sourceforge.net>
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

#include <cfloat>
#include <cmath>

#include "aabb.hpp"

namespace cursedearth
{
    void ce_aabb_merge_aabb_pass(float* origin, float* extent, float other_origin, float other_extent)
    {
        float displacement = other_origin - *origin;
        float difference = other_extent - *extent;
        if (*extent < 0.0f || difference >= fabsf(displacement)) {
            // 2nd contains 1st
            *extent = other_extent;
            *origin = other_origin;
        } else if (other_extent < 0.0f || -difference >= fabsf(displacement)) {
            // 1st contains 2nd, do nothing
        } else {
            // not contained
            float min, max;
            if (displacement > 0.0f) {
                min = *origin - *extent;
                max = other_origin + other_extent;
            } else {
                min = other_origin - other_extent;
                max = *origin + *extent;
            }
            *origin = 0.5f * (min + max);
            *extent = max - *origin;
        }
    }

    void ce_aabb_merge_point_pass(float* origin, float* extent, float point)
    {
        float displacement = point - *origin;
        if (fabsf(displacement) > *extent) {
            float min, max;
            if (*extent < 0.0f) { // degenerate
                min = max = point;
            } else if (displacement > 0.0f) {
                min = *origin - *extent;
                max = *origin + displacement;
            } else {
                max = *origin + *extent;
                min = *origin + displacement;
            }
            *origin = 0.5f * (min + max);
            *extent = max - *origin;
        }
    }

    aabb_t* ce_aabb_init(aabb_t* aabb, const vector3_t* origin, const vector3_t* extents, float radius)
    {
        aabb->origin = *origin;
        aabb->extents = *extents;
        aabb->radius = radius;
        return aabb;
    }

    aabb_t* ce_aabb_init_zero(aabb_t* aabb)
    {
        aabb->origin = CE_VEC3_ZERO;
        aabb->extents = CE_VEC3_ZERO;
        aabb->radius = 0.0f;
        return aabb;
    }

    aabb_t* ce_aabb_copy(aabb_t* aabb, const aabb_t* other)
    {
        aabb->origin = other->origin;
        aabb->extents = other->extents;
        aabb->radius = other->radius;
        return aabb;
    }

    aabb_t* ce_aabb_clear(aabb_t* aabb)
    {
        aabb->origin = CE_VEC3_ZERO;
        aabb->extents.x = aabb->extents.y = aabb->extents.z = -FLT_MAX;
        aabb->radius = 0.0f;
        return aabb;
    }

    aabb_t* ce_aabb_update_radius(aabb_t* aabb)
    {
        aabb->radius = ce_vec3_len(&aabb->extents);
        return aabb;
    }

    aabb_t* ce_aabb_merge_aabb(aabb_t* aabb, const aabb_t* other)
    {
        ce_aabb_merge_aabb_pass(&aabb->origin.x, &aabb->extents.x,
                                other->origin.x, other->extents.x);
        ce_aabb_merge_aabb_pass(&aabb->origin.y, &aabb->extents.y,
                                other->origin.y, other->extents.y);
        ce_aabb_merge_aabb_pass(&aabb->origin.z, &aabb->extents.z,
                                other->origin.z, other->extents.z);
        return aabb;
    }

    aabb_t* ce_aabb_merge_point(aabb_t* aabb, const vector3_t* point)
    {
        ce_aabb_merge_point_pass(&aabb->origin.x, &aabb->extents.x, point->x);
        ce_aabb_merge_point_pass(&aabb->origin.y, &aabb->extents.y, point->y);
        ce_aabb_merge_point_pass(&aabb->origin.z, &aabb->extents.z, point->z);
        return aabb;
    }

    aabb_t* ce_aabb_merge_point_array(aabb_t* aabb, const float* point)
    {
        ce_aabb_merge_point_pass(&aabb->origin.x, &aabb->extents.x, point[0]);
        ce_aabb_merge_point_pass(&aabb->origin.y, &aabb->extents.y, point[1]);
        ce_aabb_merge_point_pass(&aabb->origin.z, &aabb->extents.z, point[2]);
        return aabb;
    }
}
