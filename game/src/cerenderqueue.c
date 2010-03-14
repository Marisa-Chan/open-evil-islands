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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "cemath.h"
#include "cealloc.h"
#include "cerenderqueue.h"

ce_renderqueue* ce_renderqueue_new(void)
{
	ce_renderqueue* renderqueue = ce_alloc(sizeof(ce_renderqueue));
	renderqueue->scenenodes[0] = ce_vector_new();
	renderqueue->scenenodes[1] = ce_vector_new();
	return renderqueue;
}

void ce_renderqueue_del(ce_renderqueue* renderqueue)
{
	if (NULL != renderqueue) {
		ce_vector_del(renderqueue->scenenodes[1]);
		ce_vector_del(renderqueue->scenenodes[0]);
		ce_free(renderqueue, sizeof(ce_renderqueue));
	}
}

void ce_renderqueue_clear(ce_renderqueue* renderqueue)
{
	ce_vector_clear(renderqueue->scenenodes[0]);
	ce_vector_clear(renderqueue->scenenodes[1]);
}

void ce_renderqueue_add_cascade(ce_renderqueue* renderqueue,
								ce_scenenode* scenenode,
								const ce_vec3* eye,
								const ce_frustum* frustum)
{
	if (ce_frustum_test_bbox(frustum, &scenenode->world_bbox)) {
		if (NULL != scenenode->renderlayer) {
			scenenode->dist2 = ce_vec3_dist2(eye, &scenenode->world_position);
			int index = scenenode->renderlayer->renderitem->transparent;
			ce_vector_push_back(renderqueue->scenenodes[index], scenenode);
		}

		for (int i = 0; i < scenenode->childs->count; ++i) {
			ce_renderqueue_add_cascade(renderqueue,
				scenenode->childs->items[i], eye, frustum);
		}
	}
}

typedef int (*ce_renderqueue_comp)(const void* lhs, const void* rhs);

static int ce_renderqueue_comp_less(const void* lhs, const void* rhs)
{
	const ce_scenenode* scenenode1 = *(const ce_scenenode**)lhs;
	const ce_scenenode* scenenode2 = *(const ce_scenenode**)rhs;
	return ce_fisequal(scenenode1->dist2, scenenode2->dist2, CE_EPS_E3) ?
		0 : (scenenode1->dist2 < scenenode2->dist2 ? -1 : 1);
}

static int ce_renderqueue_comp_greater(const void* lhs, const void* rhs)
{
	const ce_scenenode* scenenode1 = *(const ce_scenenode**)lhs;
	const ce_scenenode* scenenode2 = *(const ce_scenenode**)rhs;
	return ce_fisequal(scenenode1->dist2, scenenode2->dist2, CE_EPS_E3) ?
		0 : (scenenode1->dist2 > scenenode2->dist2 ? -1 : 1);
}

static const ce_renderqueue_comp ce_renderqueue_comps[2] = {
	ce_renderqueue_comp_less,
	ce_renderqueue_comp_greater
};

void ce_renderqueue_render(ce_renderqueue* renderqueue,
							ce_rendersystem* rendersystem)
{
	int count = 0;

	for (int i = 0; i < 2; ++i) {
		ce_vector* scenenodes = renderqueue->scenenodes[i];

		qsort(scenenodes->items, scenenodes->count,
				sizeof(ce_scenenode*), ce_renderqueue_comps[i]);

		for (int j = 0; j < scenenodes->count; ++j) {
			ce_scenenode_render(scenenodes->items[j], rendersystem);
		}

		count += scenenodes->count;
	}

	printf("visible: %d\n", count);
}
