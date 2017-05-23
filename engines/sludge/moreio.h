/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef SLUDGE_MOREIO_H
#define SLUDGE_MOREIO_H

#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <inttypes.h>
#include <stdio.h>

namespace Sludge {

// Input
int get2bytes(FILE *fp);
char *readString(FILE *fp);
int32_t get4bytes(FILE *fp);
float getFloat(FILE *fp);

// Misc
char *copyString(const char *c);
char *joinStrings(const char *s1, const char *s2);

} // End of namespace Sludge

#endif
