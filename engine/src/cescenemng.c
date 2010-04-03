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

#include "cemath.h"
#include "celogging.h"
#include "cealloc.h"
#include "cefrustum.h"
#include "ceformat.h"
#include "cemprhlp.h"
#include "cescenemng.h"

ce_scenemng* ce_scenemng_new(const char* root_path)
{
	ce_logging_write("scenemng: root path: '%s'", root_path);

	char path[strlen(root_path) + 64];
	snprintf(path, sizeof(path), "%s/Maps", root_path);

	ce_scenemng* scenemng = ce_alloc(sizeof(ce_scenemng));
	scenemng->scenenode = ce_scenenode_new(NULL);
	scenemng->texmng = ce_texmng_new();
	scenemng->mprmng = ce_mprmng_new(path);
	scenemng->terrain = NULL;
	scenemng->figmng = ce_figmng_new();
	scenemng->rendersystem = ce_rendersystem_new();
	scenemng->renderqueue = ce_renderqueue_new();
	scenemng->viewport = ce_viewport_new();
	scenemng->camera = ce_camera_new();
	scenemng->timer = ce_timer_new();
	scenemng->fps = ce_fps_new();
	scenemng->font = ce_font_new(CE_FONT_TYPE_HELVETICA_18);
	scenemng->show_axes = true;
	scenemng->show_bboxes = false;
	scenemng->comprehensive_bbox_only = true;
	scenemng->anm_fps = 15.0f;
	scenemng->scenenode_needs_update = true;

	ce_figmng_create_rendergroup(scenemng->figmng, scenemng->renderqueue);

	const char* texture_resources[] = { "textures", "redress", "menus" };
	for (int i = 0, n = sizeof(texture_resources) /
						sizeof(texture_resources[0]); i < n; ++i) {
		snprintf(path, sizeof(path), "%s/Res/%s.res",
				root_path, texture_resources[i]);
		ce_texmng_register_resource(scenemng->texmng, path);
	}

	const char* figure_resources[] = { "figures", "menus" };
	for (int i = 0, n = sizeof(figure_resources) /
						sizeof(figure_resources[0]); i < n; ++i) {
		snprintf(path, sizeof(path), "%s/Res/%s.res",
				root_path, figure_resources[i]);
		ce_figmng_register_resource(scenemng->figmng, path);
	}

	return scenemng;
}

void ce_scenemng_del(ce_scenemng* scenemng)
{
	if (NULL != scenemng) {
		ce_font_del(scenemng->font);
		ce_fps_del(scenemng->fps);
		ce_timer_del(scenemng->timer);
		ce_camera_del(scenemng->camera);
		ce_viewport_del(scenemng->viewport);
		ce_renderqueue_del(scenemng->renderqueue);
		ce_rendersystem_del(scenemng->rendersystem);
		ce_figmng_del(scenemng->figmng);
		ce_terrain_del(scenemng->terrain);
		ce_mprmng_del(scenemng->mprmng);
		ce_texmng_del(scenemng->texmng);
		ce_scenenode_del(scenemng->scenenode);
		ce_free(scenemng, sizeof(ce_scenemng));
	}
}

void ce_scenemng_advance(ce_scenemng* scenemng)
{
	ce_timer_advance(scenemng->timer);

	float elapsed = ce_timer_elapsed(scenemng->timer);

	ce_input_advance(elapsed);
	ce_fps_advance(scenemng->fps, elapsed);

	for (int i = 0; i < scenemng->figmng->figentities->count; ++i) {
		ce_figentity_advance(scenemng->figmng->figentities->items[i],
							scenemng->anm_fps, elapsed);
	}
}

