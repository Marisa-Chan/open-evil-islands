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

#include <string.h>

#include "celogging.h"
#include "ceerror.h"

void ce_error_report_last_c_error(int code,
								const char* module,
								const char* func,
								const char* error)
{
	char buffer[128] = "unknown";
	// FIXME: currently GNU strerror_r in use
	char* message = strerror_r(code, buffer, sizeof(buffer));
	ce_logging_error("%s: %s: %s: %s", module, func, error, message);
}
