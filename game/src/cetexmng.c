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

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "cestr.h"
#include "celogging.h"
#include "cealloc.h"
#include "ceresfile.h"
#include "cetexmng.h"

ce_texmng* ce_texmng_new(void)
{
	ce_texmng* texmng = ce_alloc_zero(sizeof(ce_texmng));
	texmng->resources = ce_vector_new();
	texmng->textures = ce_vector_new();
	return texmng;
}

void ce_texmng_del(ce_texmng* texmng)
{
	if (NULL != texmng) {
		for (int i = 0; i < texmng->textures->count; ++i) {
			ce_texture_del(texmng->textures->items[i]);
		}
		for (int i = 0; i < texmng->resources->count; ++i) {
			ce_resfile_close(texmng->resources->items[i]);
		}
		ce_vector_del(texmng->textures);
		ce_vector_del(texmng->resources);
		ce_free(texmng, sizeof(ce_texmng));
	}
}

bool ce_texmng_register_resource(ce_texmng* texmng, const char* path)
{
	ce_resfile* resfile = ce_resfile_open_file(path);
	if (NULL == resfile) {
		ce_logging_error("texmng: loading '%s'... failed", path);
		return false;
	}

	ce_vector_push_back(texmng->resources, resfile);
	ce_logging_write("texmng: loading '%s'... ok", path);
	return true;
}

ce_texture* ce_texmng_get_texture(ce_texmng* texmng, const char* name)
{
	for (int i = 0; i < texmng->textures->count; ++i) {
		ce_texture* texture = texmng->textures->items[i];
		if (0 == ce_strcasecmp(name, ce_texture_get_name(texture))) {
			return texture;
		}
	}

	char path[strlen(name) + 4 + 1];
	snprintf(path, sizeof(path), "%s.mmp", name);

	for (int i = 0; i < texmng->resources->count; ++i) {
		ce_resfile* resfile = texmng->resources->items[i];
		int index = ce_resfile_node_index(resfile, path);
		if (-1 != index) {
			void* data = ce_resfile_extract_data(resfile, index);
			if (NULL == data) {
				return NULL;
			}

			ce_texture* texture = ce_texture_new(name, data);
			ce_free(data, ce_resfile_node_size(resfile, index));

			if (NULL == texture) {
				return NULL;
			}

			ce_vector_push_back(texmng->textures, texture);
			return texture;
		}
	}

	ce_logging_error("texmng: could not find texture: '%s'", name);
	return NULL;
}