void ce_scenemng_render(ce_scenemng* scenemng)
{
	ce_rendersystem_begin_render(scenemng->rendersystem, &CE_COLOR_WHITE);

	ce_rendersystem_setup_viewport(scenemng->rendersystem, scenemng->viewport);
	ce_rendersystem_setup_camera(scenemng->rendersystem, scenemng->camera);

	if (scenemng->show_axes) {
		ce_rendersystem_draw_axes(scenemng->rendersystem);
	}

	if (scenemng->scenenode_needs_update) {
		ce_scenenode_update_cascade(scenemng->scenenode, true);
		scenemng->scenenode_needs_update = false;
	}

	ce_vec3 forward, right, up;
	ce_frustum frustum;

	ce_frustum_init(&frustum, scenemng->camera->fov,
		scenemng->camera->aspect, scenemng->camera->near,
		scenemng->camera->far, &scenemng->camera->position,
		ce_camera_get_forward(scenemng->camera, &forward),
		ce_camera_get_right(scenemng->camera, &right),
		ce_camera_get_up(scenemng->camera, &up));

	ce_scenenode_cull_cascade(scenemng->scenenode, &frustum);

	for (int i = 0; i < scenemng->figmng->figentities->count; ++i) {
		ce_figentity_update(scenemng->figmng->figentities->items[i], false);
	}

	ce_scenenode_update_cascade(scenemng->scenenode, false);

	if (scenemng->show_bboxes) {
		ce_scenenode_draw_bboxes_cascade(scenemng->scenenode,
										scenemng->rendersystem,
										scenemng->comprehensive_bbox_only);
	}

	ce_renderqueue_clear(scenemng->renderqueue);

	ce_terrain_enqueue(scenemng->terrain, scenemng->renderqueue);
	ce_figmng_enqueue(scenemng->figmng, scenemng->renderqueue);

	ce_renderqueue_render(scenemng->renderqueue, scenemng->rendersystem);

	char text[128], bytefmt_text[64], bytefmt_text2[64], bytefmt_text3[64];

	snprintf(text, sizeof(text),
			"smallobj %s, max %s, overhead %s",
			ce_format_byte_detail(bytefmt_text, sizeof(bytefmt_text),
									ce_alloc_get_smallobj_allocated()),
			ce_format_byte_detail(bytefmt_text2, sizeof(bytefmt_text2),
									ce_alloc_get_smallobj_max_allocated()),
			ce_format_byte_detail(bytefmt_text3, sizeof(bytefmt_text3),
									ce_alloc_get_smallobj_overhead()));

	ce_font_render(scenemng->font, 10, scenemng->viewport->height - 1 *
		ce_font_get_height(scenemng->font) - 10, &CE_COLOR_RED, text);

	snprintf(text, sizeof(text),
			"system %s, max %s",
			ce_format_byte_detail(bytefmt_text, sizeof(bytefmt_text),
									ce_alloc_get_system_allocated()),
			ce_format_byte_detail(bytefmt_text2, sizeof(bytefmt_text2),
									ce_alloc_get_system_max_allocated()));

	ce_font_render(scenemng->font, 10, scenemng->viewport->height - 2 *
		ce_font_get_height(scenemng->font) - 10, &CE_COLOR_RED, text);

	snprintf(text, sizeof(text), "%d scene nodes in frustum", 0);

	ce_font_render(scenemng->font, 10, 10, &CE_COLOR_RED, text);

	ce_font_render(scenemng->font, scenemng->viewport->width -
		ce_font_get_width(scenemng->font, scenemng->fps->text) - 10,
		scenemng->viewport->height - ce_font_get_height(scenemng->font) - 10,
		&CE_COLOR_RED, scenemng->fps->text);

	const char* engine_text = "Powered by Cursed Earth Engine";

	ce_font_render(scenemng->font, scenemng->viewport->width -
		ce_font_get_width(scenemng->font, engine_text) - 10, 10,
		&CE_COLOR_RED, engine_text);

	ce_rendersystem_end_render(scenemng->rendersystem);
}

