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

#ifndef CE_ROOT_HPP
#define CE_ROOT_HPP

#include "timer.hpp"
#include "input.hpp"
#include "renderwindow.hpp"
#include "scenemng.hpp"
#include "optparse.hpp"

namespace cursedearth
{
    extern struct ce_root {
        bool done;
        bool show_bboxes;
        bool comprehensive_bbox_only;
        float animation_fps;
        render_window_t* renderwindow;
        ce_scenemng* scenemng;
        timer_ptr_t timer;
        input_supply_ptr_t input_supply;
        input_event_const_ptr_t exit_event;
        input_event_const_ptr_t switch_window_event;
        input_event_const_ptr_t toggle_fullscreen_event;
        input_event_const_ptr_t toggle_bbox_event;
        ce_renderwindow_listener renderwindow_listener;
    } ce_root;

    extern bool ce_root_init(ce_optparse* optparse, int argc, char* argv[]);
    extern int ce_root_exec(void);
}

#endif /* CE_ROOT_HPP */
