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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "cegl.h"
#include "celib.h"
#include "cemath.h"
#include "cebyteorder.h"
#include "celogging.h"
#include "cealloc.h"
#include "cestring.h"
#include "cetexture.h"

struct ce_texture {
	ce_string* name;
	int ref_count;
	GLuint id;
};

static int ce_texture_correct_mipmap_count(int mipmap_count)
{
	// OpenGL Specification:
	// GL_INVALID_VALUE may be generated if level is less
	// than 0 or greater than log2(GL_MAX_TEXTURE_SIZE)

	GLint max_texture_size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

	int max_level = log2f(max_texture_size);

	static bool reported; // FIXME: threads
	if (!reported && mipmap_count - 1 > max_level) {
		ce_logging_warning("texture: your hardware supports a maximum "
			"of %d mipmaps, extra mipmaps were discarded", max_level);
		reported = true;
	}

	return ce_clamp(mipmap_count, 1, max_level + 1);
}

static void ce_texture_setup_filters(int mipmap_count)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (mipmap_count > 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
										GL_LINEAR_MIPMAP_LINEAR);
		if (GLEW_VERSION_1_2 || GLEW_SGIS_texture_lod) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmap_count - 1);
		} else if (GLEW_SGIS_generate_mipmap) {
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			static bool reported; // FIXME: threads
			if (!reported) {
				ce_logging_warning("texture: some opengl features are not "
									"available, mipmapping was disabled");
				reported = true;
			}
		}
	}
}

static void ce_texture_specify(int width, int height, int level,
	GLenum internal_format, GLenum data_format, GLenum data_type, void* data)
{
	GLint max_texture_size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

	int new_width = ce_min(width, max_texture_size);
	int new_height = ce_min(height, max_texture_size);

	if (!GLEW_VERSION_2_0 && !GLEW_ARB_texture_non_power_of_two) {
		if (!ce_sispot(new_width)) new_width = ce_snlpot(new_width);
		if (!ce_sispot(new_height)) new_height = ce_snlpot(new_height);
	}

	if (width != new_width || height != new_height) {
		gluScaleImage(data_format, width, height,
			data_type, data, new_width, new_height, data_type, data);
		width = new_width;
		height = new_height;
	}

	glTexImage2D(GL_TEXTURE_2D, level, internal_format,
		width, height, 0, data_format, data_type, data);
}

