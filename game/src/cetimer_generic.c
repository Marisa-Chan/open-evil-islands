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

#include <time.h>

#include "cealloc.h"
#include "cetimer.h"

struct ce_timer {
	clock_t elapsed_old;
	clock_t elapsed_new;
	float elapsed_diff;
};

ce_timer* ce_timer_open(void)
{
	ce_timer* tmr = ce_alloc(sizeof(ce_timer));
	if (NULL == tmr) {
		return NULL;
	}
	tmr->elapsed_old = clock();
	return tmr;
}

void ce_timer_close(ce_timer* tmr)
{
	ce_free(tmr, sizeof(ce_timer));
}

void ce_timer_advance(ce_timer* tmr)
{
	tmr->elapsed_new = clock();
	tmr->elapsed_diff = (tmr->elapsed_new -
							tmr->elapsed_old) / (float)CLOCKS_PER_SEC;
	tmr->elapsed_old = tmr->elapsed_new;
}

float ce_timer_elapsed(ce_timer* tmr)
{
	return tmr->elapsed_diff;
}