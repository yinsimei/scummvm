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

#include "moreio.h"
#include "sludge.h"
#include "common/debug.h"

namespace Sludge {


int get2bytes(FILE *fp) {
	int f1, f2;

	f1 = fgetc(fp);
	f2 = fgetc(fp);

	return f1 * 256 + f2;
}

char *readString(FILE *fp) {
	int len = get2bytes(fp);
	char *s = new char[len + 1];
	//if (!checkNew(s)) {
	//	return nullptr;
	//}
	for (int a = 0; a < len; ++a) {
		s[a] = (char)(fgetc(fp) - 1);
	}
	s[len] = 0;
	debug(kSludgeDebugDataInit, "%i %s", len, s);
	return s;
}

// The following 4 byte functions treat signed integers as unsigned.
// That's done on purpose.
int32_t get4bytes(FILE *fp) {
	int f1, f2, f3, f4;
	f1 = fgetc(fp);
	f2 = fgetc(fp);
	f3 = fgetc(fp);
	f4 = fgetc(fp);
	unsigned int x = f1 + f2 * 256 + f3 * 256 * 256 + f4 * 256 * 256 * 256;
	return x;
}

float getFloat(FILE *fp) {
	float f;
	size_t bytes_read = fread(&f, sizeof(float), 1, fp);
	if (bytes_read != sizeof(float) && ferror(fp)) {
		debug("Reading error in getFloat.");
	}
//#ifdef	__BIG_ENDIAN__
//	return floatSwap(f);
//#else
	return f;
//#endif
}

char * copyString(const char *c) {
	char * newString = new char [strlen(c) + 1];
	//if (!checkNew(newString)) return NULL;
	strcpy(newString, c);
	return newString;
}

char *joinStrings(const char *s1, const char *s2) {
	char *newString = new char[strlen(s1) + strlen(s2) + 1];
	//if (!checkNew(newString)) return nullptr;
	sprintf(newString, "%s%s", s1, s2);
	return newString;
}

} // End of namespace Sludge
