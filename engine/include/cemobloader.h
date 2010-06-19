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

/*
 *  TODO: remarks
*/

#ifndef CE_MOBLOADER_H
#define CE_MOBLOADER_H

#include "cevector.h"
#include "cemobfile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	size_t queued_job_count;
	size_t completed_job_count;
	ce_string* name;
	ce_mobfile* mob_file;
} ce_mob_task;

extern ce_mob_task* ce_mob_task_new(const char* name);
extern void ce_mob_task_del(ce_mob_task* mob_task);

extern struct ce_mob_loader {
	ce_vector* mob_tasks;
}* ce_mob_loader;

extern void ce_mob_loader_init(void);
extern void ce_mob_loader_term(void);

extern void ce_mob_loader_clear(void);

extern void ce_mob_loader_load_mob(const char* name);

#ifdef __cplusplus
}
#endif

#endif /* CE_MOBLOADER_H */
