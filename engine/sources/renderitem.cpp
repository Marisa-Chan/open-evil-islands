/*
 *  This file is part of Cursed Earth.
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands.
 *  Copyright (C) 2009-2017 Yanis Kurganov <ykurganov@users.sourceforge.net>
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

#include "alloc.hpp"
#include "logging.hpp"
#include "renderitem.hpp"

namespace cursedearth
{
    ce_renderitem* ce_renderitem_new(ce_renderitem_vtable vtable, size_t size, ...)
    {
        ce_renderitem* renderitem = (ce_renderitem*)ce_alloc_zero(sizeof(ce_renderitem));
        renderitem->impl = ce_alloc_zero(size);
        renderitem->visible = true;
        renderitem->vtable = vtable;
        renderitem->size = size;

        va_list args;
        va_start(args, size);
        (vtable.ctor)(renderitem, args);
        va_end(args);

        return renderitem;
    }

    void ce_renderitem_del(ce_renderitem* renderitem)
    {
        if (NULL != renderitem) {
            (renderitem->vtable.dtor)(renderitem);
            ce_free(renderitem->impl, renderitem->size);
            ce_free(renderitem, sizeof(ce_renderitem));
        }
    }

    void ce_renderitem_update(ce_renderitem* renderitem, ...)
    {
        if (NULL != renderitem->vtable.update) {
            va_list args;
            va_start(args, renderitem);
            (renderitem->vtable.update)(renderitem, args);
            va_end(args);
        }
    }

    void ce_renderitem_render(ce_renderitem* renderitem)
    {
        (renderitem->vtable.render)(renderitem);
    }

    ce_renderitem* ce_renderitem_clone(const ce_renderitem* renderitem)
    {
        ce_renderitem* clone_renderitem = (ce_renderitem*)ce_alloc_zero(sizeof(ce_renderitem));
        *clone_renderitem = *renderitem;
        clone_renderitem->impl = ce_alloc_zero(renderitem->size);
        (renderitem->vtable.clone)(renderitem, clone_renderitem);
        return clone_renderitem;
    }
}
