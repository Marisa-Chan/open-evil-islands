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

#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>

#include "alloc.hpp"
#include "resourcemanager.hpp"
#include "resball.hpp"
#include "figproto.hpp"

namespace cursedearth
{
    ce_figproto* ce_figproto_new(const char* name, ce_res_file* res_file)
    {
        ce_figproto* figproto = (ce_figproto*)ce_alloc_zero(sizeof(ce_figproto));
        figproto->ref_count = 1;
        figproto->name = ce_string_new_str(name);

        std::vector<char> file_name(strlen(name) + 4 + 1);

        snprintf(file_name.data(), file_name.size(), "%s.adb", name);
        ce_mem_file* adb_mem_file = ce_res_ball_extract_mem_file_by_name(ce_resource_manager->database, file_name.data());
        if (NULL != adb_mem_file) {
            figproto->adb_file = ce_adb_file_new(adb_mem_file);
            ce_mem_file_del(adb_mem_file);
        }

        snprintf(file_name.data(), file_name.size(), "%s.mod", name);
        ce_res_file* mod_res_file = ce_res_ball_extract_res_file_by_name(res_file, file_name.data());

        snprintf(file_name.data(), file_name.size(), "%s.bon", name);
        ce_res_file* bon_res_file = ce_res_ball_extract_res_file_by_name(res_file, file_name.data());

        snprintf(file_name.data(), file_name.size(), "%s.anm", name);
        ce_res_file* anm_res_file = ce_res_ball_extract_res_file_by_name(res_file, file_name.data());

        assert(NULL != mod_res_file); // mod required
        assert(NULL != bon_res_file); // bon required

        // anm optional
        std::vector<ce_res_file*> anm_res_files(NULL == anm_res_file ? 1 : anm_res_file->node_count + 1);
        if (NULL != anm_res_file) {
            ce_res_ball_extract_all_res_files(anm_res_file, anm_res_files.data());
            anm_res_files[anm_res_file->node_count] = NULL;
        } else {
            anm_res_files[0] = NULL;
        }

        ce_lnkfile* lnkfile = ce_lnkfile_open(mod_res_file, name);

        figproto->fignode = ce_fignode_new(mod_res_file, bon_res_file, anm_res_files.data(), lnkfile);

        ce_lnkfile_close(lnkfile);
        if (NULL != anm_res_file) {
            ce_res_ball_clean_all_res_files(anm_res_file, anm_res_files.data());
        }
        ce_res_file_del(anm_res_file);
        ce_res_file_del(bon_res_file);
        ce_res_file_del(mod_res_file);

        return figproto;
    }

    void ce_figproto_del(ce_figproto* figproto)
    {
        if (NULL != figproto) {
            assert(figproto->ref_count > 0);
            if (0 == --figproto->ref_count) {
                ce_fignode_del(figproto->fignode);
                ce_adb_file_del(figproto->adb_file);
                ce_string_del(figproto->name);
                ce_free(figproto, sizeof(ce_figproto));
            }
        }
    }

    void ce_figproto_accept_renderqueue(ce_figproto* figproto, ce_renderqueue* renderqueue)
    {
        ce_fignode_accept_renderqueue_cascade(figproto->fignode, renderqueue);
    }
}
