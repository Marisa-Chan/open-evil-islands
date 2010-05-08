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

#include <assert.h>

#include "cegl.h"
#include "celib.h"
#include "cealloc.h"
#include "celogging.h"
#include "cecontext.h"

#include "cecontext_posix.h"

ce_context* ce_context_new(Display* display)
{
	int error_base, event_base;
	const int major_version_req = 1, minor_version_req = 2;
	int major_version, minor_version;

	if (!glXQueryExtension(display, &error_base, &event_base) ||
			!glXQueryVersion(display, &major_version, &minor_version)) {
		ce_logging_error("context: no GLX support available");
		return NULL;
	}

	ce_logging_write("context: using GLX %d.%d", major_version, minor_version);

	if (major_version_req != major_version || minor_version < minor_version_req) {
		ce_logging_error("context: GLX %d.>=%d required",
			major_version_req, minor_version_req);
		return NULL;
	}

	XVisualInfo* visualinfo = glXChooseVisual(display, XDefaultScreen(display),
			(int[]) { GLX_RGBA, GLX_DOUBLEBUFFER,
						GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1,
						GLX_BLUE_SIZE, 1, GLX_ALPHA_SIZE, 1,
						GLX_STENCIL_SIZE, 1, GLX_DEPTH_SIZE, 1, None });
	if (NULL == visualinfo) {
		ce_logging_error("context: no appropriate visual found");
	}

	ce_logging_write("context: visual %u chosen", visualinfo->visualid);

	ce_context* context = ce_alloc(sizeof(ce_context));
	context->error_base = error_base;
	context->event_base = event_base;
	context->major_version = major_version;
	context->minor_version = minor_version;
	context->bpp = 32;
	context->visualinfo = visualinfo;
	context->context = glXCreateContext(display, visualinfo, NULL, True);

	glXGetConfig(display, visualinfo, GLX_BUFFER_SIZE, &context->bpp);

	if (glXIsDirect(display, context->context)) {
		ce_logging_write("context: you have direct rendering");
	} else {
		ce_logging_warning("context: no direct rendering possible");
	}

	return context;
}

void ce_context_del(ce_context* context)
{
	if (NULL != context) {
		assert(glXGetCurrentContext() == context->context);
		if (NULL != context->context) {
			ce_gl_term();
			Display* display = glXGetCurrentDisplay();
			glXMakeCurrent(display, None, NULL);
			glXDestroyContext(display, context->context);
		}
		if (NULL != context->visualinfo) {
			XFree(context->visualinfo);
		}
		ce_free(context, sizeof(ce_context));
	}
}

void ce_context_make_current(ce_context* context, Display* display,
													GLXDrawable drawable)
{
	assert(NULL == glXGetCurrentContext());
	glXMakeCurrent(display, drawable, context->context);
	ce_gl_init();
}

void ce_context_swap(ce_context* context)
{
	ce_unused(context);
	assert(NULL != glXGetCurrentContext());
	assert(glXGetCurrentContext() == context->context);
	glXSwapBuffers(glXGetCurrentDisplay(), glXGetCurrentDrawable());
}