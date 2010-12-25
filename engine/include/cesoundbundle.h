/*
 *  This file is part of Cursed Earth
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands
 *  Copyright (C) 2009-2010 Yanis Kurganov
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

#ifndef CE_SOUNDBUNDLE_H
#define CE_SOUNDBUNDLE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int state;
	float time;
} ce_sound_bundle;

extern ce_sound_bundle* ce_sound_bundle_new(void);
extern void ce_sound_bundle_del(ce_sound_bundle* sound_bundle);

#ifdef __cplusplus
}
#endif

#endif /* CE_SOUNDBUNDLE_H */