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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "cealloc.h"
#include "ceroot.h"
#include "cesoundinstance.h"

static ce_optparse* optparse;

static ce_soundresource* soundresource1;
static ce_soundresource* soundresource2;

static ce_soundinstance* soundinstance1;
static ce_soundinstance* soundinstance2;

static void clean()
{
	ce_optparse_del(optparse);
}

int main(int argc, char* argv[])
{
	ce_alloc_init();
	atexit(clean);

	//char* data = ce_alloc(3);
	//data[0] = '1';data[1] = 'a';data[2] = '1';
	//ce_memfile* mf = ce_memfile_open_data(data, 3);

	/*ce_memfile* mf = ce_memfile_open_path("test.txt");
	//ce_memfile_seek(mf, 0, CE_MEMFILE_SEEK_END);
	while (!ce_memfile_eof(mf)) {
		char ch[2];
		size_t sz = ce_memfile_read(mf, ch, 1, 2);
		printf("%c%c %lu\n", ch[0], ch[1], sz);
	}
	ce_memfile_close(mf);
	return 0;*/

	optparse = ce_root_create_optparse();

	ce_optparse_add(optparse, "track1", CE_TYPE_STRING, NULL, true,
		NULL, NULL, "any *.oga file in 'CE/Stream'");
	ce_optparse_add(optparse, "track2", CE_TYPE_STRING, NULL, true,
		NULL, NULL, "any *.oga file in 'CE/Stream'");

	if (!ce_optparse_parse(optparse, argc, argv) || !ce_root_init(optparse)) {
		return EXIT_FAILURE;
	}

	const char *track1, *track2;
	ce_optparse_get(optparse, "track1", &track1);
	ce_optparse_get(optparse, "track2", &track2);

	soundresource1 = ce_soundresource_new_builtin_path(track1);
	if (NULL == soundresource1) {
		return EXIT_FAILURE;
	}

	soundresource2 = ce_soundresource_new_builtin_path(track2);
	if (NULL == soundresource2) {
		ce_soundresource_del(soundresource1);
		return EXIT_FAILURE;
	}

	soundinstance1 = ce_soundinstance_new(soundresource1);
	//soundinstance2 = ce_soundinstance_new(soundresource2);

	ce_soundinstance_play(soundinstance1);
	//ce_soundinstance_play(soundinstance2);

	int code = ce_root_exec();

	ce_soundinstance_del(soundinstance2);
	ce_soundinstance_del(soundinstance1);

	ce_soundresource_del(soundresource2);
	ce_soundresource_del(soundresource1);

	return code;
}
