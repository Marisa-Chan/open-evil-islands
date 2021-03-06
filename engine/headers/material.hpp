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

#ifndef CE_MATERIAL_HPP
#define CE_MATERIAL_HPP

#include "color.hpp"
#include "shader.hpp"

namespace cursedearth
{
    typedef enum {
        CE_MATERIAL_MODE_MODULATE,
        CE_MATERIAL_MODE_DECAL,
        CE_MATERIAL_MODE_REPLACE,
        CE_MATERIAL_MODE_COUNT
    } ce_material_mode;

    typedef struct {
        ce_material_mode mode;
        color_t ambient;
        color_t diffuse;
        color_t specular;
        color_t emission;
        float shininess;
        bool alpha_test;
        bool blend;
        ce_shader* shader;
    } ce_material;

    ce_material* ce_material_new(void);
    void ce_material_del(ce_material* material);
}

#endif