static void ce_texture_generate(ce_mmpfile* mmpfile,
	GLenum internal_format, GLenum data_format, GLenum data_type)
{
	int mipmap_count = ce_texture_correct_mipmap_count(mmpfile->mipmap_count);
	uint8_t* src = mmpfile->texels;

	// most ei's textures of width divisible by 4 (ogl's default row alignment)
	const bool not_aligned = 0 != mmpfile->width % 4;

	if (not_aligned) {
		glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	// FIXME: right shift on signed integer
	for (int i = 0, width = mmpfile->width, height = mmpfile->height;
			i < mipmap_count; ++i, width >>= 1, height >>= 1) {
		ce_texture_specify(width, height, i,
			internal_format, data_format, data_type, src);
		src += ce_mmpfile_storage_size(width, height, 1, mmpfile->bit_count);
	}

	if (not_aligned) {
		glPopClientAttrib();
	}

	ce_texture_setup_filters(mipmap_count);
}

static void ce_texture_generate_compressed(ce_mmpfile* mmpfile,
											GLenum internal_format)
{
	int mipmap_count = ce_texture_correct_mipmap_count(mmpfile->mipmap_count);
	const uint8_t* src = mmpfile->texels;

	// FIXME: right shift on signed integer
	for (int i = 0, width = mmpfile->width, height = mmpfile->height;
			i < mipmap_count; ++i, width >>= 1, height >>= 1) {
		int size = ce_mmpfile_storage_size(width, height, 1, mmpfile->bit_count);
		glCompressedTexImage2D(GL_TEXTURE_2D, i,
			internal_format, width, height, 0, size, src);
		src += size;
	}

	ce_texture_setup_filters(mipmap_count);
}

static void ce_texture_generate_unknown(ce_mmpfile* mmpfile)
{
	assert(false && "not implemented");
	ce_unused(mmpfile);
}

static void ce_texture_generate_dxt(ce_mmpfile* mmpfile)
{
	if (GLEW_VERSION_1_3 && (GLEW_EXT_texture_compression_s3tc ||
			(CE_MMPFILE_FORMAT_DXT1 == mmpfile->format &&
			GLEW_EXT_texture_compression_dxt1))) {
		ce_texture_generate_compressed(mmpfile, (GLenum[])
			{ GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
				GL_COMPRESSED_RGBA_S3TC_DXT3_EXT }
			[CE_MMPFILE_FORMAT_DXT3 == mmpfile->format]);
	} else {
		ce_mmpfile_convert(mmpfile, CE_MMPFILE_FORMAT_R8G8B8A8);
		ce_texture_generate(mmpfile, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	}
}

static void ce_texture_generate_r5g6b5(ce_mmpfile* mmpfile)
{
	if (GLEW_VERSION_1_2) {
		ce_texture_generate(mmpfile, GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5);
	} else {
		ce_mmpfile_convert(mmpfile, CE_MMPFILE_FORMAT_R8G8B8A8);
		ce_texture_generate(mmpfile, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	}
}

static void ce_texture_generate_a1rgb5(ce_mmpfile* mmpfile)
{
	if (GLEW_VERSION_1_2) {
		ce_texture_generate(mmpfile, GL_RGBA, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV);
	} else if (GLEW_EXT_packed_pixels) {
		ce_mmpfile_convert(mmpfile, CE_MMPFILE_FORMAT_RGB5A1);
		ce_texture_generate(mmpfile, GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1);
	} else {
		ce_mmpfile_convert(mmpfile, CE_MMPFILE_FORMAT_R8G8B8A8);
		ce_texture_generate(mmpfile, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	}
}

static void ce_texture_generate_argb4(ce_mmpfile* mmpfile)
{
	if (GLEW_VERSION_1_2) {
		ce_texture_generate(mmpfile, GL_RGBA, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV);
	} else if (GLEW_EXT_packed_pixels) {
		ce_mmpfile_convert(mmpfile, CE_MMPFILE_FORMAT_RGBA4);
		ce_texture_generate(mmpfile, GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4);
	} else {
		ce_mmpfile_convert(mmpfile, CE_MMPFILE_FORMAT_R8G8B8A8);
		ce_texture_generate(mmpfile, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	}
}

static void ce_texture_generate_argb8(ce_mmpfile* mmpfile)
{
	if (GLEW_VERSION_1_2) {
		ce_texture_generate(mmpfile, GL_RGBA, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV);
	} else if (GLEW_EXT_packed_pixels) {
		ce_mmpfile_convert(mmpfile, CE_MMPFILE_FORMAT_RGBA8);
		ce_texture_generate(mmpfile, GL_RGBA, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8);
	} else {
		ce_mmpfile_convert(mmpfile, CE_MMPFILE_FORMAT_R8G8B8A8);
		ce_texture_generate(mmpfile, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	}
}

static void (*ce_texture_generate_procs[CE_MMPFILE_FORMAT_COUNT])(ce_mmpfile*) = {
	ce_texture_generate_unknown,
	ce_texture_generate_dxt,
	ce_texture_generate_dxt,
	ce_texture_generate_unknown,
	ce_texture_generate_r5g6b5,
	ce_texture_generate_a1rgb5,
	ce_texture_generate_argb4,
	ce_texture_generate_argb8,
	ce_texture_generate_unknown,
	ce_texture_generate_unknown,
	ce_texture_generate_unknown,
	ce_texture_generate_unknown
};

ce_texture* ce_texture_new(const char* name, ce_mmpfile* mmpfile)
{
	ce_texture* texture = ce_alloc(sizeof(ce_texture));
	texture->name = ce_string_new_str(name);
	texture->ref_count = 1;

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	(*ce_texture_generate_procs[mmpfile->format])(mmpfile);

	if (ce_gl_report_errors()) {
		ce_logging_error("texture: opengl failed");
	}

	return texture;
}

void ce_texture_del(ce_texture* texture)
{
	if (NULL != texture) {
		assert(texture->ref_count > 0);
		if (0 == --texture->ref_count) {
			glDeleteTextures(1, &texture->id);
			ce_string_del(texture->name);
			ce_free(texture, sizeof(ce_texture));
		}
	}
}

void ce_texture_wrap(ce_texture* texture, ce_texture_wrap_mode mode)
{
	glBindTexture(GL_TEXTURE_2D, texture->id);

	if (CE_TEXTURE_WRAP_MODE_CLAMP_TO_EDGE == mode) {
		if (GLEW_VERSION_1_2 || GLEW_EXT_texture_edge_clamp ||
								GLEW_SGIS_texture_edge_clamp) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
	} else if (CE_TEXTURE_WRAP_MODE_CLAMP == mode) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

bool ce_texture_equal(const ce_texture* texture, const ce_texture* other)
{
	return texture->id == other->id;
}

const char* ce_texture_get_name(ce_texture* texture)
{
	return texture->name->str;
}

void ce_texture_bind(ce_texture* texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->id);
}

void ce_texture_unbind(ce_texture* texture)
{
	ce_unused(texture);
	glDisable(GL_TEXTURE_2D);
}

ce_texture* ce_texture_add_ref(ce_texture* texture)
{
	++texture->ref_count;
	return texture;
}
