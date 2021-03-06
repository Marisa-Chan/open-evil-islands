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

#ifndef CE_CONFIGFILE_HPP
#define CE_CONFIGFILE_HPP

#include <boost/filesystem/path.hpp>

#include "vector.hpp"
#include "string.hpp"

namespace cursedearth
{
    typedef struct {
        ce_string* name;
        ce_string* value;
    } ce_config_option;

    typedef struct {
        ce_string* name;
        ce_vector* options;
    } ce_config_section;

    typedef struct {
        ce_vector* sections;
    } ce_config_file;

    ce_config_file* ce_config_file_open(const boost::filesystem::path&);
    void ce_config_file_close(ce_config_file* config_file);

    inline size_t ce_config_file_section_count(ce_config_file* config_file)
    {
        return config_file->sections->count;
    }

    inline size_t ce_config_file_option_count(ce_config_file* config_file, size_t section_index)
    {
        ce_config_section* section = (ce_config_section*)config_file->sections->items[section_index];
        return section->options->count;
    }

    size_t ce_config_file_section_index(ce_config_file* config_file, const char* section_name);
    size_t ce_config_file_option_index(ce_config_file* config_file, size_t section_index, const char* option_name);

    inline const char* ce_config_file_get(ce_config_file* config_file, size_t section_index, size_t option_index)
    {
        ce_config_section* section = (ce_config_section*)config_file->sections->items[section_index];
        ce_config_option* option = (ce_config_option*)section->options->items[option_index];
        return option->value->str;
    }

    const char* ce_config_file_find(ce_config_file* config_file, const char* section_name, const char* option_name);
}

#endif
