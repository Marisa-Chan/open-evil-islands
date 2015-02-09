/*
 *  This file is part of Cursed Earth.
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands.
 *  Copyright (C) 2009-2015 Yanis Kurganov <ykurganov@users.sourceforge.net>
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

#include <windows.h>

#include "alloc.hpp"
#include "logging.hpp"
#include "timer.hpp"

namespace cursedearth
{
    struct ce_timer
    {
        float elapsed;
        float frequency_inv;
        LARGE_INTEGER start;
        LARGE_INTEGER stop;
    };

    LONGLONG ce_timer_query_frequency(void)
    {
        LARGE_INTEGER frequency;
        if (QueryPerformanceFrequency(&frequency)) {
            return frequency.QuadPart;
        }
        ce_logging_warning("timer: using default frequency");
        return 1000000ll;
    }

    void ce_timer_query_counter(LARGE_INTEGER* value)
    {
        DWORD_PTR old_mask = SetThreadAffinityMask(GetCurrentThread(), 0);
        QueryPerformanceCounter(value);
        SetThreadAffinityMask(GetCurrentThread(), old_mask);
    }

    ce_timer* ce_timer_new(void)
    {
        ce_timer* timer = (ce_timer*)ce_alloc(sizeof(ce_timer));
        timer->frequency_inv = 1.0f / ce_timer_query_frequency();
        return timer;
    }

    void ce_timer_del(ce_timer* timer)
    {
        ce_free(timer, sizeof(ce_timer));
    }

    void ce_timer_start(ce_timer* timer)
    {
        ce_timer_query_counter(&timer->start);
    }

    float ce_timer_advance(ce_timer* timer)
    {
        ce_timer_query_counter(&timer->stop);
        timer->elapsed = (timer->stop.QuadPart - timer->start.QuadPart) * timer->frequency_inv;
        timer->start = timer->stop;
        return timer->elapsed;
    }

    float ce_timer_elapsed(ce_timer* timer)
    {
        return timer->elapsed;
    }
}
