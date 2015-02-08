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

#include <cstring>
#include <cstdio>
#include <cassert>

#include "lib.hpp"
#include "alloc.hpp"
#include "renderwindow.hpp"

namespace cursedearth
{
    ce_renderwindow_keypair* ce_renderwindow_keypair_new(unsigned long key, input_button_t button)
    {
        ce_renderwindow_keypair* keypair = (ce_renderwindow_keypair*)ce_alloc(sizeof(ce_renderwindow_keypair));
        keypair->key = key;
        keypair->button = button;
        return keypair;
    }

    void ce_renderwindow_keypair_del(ce_renderwindow_keypair* keypair)
    {
        ce_free(keypair, sizeof(ce_renderwindow_keypair));
    }

    int ce_renderwindow_keypair_sort_comp(const void* arg1, const void* arg2)
    {
        const ce_renderwindow_keypair* keypair1 = *(const ce_renderwindow_keypair**)arg1;
        const ce_renderwindow_keypair* keypair2 = *(const ce_renderwindow_keypair**)arg2;
        return keypair1->key < keypair2->key ? -1 : (int)(keypair1->key - keypair2->key);
    }

    int ce_renderwindow_keypair_search_comp(const void* arg1, const void* arg2)
    {
        const unsigned long* key = static_cast<const unsigned long*>(arg1);
        const ce_renderwindow_keypair* keypair = *(const ce_renderwindow_keypair**)arg2;
        return *key < keypair->key ? -1 : (int)(*key - keypair->key);
    }

    ce_renderwindow_keymap* ce_renderwindow_keymap_new(void)
    {
        ce_renderwindow_keymap* keymap = (ce_renderwindow_keymap*)ce_alloc(sizeof(ce_renderwindow_keymap));
        keymap->keypairs = ce_vector_new_reserved(static_cast<size_t>(input_button_t::count));
        return keymap;
    }

    void ce_renderwindow_keymap_del(ce_renderwindow_keymap* keymap)
    {
        if (NULL != keymap) {
            ce_vector_for_each(keymap->keypairs, (void(*)(void*))ce_renderwindow_keypair_del);
            ce_vector_del(keymap->keypairs);
            ce_free(keymap, sizeof(ce_renderwindow_keymap));
        }
    }

    void ce_renderwindow_keymap_add(ce_renderwindow_keymap* keymap, unsigned long key, input_button_t button)
    {
        ce_vector_push_back(keymap->keypairs, ce_renderwindow_keypair_new(key, button));
    }

    void ce_renderwindow_keymap_add_array(ce_renderwindow_keymap* keymap, const unsigned long keys[static_cast<size_t>(input_button_t::count)])
    {
        for (size_t i = static_cast<size_t>(input_button_t::unknown); i < static_cast<size_t>(input_button_t::count); ++i) {
            ce_renderwindow_keymap_add(keymap, keys[i], static_cast<input_button_t>(i));
        }
    }

    void ce_renderwindow_keymap_sort(ce_renderwindow_keymap* keymap)
    {
        qsort(keymap->keypairs->items, keymap->keypairs->count,
            sizeof(ce_renderwindow_keypair*), ce_renderwindow_keypair_sort_comp);
    }

    size_t ce_renderwindow_keymap_search(ce_renderwindow_keymap* keymap, unsigned long key)
    {
        ce_renderwindow_keypair** keypair = (ce_renderwindow_keypair**)bsearch(&key, keymap->keypairs->items,
            keymap->keypairs->count, sizeof(ce_renderwindow_keypair*), ce_renderwindow_keypair_search_comp);
        return static_cast<size_t>(NULL != keypair ? (*keypair)->button : input_button_t::unknown);
    }

    ce_renderwindow* ce_renderwindow_new(ce_renderwindow_vtable vtable, size_t size, ...)
    {
        ce_renderwindow* renderwindow = new ce_renderwindow;
        memset(renderwindow, 0, sizeof(ce_renderwindow));
        renderwindow->impl = new uint8_t[size];
        memset(renderwindow->impl, 0, size);
        renderwindow->vtable = vtable;
        renderwindow->size = size;

        va_list args;
        va_start(args, size);

        renderwindow->geometry[renderwindow->state].width = ce_max(int, 400, va_arg(args, int));
        renderwindow->geometry[renderwindow->state].height = ce_max(int, 300, va_arg(args, int));

        renderwindow->m_input_context = std::make_shared<input_context_t>();
        renderwindow->m_input_context->clear();

        renderwindow->keymap = ce_renderwindow_keymap_new();
        renderwindow->listeners = ce_vector_new();

        bool ok = (*vtable.ctor)(renderwindow, args);

        va_end(args);

        if (!ok) {
            ce_renderwindow_del(renderwindow);
            return NULL;
        }

        return renderwindow;
    }

    void ce_renderwindow_del(ce_renderwindow* renderwindow)
    {
        if (NULL != renderwindow) {
            (*renderwindow->vtable.dtor)(renderwindow);

            ce_vector_del(renderwindow->listeners);
            ce_renderwindow_keymap_del(renderwindow->keymap);

            delete[] static_cast<uint8_t*>(renderwindow->impl);
            delete renderwindow;
        }
    }

