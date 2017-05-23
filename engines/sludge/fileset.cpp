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

#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include "common/debug.h"

#include "fileset.h"
#include "moreio.h"
#include "sludge.h"
#include "version.h"

namespace Sludge {

// Converts a string from Windows CP-1252 to UTF-8.
// This is needed for old games.
char *convertString(char *s) {
	/*
	 static char *buf = nullptr;
	 if (!buf) {
	 buf = new char [65536];
	 //if (!checkNew(buf)) return NULL;
	 }
	 char **tmp1 = (char **) &s;
	 char **tmp2 = (char **) &buf;
	 char *sOrig = s;
	 char *bufOrig = buf;
	 #if defined __unix__ && !(defined __APPLE__)
	 iconv_t convert = iconv_open("UTF-8", "ISO8859-2");
	 #else
	 iconv_t convert = iconv_open("UTF-8", "CP1250");
	 #endif

	 if (convert == (iconv_t)-1) {
	 switch (errno) {
	 case EINVAL:
	 fprintf(stderr, "Error: Encoding not supported by iconv.");
	 break;
	 default:
	 fprintf(stderr, "Error: Could not open iconv conversion descriptor.");
	 }
	 }

	 size_t len1 = strlen(s)+1;
	 size_t len2 = 65535;
	 size_t iconv_value =
	 #ifdef _WIN32
	 iconv(convert,(const char **) tmp1, &len1, tmp2, &len2);
	 #else
	 iconv(convert,(char **) tmp1, &len1, tmp2, &len2);
	 #endif

	 if (iconv_value == (size_t) -1) {
	 switch (errno) {
	 // See "man 3 iconv" for an explanation.
	 case EILSEQ:
	 fprintf(stderr, "Invalid multibyte sequence.");
	 break;
	 case EINVAL:
	 fprintf(stderr, "Incomplete multibyte sequence.");
	 break;
	 case E2BIG:
	 fprintf(stderr, "No more room.");
	 break;
	 default:
	 fprintf(stderr, "Error: %s.", strerror(errno));
	 }
	 fatal("Conversion to Unicode failed. This can be fixed by recompiling the game in a current version of the SLUDGE Development Kit, but this error should never happen. Congratulations, you've found a bug in the SLUDGE engine! Please let us know about it.");
	 }
	 iconv_close(convert);

	 delete [] sOrig;
	 return copyString(buf = bufOrig);
	 */
	return s;
}

void SludgeEngine::setFileIndices(FILE *fp, unsigned int numLanguages, unsigned int skipBefore) {
	if (fp) {
		// Keep hold of the file handle, and let things get at it
		_bigDataFile = fp;
		_startIndex = ftell(fp);} else {
		// No file pointer - this means that we reuse the _bigDataFile
		fp = _bigDataFile;
		fseek(fp, _startIndex, 0);}
	_sliceBusy = false;

	if (skipBefore > numLanguages) {
		debug("Not a valid language ID!Using default instead.");
		skipBefore = 0;
	}

	// STRINGS
	int skipAfter = numLanguages - skipBefore;
	while (skipBefore) {
		fseek(fp, get4bytes(fp), 0);--skipBefore;
	}
	_startOfTextIndex = ftell(fp)+ 4;

	fseek(
		fp, get4bytes(fp), 0);

while (skipAfter) {
		fseek(fp, get4bytes(fp), 0);--skipAfter;
	}

	_startOfSubIndex = ftell(fp)+ 4;
	fseek(fp, get4bytes(fp), 1);

_startOfObjectIndex = ftell(fp)+ 4;
	fseek(fp, get4bytes(fp), 1);

// Remember that the data section starts here
_startOfDataIndex = ftell(fp); }

bool SludgeEngine::openSubSlice(int num) {
debug("Trying to open sub %i", num);

if (_sliceBusy) {
	debug("Can't read from data file. I'm already reading something");
	return false;
}

debug("Going to position %i", _startOfSubIndex + (num << 2));
fseek(_bigDataFile, _startOfSubIndex + (num << 2), 0)
;
	fseek(_bigDataFile, get4bytes(_bigDataFile), 0)
;
debug("Told to skip forward to %li", ftell(_bigDataFile));
return _sliceBusy = true;
}

bool SludgeEngine::openObjectSlice(int num) {
debug("Trying to open object %i", num);

if (_sliceBusy) {
	debug("Can't read from data file. I'm already reading something");
	return false;
}

debug("Going to position %i", _startOfObjectIndex + (num << 2));
fseek(_bigDataFile, _startOfObjectIndex + (num << 2), 0)
;
	fseek(_bigDataFile, get4bytes(_bigDataFile), 0)
;
debug("Told to skip forward to %li", ftell(_bigDataFile));
return _sliceBusy = true;
}

unsigned int SludgeEngine::openFileFromNum(int num) {
if (_sliceBusy) {
	debug("Can't read from data file. I'm already reading something");
	return 0;
}

	debug("Trying to open file %i", num);
	debug("Jumping to %i (for index)", _startOfDataIndex + (num << 2));
fseek(_bigDataFile, _startOfDataIndex + (num << 2), 0)
;
	fseek(_bigDataFile, get4bytes(_bigDataFile), 1)
;
//	fprintf(dbug, "Jumping to %li (for data) ", ftell(_bigDataFile));
_sliceBusy = true;

return get4bytes(_bigDataFile);
}

char *SludgeEngine::getNumberedString(int value) {
if (_sliceBusy) {
	debug("Can't read from data file. I'm already reading something");
	return nullptr;
}

fseek(_bigDataFile, (value << 2) + _startOfTextIndex, 0)
;
value = get4bytes(_bigDataFile);
fseek(_bigDataFile, value, 0)
;

char *s = readString(_bigDataFile);

// This is an older game - We need to convert the string to UTF-8
if (_gameVersion < VERSION(2, 2)) {
	s = convertString(s);
}

return s;
}

bool SludgeEngine::startAccess() {
int wasBusy = _sliceBusy;
_sliceBusy = true;
return wasBusy;
}

void SludgeEngine::finishAccess() {
_sliceBusy = false;
}

} // End of namespace Sludge