ce_terrain* ce_scenemng_create_terrain(ce_scenemng* scenemng,
										const char* name,
										const ce_vec3* position,
										const ce_quat* orientation,
										ce_scenenode* scenenode)
{
	ce_texture* stub_texture =
		ce_texmng_get_texture(scenemng->texmng, "default0");
	if (NULL == stub_texture) {
		return NULL;
	}

	ce_mprfile* mprfile = ce_mprmng_open_mprfile(scenemng->mprmng, name);
	if (NULL == mprfile) {
		return NULL;
	}

	// mpr name + nnn
	char texture_name[mprfile->name->length + 3 + 1];
	ce_texture* textures[mprfile->texture_count];

	for (int i = 0, n = mprfile->texture_count; i < n; ++i) {
		snprintf(texture_name, sizeof(texture_name),
				"%s%03d", mprfile->name->str, i);

		textures[i] = ce_texmng_get_texture(scenemng->texmng, texture_name);
		if (NULL == textures[i]) {
			ce_mprfile_close(mprfile);
			return NULL;
		}
	}

	if (NULL == scenenode) {
		scenenode = scenemng->scenenode;
	}

	ce_terrain_del(scenemng->terrain);
	scenemng->terrain = ce_terrain_new(mprfile, position,
										orientation, stub_texture,
										textures, scenenode);

	ce_terrain_create_rendergroup(scenemng->terrain, scenemng->renderqueue);

	scenemng->scenenode_needs_update = true;

	return scenemng->terrain;
}

ce_figentity*
ce_scenemng_create_figentity(ce_scenemng* scenemng,
							const char* name,
							const ce_complection* complection,
							const ce_vec3* position,
							const ce_quat* orientation,
							int texture_count,
							const char* texture_names[],
							ce_scenenode* scenenode)
{
	ce_texture* textures[texture_count];

	for (int i = 0; i < texture_count; ++i) {
		textures[i] = ce_texmng_get_texture(scenemng->texmng, texture_names[i]);
		if (NULL == textures[i]) {
			return NULL;
		}
	}

	if (NULL == scenenode) {
		scenenode = scenemng->scenenode;
		if (NULL != scenemng->terrain) {
			scenenode = ce_terrain_find_scenenode(scenemng->terrain,
												position->x, position->z);
		}
	}

	ce_figentity* figentity =
		ce_figmng_create_figentity(scenemng->figmng, name,
									complection, position,
									orientation, texture_count,
									textures, scenenode);

	if (NULL != figentity) {
		ce_figentity_update(figentity, true);
		scenemng->scenenode_needs_update = true;
	}

	return figentity;
}

ce_figentity*
ce_scenemng_create_figentity_mobobject(ce_scenemng* scenemng,
									const ce_mobobject_object* mobobject)
{
	ce_vec3 position = mobobject->position;
	ce_fswap(&position.y, &position.z);
	if (NULL != scenemng->terrain) {
		position.y += ce_mprhlp_get_height(scenemng->terrain->mprfile,
											position.x, position.z);
	}
	// yeah! it's a real hard-code :) move creatures up
	if (50 == mobobject->type ||
			51 == mobobject->type ||
			52 == mobobject->type) {
		position.y += 1.0f;
	}
	position.z = -position.z; // FIXME: opengl's hardcode

	ce_quat orientation, q;
	ce_quat_init_polar(&q, ce_deg2rad(-90.0f), &CE_VEC3_UNIT_X);
	ce_quat_mul(&orientation, &q, &mobobject->rotation);

	const char* texture_names[] = { mobobject->primary_texture->str,
									mobobject->secondary_texture->str };

	return ce_scenemng_create_figentity(scenemng,
										mobobject->model_name->str,
										&mobobject->complection,
										&position, &orientation,
										2, texture_names, NULL);
}

void ce_scenemng_remove_figentity(ce_scenemng* scenemng, ce_figentity* figentity)
{
	ce_figmng_remove_figentity(scenemng->figmng, figentity);
}

void ce_scenemng_load_mobfile(ce_scenemng* scenemng,
								const ce_mobfile* mobfile)
{
	for (int i = 0; i < mobfile->objects->count; ++i) {
		const ce_mobobject_object* mobobject = mobfile->objects->items[i];
		ce_scenemng_create_figentity_mobobject(scenemng, mobobject);
	}
}