    void ce_renderwindow_add_listener(ce_renderwindow* renderwindow, ce_renderwindow_listener* listener)
    {
        ce_vector_push_back(renderwindow->listeners, listener);
    }

    void ce_renderwindow_show(ce_renderwindow* renderwindow)
    {
        (*renderwindow->vtable.show)(renderwindow);
    }

    void ce_renderwindow_minimize(ce_renderwindow* renderwindow)
    {
        if (CE_RENDERWINDOW_STATE_FULLSCREEN == renderwindow->state) {
            assert(!renderwindow->restore_fullscreen);
            renderwindow->restore_fullscreen = true;

            // exit from fullscreen before minimizing
            ce_renderwindow_toggle_fullscreen(renderwindow);
        }

        (*renderwindow->vtable.minimize)(renderwindow);
    }

    void ce_renderwindow_toggle_fullscreen(ce_renderwindow* renderwindow)
    {
        const ce_renderwindow_state state[] = { CE_RENDERWINDOW_STATE_FULLSCREEN, CE_RENDERWINDOW_STATE_WINDOW };
        renderwindow->state = state[renderwindow->state];

        if (NULL != renderwindow->vtable.fullscreen.prepare) {
            (*renderwindow->vtable.fullscreen.prepare)(renderwindow);
        }

        if (CE_RENDERWINDOW_STATE_FULLSCREEN == renderwindow->state) {
            if (NULL != renderwindow->vtable.fullscreen.before_enter) {
                (*renderwindow->vtable.fullscreen.before_enter)(renderwindow);
            }

            size_t index = ce_displaymng_enter(renderwindow->displaymng,
                renderwindow->geometry[renderwindow->state].width,
                renderwindow->geometry[renderwindow->state].height,
                renderwindow->visual.bpp, renderwindow->visual.rate,
                renderwindow->visual.rotation, renderwindow->visual.reflection);

            const ce_displaymode* mode = (const ce_displaymode*)renderwindow->displaymng->supported_modes->items[index];

            renderwindow->geometry[renderwindow->state].width = mode->width;
            renderwindow->geometry[renderwindow->state].height = mode->height;

            renderwindow->visual.bpp = mode->bpp;
            renderwindow->visual.rate = mode->rate;
            // TODO: rotation, reflection

            if (NULL != renderwindow->vtable.fullscreen.after_enter) {
                (*renderwindow->vtable.fullscreen.after_enter)(renderwindow);
            }
        } else {
            if (NULL != renderwindow->vtable.fullscreen.before_exit) {
                (*renderwindow->vtable.fullscreen.before_exit)(renderwindow);
            }

            ce_displaymng_exit(renderwindow->displaymng);

            if (NULL != renderwindow->vtable.fullscreen.after_exit) {
                (*renderwindow->vtable.fullscreen.after_exit)(renderwindow);
            }
        }

        if (NULL != renderwindow->vtable.fullscreen.done) {
            (*renderwindow->vtable.fullscreen.done)(renderwindow);
        }
    }

    void ce_renderwindow_action_proc_none(ce_renderwindow*)
    {
    }

    void ce_renderwindow_action_proc_restored(ce_renderwindow* renderwindow)
    {
        if (renderwindow->restore_fullscreen) {
            renderwindow->restore_fullscreen = false;
            assert(CE_RENDERWINDOW_STATE_WINDOW == renderwindow->state);
            ce_renderwindow_toggle_fullscreen(renderwindow);
        }
    }

    void (*ce_renderwindow_action_procs[CE_RENDERWINDOW_ACTION_COUNT])(ce_renderwindow*) = {
        ce_renderwindow_action_proc_none,
        ce_renderwindow_minimize,
        ce_renderwindow_action_proc_restored
    };

    void ce_renderwindow::pump()
    {
        // reset pointer offset every frame
        m_input_context->pointer_offset = CE_VEC2_ZERO;

        // reset wheel buttons: there are no "WheelRelease" events in most cases
        m_input_context->buttons[static_cast<size_t>(input_button_t::mb_wheelup)] = false;
        m_input_context->buttons[static_cast<size_t>(input_button_t::mb_wheeldown)] = false;

        (*vtable.pump)(this);

        (*ce_renderwindow_action_procs[action])(this);
        action = CE_RENDERWINDOW_ACTION_NONE;
    }

    void ce_renderwindow_emit_resized(ce_renderwindow* renderwindow, int width, int height)
    {
        for (size_t i = 0; i < renderwindow->listeners->count; ++i) {
            ce_renderwindow_listener* listener = (ce_renderwindow_listener*)renderwindow->listeners->items[i];
            if (NULL != listener->resized) {
                (*listener->resized)(listener->listener, width, height);
            }
        }
    }

    void ce_renderwindow_emit_closed(ce_renderwindow* renderwindow)
    {
        for (size_t i = 0; i < renderwindow->listeners->count; ++i) {
            ce_renderwindow_listener* listener = (ce_renderwindow_listener*)renderwindow->listeners->items[i];
            if (NULL != listener->closed) {
                (*listener->closed)(listener->listener);
            }
        }
    }
}
