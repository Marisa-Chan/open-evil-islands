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

#include "matrix3.hpp"

namespace cursedearth
{
    const matrix3_t CE_MAT3_ZERO = {
        {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f
        }
    };

    const matrix3_t CE_MAT3_IDENTITY = {
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        }
    };

    matrix3_t* ce_mat3_init_zero(matrix3_t* mat)
    {
        mat->m[0] = 0.0f; mat->m[1] = 0.0f; mat->m[2] = 0.0f;
        mat->m[3] = 0.0f; mat->m[4] = 0.0f; mat->m[5] = 0.0f;
        mat->m[6] = 0.0f; mat->m[7] = 0.0f; mat->m[8] = 0.0f;
        return mat;
    }

    matrix3_t* ce_mat3_init_identity(matrix3_t* mat)
    {
        mat->m[0] = 1.0f; mat->m[1] = 0.0f; mat->m[2] = 0.0f;
        mat->m[3] = 0.0f; mat->m[4] = 1.0f; mat->m[5] = 0.0f;
        mat->m[6] = 0.0f; mat->m[7] = 0.0f; mat->m[8] = 1.0f;
        return mat;
    }
}
