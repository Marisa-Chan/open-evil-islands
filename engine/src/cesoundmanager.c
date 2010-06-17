/*
 *  This file is part of Cursed Earth
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands
 *  Copyright (C) 2009-2010 Yanis Kurganov
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

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "celib.h"
#include "cealloc.h"
#include "celogging.h"
#include "cepath.h"
#include "ceoptionmanager.h"
#include "cesoundmanager.h"

static const char* ce_sound_dirs[] = {"Stream", "Movies", NULL};
static const char* ce_sound_exts[] = {".wav", ".oga", ".mp3", ".ogv", ".bik", NULL};

struct ce_sound_manager* ce_sound_manager;

void ce_sound_manager_init(void)
{
	char path[ce_option_manager->ei_path->length + 16];
	for (size_t i = 0; NULL != ce_sound_dirs[i]; ++i) {
		ce_path_join_clear(path, sizeof(path),
			ce_option_manager->ei_path->str, ce_sound_dirs[i], NULL);
		ce_logging_write("sound manager: using path '%s'", path);
	}

	ce_sound_manager = ce_alloc_zero(sizeof(struct ce_sound_manager));
	ce_sound_manager->sound_instances = ce_vector_new();
}

void ce_sound_manager_term(void)
{
	if (NULL != ce_sound_manager) {
		ce_vector_for_each(ce_sound_manager->sound_instances, ce_sound_instance_del);
		ce_vector_del(ce_sound_manager->sound_instances);
		ce_free(ce_sound_manager, sizeof(struct ce_sound_manager));
	}
}

void ce_sound_manager_advance(float elapsed)
{
	ce_unused(elapsed);

	for (size_t i = 0; i < ce_sound_manager->sound_instances->count; ++i) {
		ce_sound_instance* sound_instance = ce_sound_manager->sound_instances->items[i];
		if (ce_sound_instance_is_stopped(sound_instance)) {
			ce_sound_instance_del(sound_instance);
			ce_vector_remove_unordered(ce_sound_manager->sound_instances, i--);
		}
	}
}

ce_sound_object ce_sound_manager_create(const char* name)
{
	char path[ce_option_manager->ei_path->length + strlen(name) + 32];
	if (NULL == ce_path_find_special1(path, sizeof(path),
										ce_option_manager->ei_path->str,
										name, ce_sound_dirs, ce_sound_exts)) {
		return 0;
	}

	ce_memfile* memfile = ce_memfile_open_path(path);
	if (NULL == memfile) {
		return 0;
	}

	ce_sound_resource* sound_resource = ce_sound_resource_new_builtin(memfile);
	if (NULL == sound_resource) {
		ce_memfile_close(memfile);
		return 0;
	}

	ce_sound_instance* sound_instance =
		ce_sound_instance_new(++ce_sound_manager->last_sound_object, sound_resource);
	if (NULL == sound_instance) {
		ce_logging_error("sound manager: could not create instance for '%s'", name);
		ce_sound_resource_del(sound_resource);
		return 0;
	}

	ce_vector_push_back(ce_sound_manager->sound_instances, sound_instance);

	return sound_instance->sound_object;
}

ce_sound_instance* ce_sound_manager_find(ce_sound_object sound_object)
{
	for (size_t i = 0; i < ce_sound_manager->sound_instances->count; ++i) {
		ce_sound_instance* sound_instance = ce_sound_manager->sound_instances->items[i];
		if (sound_object == sound_instance->sound_object) {
			return sound_instance;
		}
	}
	return NULL;
}
