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

#ifndef CE_VIDEORESOURCE_H
#define CE_VIDEORESOURCE_H

#include <stddef.h>
#include <stdbool.h>

#include "cememfile.h"
#include "ceycbcr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ce_video_resource ce_video_resource;

typedef struct {
	size_t (*size_hint)(ce_memfile* memfile);
	bool (*test)(ce_memfile* memfile);
	bool (*ctor)(ce_video_resource* video_resource);
	void (*dtor)(ce_video_resource* video_resource);
	bool (*read)(ce_video_resource* video_resource);
	bool (*reset)(ce_video_resource* video_resource);
} ce_video_resource_vtable;

struct ce_video_resource {
	unsigned int width, height;
	float fps, time;
	size_t frame_index, frame_count;
	ce_ycbcr ycbcr;
	ce_memfile* memfile;
	ce_video_resource_vtable vtable;
	size_t size;
	char impl[];
};

extern ce_video_resource* ce_video_resource_new(ce_memfile* memfile);
extern void ce_video_resource_del(ce_video_resource* video_resource);

static inline bool ce_video_resource_read(ce_video_resource* video_resource)
{
	return (*video_resource->vtable.read)(video_resource);
}

extern bool ce_video_resource_reset(ce_video_resource* video_resource);

#ifdef __cplusplus
}
#endif

#endif /* CE_VIDEORESOURCE_H */
