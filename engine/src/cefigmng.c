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
#include "cefigmng.h"

// TODO: cleanup unused protos and meshes

ce_figmng* ce_figmng_new(void)
{
	ce_figmng* figmng = ce_alloc(sizeof(ce_figmng));
	figmng->resfiles = ce_vector_new();
	figmng->figprotos = ce_vector_new();
	figmng->figmeshes = ce_vector_new();
	return figmng;
}

void ce_figmng_del(ce_figmng* figmng)
{
	if (NULL != figmng) {
		for (int i = 0; i < figmng->figmeshes->count; ++i) {
			ce_figmesh_del(figmng->figmeshes->items[i]);
		}
		for (int i = 0; i < figmng->figprotos->count; ++i) {
			ce_figproto_del(figmng->figprotos->items[i]);
		}
		for (int i = 0; i < figmng->resfiles->count; ++i) {
			ce_resfile_close(figmng->resfiles->items[i]);
		}
		ce_vector_del(figmng->figmeshes);
		ce_vector_del(figmng->figprotos);
		ce_vector_del(figmng->resfiles);
		ce_free(figmng, sizeof(ce_figmng));
	}
}

bool ce_figmng_register_resource(ce_figmng* figmng, const char* path)
{
	ce_resfile* resfile = ce_resfile_open_file(path);
	if (NULL == resfile) {
		ce_logging_error("figmng: loading '%s'... failed", path);
		return false;
	}

	ce_vector_push_back(figmng->resfiles, resfile);
	ce_logging_write("figmng: loading '%s'... ok", path);
	return true;
}

static ce_figproto* ce_figmng_get_figproto(ce_figmng* figmng, const char* name)
{
	for (int i = 0; i < figmng->figprotos->count; ++i) {
		ce_figproto* figproto = figmng->figprotos->items[i];
		if (0 == ce_strcasecmp(name, figproto->name->str)) {
			return figproto;
		}
	}

	// only guess figure name
	char file_name[strlen(name) + 4 + 1];
	snprintf(file_name, sizeof(file_name), "%s.mod", name);

	for (int i = 0; i < figmng->resfiles->count; ++i) {
		ce_resfile* resfile = figmng->resfiles->items[i];
		if (-1 != ce_resfile_node_index(resfile, file_name)) {
			ce_figproto* figproto = ce_figproto_new(name, resfile);
			if (NULL != figproto) {
				ce_vector_push_back(figmng->figprotos, figproto);
				return figproto;
			}
		}
	}

	ce_logging_error("figmng: could not create figproto: '%s'", name);
	return NULL;
}

static ce_figmesh* ce_figmng_get_figmesh(ce_figmng* figmng,
										const char* name,
										const ce_complection* complection)
{
	for (int i = 0; i < figmng->figmeshes->count; ++i) {
		ce_figmesh* figmesh = figmng->figmeshes->items[i];
		if (0 == ce_strcasecmp(name, figmesh->figproto->name->str) &&
				ce_complection_equal(complection, &figmesh->complection)) {
			return figmesh;
		}
	}

	ce_figproto* figproto = ce_figmng_get_figproto(figmng, name);
	if (NULL != figproto) {
		ce_figmesh* figmesh = ce_figmesh_new(figproto, complection);
		if (NULL != figmesh) {
			ce_vector_push_back(figmng->figmeshes, figmesh);
			return figmesh;
		}
	}

	ce_logging_error("figmng: could not create figmesh: '%s'", name);
	return NULL;
}

ce_figentity* ce_figmng_create_figentity(ce_figmng* figmng,
										const char* name,
										const ce_complection* complection,
										const ce_vec3* position,
										const ce_quat* orientation,
										int texture_count,
										ce_texture* textures[],
										ce_scenenode* scenenode)
{
	ce_figmesh* figmesh = ce_figmng_get_figmesh(figmng, name, complection);
	if (NULL != figmesh) {
		ce_figentity* figentity = ce_figentity_new(figmesh, position,
													orientation, texture_count,
													textures, scenenode);
		if (NULL != figentity) {
			return figentity;
		}
	}

	ce_logging_error("figmng: could not create figentity: '%s'", name);
	return NULL;
}