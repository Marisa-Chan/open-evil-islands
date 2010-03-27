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
#include <assert.h>

#include "cealloc.h"
#include "cefighlp.h"
#include "cefigentity.h"

static bool ce_figentity_create_scenenodes(ce_fignode* fignode,
											ce_texture* textures[],
											ce_vector* renderitems,
											ce_scenenode* scenenode)
{
	// FIXME: to be reversed...
	int texture_number = fignode->figfile->texture_number - 1;
	if (texture_number > 1) {
		texture_number = 0;
	}

	ce_renderlayer* renderlayer = ce_renderlayer_new(
		ce_fighlp_create_material(fignode->figfile, textures[texture_number]));
	ce_renderlayer_add_renderitem(renderlayer, ce_renderitem_clone(
									renderitems->items[fignode->index]));

	ce_scenenode* child = ce_scenenode_new(scenenode);
	ce_scenenode_add_renderlayer(child, renderlayer);

	for (int i = 0; i < fignode->childs->count; ++i) {
		if (!ce_figentity_create_scenenodes(fignode->childs->items[i],
										textures, renderitems, scenenode)) {
			return false;
		}
	}

	return true;
}

ce_figentity* ce_figentity_new(ce_figmesh* figmesh,
								const ce_vec3* position,
								const ce_quat* orientation,
								ce_texture* textures[],
								ce_scenenode* scenenode)
{
	ce_figentity* figentity = ce_alloc(sizeof(ce_figentity));
	figentity->figmesh = ce_figmesh_add_ref(figmesh);
	figentity->figbone = ce_figbone_new(figmesh->figproto->fignode,
										&figmesh->complection, NULL);
	figentity->scenenode = ce_scenenode_new(scenenode);
	figentity->scenenode->position = *position;
	figentity->scenenode->orientation = *orientation;

	if (!ce_figentity_create_scenenodes(figmesh->figproto->fignode,
										textures, figmesh->renderitems,
										figentity->scenenode)) {
		ce_figentity_del(figentity);
		return NULL;
	}

	return figentity;
}

void ce_figentity_del(ce_figentity* figentity)
{
	if (NULL != figentity) {
		ce_scenenode_del(figentity->scenenode);
		ce_figbone_del(figentity->figbone);
		ce_figmesh_del(figentity->figmesh);
		ce_free(figentity, sizeof(ce_figentity));
	}
}

void ce_figentity_advance(ce_figentity* figentity, float fps, float elapsed)
{
	ce_figbone_advance(figentity->figbone,
		figentity->figmesh->figproto->fignode,
		figentity->scenenode->childs, fps, elapsed);
}

int ce_figentity_get_animation_count(ce_figentity* figentity)
{
	return figentity->figmesh->figproto->fignode->anmfiles->count;
}

const char* ce_figentity_get_animation_name(ce_figentity* figentity, int index)
{
	ce_fignode* fignode = figentity->figmesh->figproto->fignode;
	ce_anmfile* anmfile = fignode->anmfiles->items[index];
	return anmfile->name->str;
}

bool ce_figentity_play_animation(ce_figentity* figentity, const char* name)
{
	return ce_figbone_play_animation(figentity->figbone,
		figentity->figmesh->figproto->fignode, name);
}

void ce_figentity_stop_animation(ce_figentity* figentity)
{
	ce_figbone_stop_animation(figentity->figbone,
		figentity->figmesh->figproto->fignode);
}
