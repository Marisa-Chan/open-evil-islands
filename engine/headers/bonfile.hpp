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

/*
 * doc/formats/bon.txt
 */

#ifndef CE_BONFILE_HPP
#define CE_BONFILE_HPP

#include <cstddef>

#include "resfile.hpp"

namespace cursedearth
{
    typedef struct {
        size_t size;
        float* bone;
    } ce_bonfile;

    ce_bonfile* ce_bonfile_open(ce_res_file* res_file, const char* name);
    void ce_bonfile_close(ce_bonfile* bonfile);
}

#endif
