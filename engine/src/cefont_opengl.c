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
#include <assert.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "cegl.h"
#include "celib.h"
#include "cealloc.h"
#include "celogging.h"
#include "ceresource.h"
#include "cefont.h"

#define CE_FONT_NUM_CHARS 96

static const unsigned int CE_FONT_SPACE = 32;
//static const unsigned int CE_FONT_NUM_CHARS = 96;

static const size_t CE_FONT_MARGIN = 3;

struct ce_font {
	FT_Library library;
	unsigned char widths[CE_FONT_NUM_CHARS];
	unsigned int height;
	GLuint list, tex;
};

ce_font* ce_font_new(ce_font_type type)
{
	ce_font* font = ce_alloc_zero(sizeof(ce_font));
	return font;
}

ce_font* ce_font_new_path(const char* resource_path)
{
	ce_font* font = ce_alloc_zero(sizeof(ce_font));

	// TODO: FT_New_Library and own memory allocators

	FT_Error code = FT_Init_FreeType(&font->library);
	if (0 != code) {
		ce_logging_error("font: could not initialize FreeType2 library");
		ce_font_del(font);
		return NULL;
	}

	size_t resource_index = ce_resource_find(resource_path);
	assert(resource_index < CE_RESOURCE_DATA_COUNT);

	FT_Face face;
	code = FT_New_Memory_Face(font->library, ce_resource_data[resource_index],
							ce_resource_data_sizes[resource_index], 0, &face);
	if (0 != code) {
		assert(FT_Err_Unknown_File_Format != code && "font format is unsupported");
		ce_logging_error("font: could not load font data");
		ce_font_del(font);
		return NULL;
	}

	assert(face->face_flags & FT_FACE_FLAG_SCALABLE &&
			face->face_flags & FT_FACE_FLAG_HORIZONTAL && "invalid font");

	FT_Set_Pixel_Sizes(face, 24, 0); // TODO: size

	// find max ascent/descent to calculate imageHeight
	size_t image_width = 256;
	int max_ascent = 0, max_descent = 0;
	size_t line_space = image_width - CE_FONT_MARGIN;
	size_t lines = 1, char_index;

	for (unsigned int ch = 0; ch < CE_FONT_NUM_CHARS; ++ch) {
		// look up the character in the font file
		char_index = FT_Get_Char_Index(face, ch + CE_FONT_SPACE);

		// render the current glyph
		FT_Load_Glyph(face, char_index, FT_LOAD_RENDER);

		font->widths[ch] = (face->glyph->metrics.horiAdvance >> 6) + CE_FONT_MARGIN;

		// if the line is full go to the next line
		if(font->widths[ch] > line_space) {
			line_space = image_width - CE_FONT_MARGIN;
			++lines;
		}

		line_space -= font->widths[ch];

		max_ascent = ce_max(max_ascent, face->glyph->bitmap_top);
		max_descent = ce_max(max_descent, face->glyph->bitmap.rows -
											face->glyph->bitmap_top);
	}

	font->height = max_ascent + max_descent;

	// TODO: POT?
	size_t image_height = (font->height + CE_FONT_MARGIN) * lines + CE_FONT_MARGIN;

	// generation of the actual texture
	font->list = glGenLists(CE_FONT_NUM_CHARS);

	unsigned char* image = ce_alloc(image_width * image_height);

	size_t x = CE_FONT_MARGIN;
	size_t y = CE_FONT_MARGIN + max_ascent;

	for (unsigned int ch = 0; ch < CE_FONT_NUM_CHARS; ++ch) {
		size_t char_index = FT_Get_Char_Index(face, ch + CE_FONT_SPACE);

		FT_Load_Glyph(face, char_index, FT_LOAD_DEFAULT);
		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		if (font->widths[ch] > image_width - x) {
			x = CE_FONT_MARGIN;
			y += font->height + CE_FONT_MARGIN;
		}

		// calculate texture coordinates of the character
		float texx1 = x / (float)image_width;
		float texx2 = (x + font->widths[ch]) / (float)image_width;
		float texy1 = (y - max_ascent) / (float)image_height;
		float texy2 = texy1 + font->height / (float)image_height;

		glNewList(font->list + ch, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(texx1, texy1); glVertex2i(0, 0);
		glTexCoord2f(texx2, texy1); glVertex2i(font->widths[ch], 0);
		glTexCoord2f(texx2, texy2); glVertex2i(font->widths[ch], font->height);
		glTexCoord2f(texx1, texy2); glVertex2i(0, font->height);
		glEnd();
		glTranslatef(font->widths[ch], 0.0f, 0.0f);
		glEndList();

		for (int row = 0; row < face->glyph->bitmap.rows; ++row) {
			for (int pixel = 0; pixel < face->glyph->bitmap.width; ++pixel) {
				// set pixel at position to intensity (0-255) at the position
				image[(x + face->glyph->bitmap_left + pixel) +
					(y - face->glyph->bitmap_top + row) * image_width] =
						face->glyph->bitmap.buffer[pixel +
						row * face->glyph->bitmap.pitch];
			}
		}

		x += font->widths[ch];
	}

	glGenTextures(1, &font->tex);
	glBindTexture(GL_TEXTURE_2D, font->tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, image_width, image_height,
					0, GL_ALPHA, GL_UNSIGNED_BYTE, image);

	ce_free(image, image_width * image_height);
	FT_Done_Face(face);

	return font;
}

void ce_font_del(ce_font* font)
{
	if (NULL != font) {
		glDeleteTextures(1, &font->tex);
		glDeleteLists(font->list, CE_FONT_NUM_CHARS);
		if (NULL != font->library) {
			FT_Done_FreeType(font->library);
		}
		ce_free(font, sizeof(ce_font));
	}
}

int ce_font_get_height(ce_font* font)
{
	return font->height;
}

int ce_font_get_width(ce_font* font, const char* text)
{
	int width = 0;
	while (*text) {
		width += font->widths[*text++ - CE_FONT_SPACE];
	}
	return width;
}

void ce_font_render(ce_font* font, int x, int y,
					const ce_color* color, const char* text)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, font->tex);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslated(x, y, 0.0f);

	for (size_t i = 0, length = strlen(text); i < length; ++i) {
		unsigned char ch = text[i] - CE_FONT_SPACE;

		//assert(ch < CE_FONT_NUM_CHARS);

		// replace characters outside the valid range with undrawable
		if(ch > CE_FONT_NUM_CHARS) {
			ch = CE_FONT_NUM_CHARS - 1; // last character is 'undrawable'
		}

		glCallList(font->list + ch);
	}

	//glListBase(font->list - 32);
	//glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();
}
