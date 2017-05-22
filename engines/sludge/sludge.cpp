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

#include "common/scummsys.h"
 
#include "common/config-manager.h"
#include "common/debug.h"
#include "common/debug-channels.h"
#include "common/error.h"
#include "common/file.h"

#include "engines/util.h"

#include "sludge/sludge.h"
#include "sludge/error.h"
#include "sludge/version.h"
//#include "sludge/moreio.h"

namespace Sludge {
 
SludgeEngine::SludgeEngine(OSystem *syst, const SludgeGameDescription *gameDesc)
 : Engine(syst), _gameDescription(gameDesc), _console(nullptr) {
 
	// register your random source
	_rnd = new Common::RandomSource("sludge");

	// Add debug channels
	DebugMan.addDebugChannel(kSludgeDebugScript, "Script", "Script debug level");

	// check init
	debug("SludgeEngine::SludgeEngine");
}
 
SludgeEngine::~SludgeEngine() {

	// Dispose resources
	delete _rnd;
 
	// Remove debug levels
	DebugMan.clearAllDebugChannels();
	
	// Dispose console
	delete _console;

	// Remove game data init
	if (allBIFNames) {
		for (int i = 0; i < numBIFNames; ++i) {
			delete[] allBIFNames[i];
			allBIFNames[i] = nullptr;
		}
		delete[] allBIFNames;
		allBIFNames = nullptr;
	}
	if (allUserFunc) {
		for (int i = 0; i < numUserFunc; ++i) {
			delete[] allBIFNames[i];
			allBIFNames[i] = nullptr;
		}
		delete[] allUserFunc;
		allUserFunc = nullptr;
	}
	if (allResourceNames) {
		for (int i = 0; i < numResourceNames; ++i) {
			delete[] allResourceNames[i];
			allResourceNames[i] = nullptr;
		}
		delete[] allResourceNames;
		allResourceNames = nullptr;
	}
	if (languageTable) {
		delete[] languageTable;
		languageTable = nullptr;
	}
	if (languageName) {
		for (int i = 0; i < gameSettings.numLanguages; ++i) {
			delete[] languageName[i];
			languageName[i] = nullptr;
		}
		delete []languageName;
		languageName = nullptr;
	}
}
 
Common::Error SludgeEngine::run() {
	// init graphics
	initGraphics(640, 480, false);

	// create console
	_console = new SludgeConsole(this);

	initSludge();

	return Common::kNoError;
}

FILE * SludgeEngine::openAndVerify (const char *filename, const char *er, int &fileVersion) {
	FILE * fp = fopen (filename, "rb");
	if (!fp) {
		debug (kSludgeDebugDataInit, "Can't open file %s", filename);
		return NULL;
	}
	bool headerBad = false;
	if (fgetc (fp) != 'S') headerBad = true;
	if (fgetc (fp) != 'L') headerBad = true;
	if (fgetc (fp) != 'U') headerBad = true;
	if (fgetc (fp) != 'D') headerBad = true;
	if (fgetc (fp) != 'G') headerBad = true;
	if (fgetc (fp) != 'E') headerBad = true;
	if (headerBad) {
		debug (kSludgeDebugDataInit, "%s %s", er, filename);
		return NULL;
	}
	char c;
	c = fgetc (fp);
	debug(kSludgeDebugDataInit, "%c", c);
	while ((c = fgetc(fp))) {debug(kSludgeDebugDataInit, "%c", c);}

	int majVersion = fgetc (fp);
	debug(kSludgeDebugDataInit, "majVersion %i", majVersion);
	int minVersion = fgetc (fp);
	debug(kSludgeDebugDataInit, "minVersion %i", minVersion);
	fileVersion = majVersion * 256 + minVersion;

	char txtVer[120];

	if (fileVersion > WHOLE_VERSION) {
		debug (kSludgeDebugDataInit, ERROR_VERSION_TOO_LOW_2, majVersion, minVersion);
		debug (kSludgeDebugDataInit, "%s %s", ERROR_VERSION_TOO_LOW_1, txtVer);
		return NULL;
	} else if (fileVersion < MINIM_VERSION) {
		debug (kSludgeDebugDataInit, ERROR_VERSION_TOO_HIGH_2, majVersion, minVersion);
		debug (kSludgeDebugDataInit, "%s %s", ERROR_VERSION_TOO_HIGH_1, txtVer);
		return NULL;
	}
	return fp;
}

bool SludgeEngine::initSludge() {
	
	FILE * fp = openAndVerify ("Welcome.slg", ERROR_BAD_HEADER, gameVersion);
	if (! fp) return false;

	char c = fgetc (fp);
	putchar(c);
	if (c) {
		numBIFNames = get2bytes (fp);
		debug(kSludgeDebugDataInit, "numBIFNames %i", numBIFNames);
		allBIFNames = new char * [numBIFNames];
		//if (! checkNew (allBIFNames)) return false;

		for (int fn = 0; fn < numBIFNames; fn ++) {
			allBIFNames[fn] = readString (fp);
		}
		numUserFunc = get2bytes (fp);
		debug(kSludgeDebugDataInit, "numUserFunc %i", numUserFunc);
		allUserFunc = new char * [numUserFunc];
		//if (! checkNew (allUserFunc)) return false;

		for (int fn = 0; fn < numUserFunc; fn ++) {
			allUserFunc[fn] = readString (fp);
		}
		if (gameVersion >= VERSION(1,3)) {
			numResourceNames = get2bytes (fp);
			debug(kSludgeDebugDataInit, "numResourceNames %i", numResourceNames);
			allResourceNames = new char * [numResourceNames];
			//if (! checkNew (allResourceNames)) return false;

			for (int fn = 0; fn < numResourceNames; fn ++) {
				allResourceNames[fn] = readString (fp);
			}
		}
	}

	winWidth = get2bytes (fp);
	debug(kSludgeDebugDataInit, "winWidth : %i", winWidth);
	winHeight = get2bytes (fp);
	debug(kSludgeDebugDataInit, "winHeight : %i", winHeight);
	specialSettings = fgetc (fp);
	debug(kSludgeDebugDataInit, "specialSettings : %i", specialSettings);
	int fps = fgetc (fp);
	desiredfps = 1000/fps;
	debug(kSludgeDebugDataInit, "desiredfps : %i", fps);

	delete[] readString (fp); // Unused - was used for registration purposes.

	size_t bytes_read = fread (& fileTime, sizeof (FILETIME), 1, fp);
	if (bytes_read != sizeof (FILETIME) && ferror(fp)) {
		debug("Reading error in initSludge.\n");
	}

	const char * dataFol = (gameVersion >= VERSION(1,3)) ? readString(fp) : "";

	gameSettings.numLanguages = (gameVersion >= VERSION(1,3)) ? (fgetc (fp)) : 0;
	debug(kSludgeDebugDataInit, "numLanguages : %i", gameSettings.numLanguages);
	makeLanguageTable(fp);


	if (gameVersion >= VERSION(1,6))
	{
		fgetc(fp);
		// aaLoad
		fgetc (fp);
		getFloat (fp);
		getFloat (fp);
	}

	char * checker = readString (fp);
	debug(kSludgeDebugDataInit, "checker : %s", checker);

	if (strcmp (checker, "okSoFar")) {
		debug (ERROR_BAD_HEADER);
		return false;
	}
	delete checker;
	checker = NULL;


    unsigned char customIconLogo = fgetc (fp);
    debug(kSludgeDebugDataInit, "logo: %c", customIconLogo);

	if (customIconLogo & 1) {
		debug(kSludgeDebugDataInit, "There is an icon -read it!");
		// There is an icon - read it!
		int n;
/*
		long file_pointer = ftell (fp);

		png_structp png_ptr;
		png_infop info_ptr, end_info;

		int fileIsPNG = true;

		// Is this a PNG file?

		char tmp[10];
		bytes_read = fread(tmp, 1, 8, fp);
		if (bytes_read != 8 && ferror (fp)) {
			debugOut("Reading error in initSludge.\n");
		}
		if (png_sig_cmp((png_byte *) tmp, 0, 8)) {
			// No, it's old-school HSI
			fileIsPNG = false;
			fseek(fp, file_pointer, SEEK_SET);

			iconW = get2bytes (fp);
			iconH = get2bytes (fp);
		} else {
			// Read the PNG header

			png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png_ptr) {
				return false;
			}

			info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr) {
				png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
				return false;
			}

			end_info = png_create_info_struct(png_ptr);
			if (!end_info) {
				png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
				return false;
			}
			png_init_io(png_ptr, fp);		// Tell libpng which file to read
			png_set_sig_bytes(png_ptr, 8);	// 8 bytes already read

			png_read_info(png_ptr, info_ptr);

			png_uint_32 width, height;
			int bit_depth, color_type, interlace_type, compression_type, filter_method;
			png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method);

			iconW = width;
			iconH = height;

			if (bit_depth < 8) png_set_packing(png_ptr);
			png_set_expand(png_ptr);
			if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);
			if (bit_depth == 16) png_set_strip_16(png_ptr);

			png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);

			png_read_update_info(png_ptr, info_ptr);
			png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method);

		}

        gameIcon = new unsigned char [iconW*iconH*4];
        if (! gameIcon) {
        	debug ("Can't reserve memory for game icon.");
        	return false;
        }

        int32_t transCol = 63519;
        Uint8 *p = (Uint8 *) gameIcon;

        if (fileIsPNG) {
            unsigned char * row_pointers[iconH];
            for (int i = 0; i<iconH; i++)
                row_pointers[i] = p + 4*i*iconW;

            png_read_image(png_ptr, (png_byte **) row_pointers);
            png_read_end(png_ptr, NULL);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        } else {

            for (int t2 = 0; t2 < iconH; t2 ++) {
                int t1 = 0;
                while (t1 < iconW) {
                    unsigned short c = (unsigned short) get2bytes (fp);
                    if (c & 32) {
                        n = fgetc (fp) + 1;
                        c -= 32;
                    } else {
                        n = 1;
                    }
                    while (n --) {
                       *p++ = (Uint8) redValue(c);
                        *p++ = (Uint8) greenValue(c);
                        *p++ = (Uint8) blueValue(c);
                        *p++ = (Uint8) (c == transCol) ? 0 : 255;

                        t1++;
                    }
                }
            }
        }
*/
	}

	if (customIconLogo & 2) {
		debug(kSludgeDebugDataInit, "There is a logo -read it!");

		/*
		// There is an logo - read it!
		int n;

		long file_pointer = ftell (fp);

		png_structp png_ptr;
		png_infop info_ptr, end_info;

		int fileIsPNG = true;

		// Is this a PNG file?

		char tmp[10];
		bytes_read = fread(tmp, 1, 8, fp);
		if (bytes_read != 8 && ferror (fp)) {
			debugOut("Reading error in initSludge.\n");
		}
		if (png_sig_cmp((png_byte *) tmp, 0, 8)) {
			// No, it's old-school HSI
			fileIsPNG = false;
			fseek(fp, file_pointer, SEEK_SET);

			logoW = get2bytes (fp);
			logoH = get2bytes (fp);
		} else {
			// Read the PNG header

			png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png_ptr) {
				return false;
			}

			info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr) {
				png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
				return false;
			}

			end_info = png_create_info_struct(png_ptr);
			if (!end_info) {
				png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
				return false;
			}
			png_init_io(png_ptr, fp);		// Tell libpng which file to read
			png_set_sig_bytes(png_ptr, 8);	// 8 bytes already read

			png_read_info(png_ptr, info_ptr);

			png_uint_32 width, height;
			int bit_depth, color_type, interlace_type, compression_type, filter_method;
			png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method);

			logoW = width;
			logoH = height;

			if (bit_depth < 8) png_set_packing(png_ptr);
			png_set_expand(png_ptr);
			if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);
			if (bit_depth == 16) png_set_strip_16(png_ptr);
#ifdef WIN32
            // Windows wants a BGR bitmap
            if (color_type == PNG_COLOR_TYPE_RGB ||
                color_type == PNG_COLOR_TYPE_RGB_ALPHA)
                    png_set_bgr(png_ptr);
#endif

			png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);

			png_read_update_info(png_ptr, info_ptr);
			png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method);

		}

        if ((logoW != 310) || (logoH != 88)) {
        	debug ("Game logo have wrong dimensions. (Should be 310x88)");
        	return false;
        }

        gameLogo = new unsigned char [logoW*logoH*4];
        if (! gameLogo) {
        	debug ("Can't reserve memory for game logo.");
        	return false;
        }

        // int32_t transCol = 63519;
        Uint8 *p = (Uint8 *) gameLogo;

        if (fileIsPNG) {
            unsigned char * row_pointers[logoH];
            for (int i = 0; i<logoH; i++)
                row_pointers[i] = p + 4*i*logoW;

            png_read_image(png_ptr, (png_byte **) row_pointers);
            png_read_end(png_ptr, NULL);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        } else {

            for (int t2 = 0; t2 < logoH; t2 ++) {
                int t1 = 0;
                while (t1 < logoW) {
                    unsigned short c = (unsigned short) get2bytes (fp);
                    if (c & 32) {
                        n = fgetc (fp) + 1;
                        c -= 32;
                    } else {
                        n = 1;
                    }
                    while (n --) {
#ifdef WIN32
                        // Windows wants a BGR bitmap
                       *p++ = (Uint8) blueValue(c);
                        *p++ = (Uint8) greenValue(c);
                        *p++ = (Uint8) redValue(c);
#else
                       *p++ = (Uint8) redValue(c);
                        *p++ = (Uint8) greenValue(c);
                        *p++ = (Uint8) blueValue(c);
#endif
                        *p++ = (Uint8) 255;

                        t1++;
                    }
                }
            }
        }
        */
	}

 	numGlobals = get2bytes (fp);
 	debug(kSludgeDebugDataInit, "numGlobals : %i\n", numGlobals);

	//globalVars = new variable[numGlobals];
	//if (! checkNew (globalVars)) return false;
	//for (a = 0; a < numGlobals; a ++) initVarNew (globalVars[a]);

	// Get the original (untranslated) name of the game and convert it to Unicode.
	// We use this to find saved preferences and saved games.
	setFileIndices(fp, gameSettings.numLanguages, 0);

/*
	char * gameNameOrig = getNumberedString(1);
	char * gameName = encodeFilename (gameNameOrig);
	delete gameNameOrig;

	changeToUserDir ();

#ifdef _WIN32
	mkdir (gameName);
#else
	mkdir (gameName, 0000777);
#endif

	if (chdir (gameName)) {
		return fatal ("This game's preference folder is inaccessible!\nI can't access the following directory (maybe there's a file with the same name, or maybe it's read-protected):", gameName);
	}

	delete [] gameName;

	// Get user settings
	readIniFile (filename);

	// Now set file indices properly to the chosen language.
	languageNum = getLanguageForFileB ();
	if (languageNum < 0) return fatal ("Can't find the translation data specified!");
	setFileIndices (NULL, gameSettings.numLanguages, languageNum);

	if (dataFol[0]) {
		char *dataFolder = encodeFilename(dataFol);

		if (chdir (dataFolder)) return fatal ("This game's data folder is inaccessible!\nI can't access the following directory (maybe there's a file with the same name, or maybe it's read-protected):", dataFolder);

		delete dataFolder;
	}

 	positionStatus (10, winHeight - 15);
*/
	return true;
}

int SludgeEngine::get2bytes (FILE * fp) {
	int f1, f2;

	f1 = fgetc (fp);
	f2 = fgetc (fp);

	return (f1 * 256 + f2);
}

char * SludgeEngine::readString (FILE * fp) {
	int a, len = get2bytes (fp);
	char * s = new char[len + 1];
	//if (! checkNew (s)) {
	//	return NULL;
	//}
	for (a = 0; a < len; a ++) {
		s[a] = (char) (fgetc (fp) - 1);
	}
	s[len] = 0;
	debug(kSludgeDebugDataInit, "%i %s", len, s);
	return s;
}

// The following 4 byte functions treat signed integers as unsigned.
// That's done on purpose.
int32_t SludgeEngine::get4bytes (FILE * fp) {
	int f1, f2, f3, f4;
	f1 = fgetc (fp);
	f2 = fgetc (fp);
	f3 = fgetc (fp);
	f4 = fgetc (fp);
	unsigned int x = f1 + f2*256 + f3*256*256 + f4*256*256*256;
	return x;
}

float SludgeEngine::getFloat (FILE * fp) {
	float f;
	size_t bytes_read = fread(& f, sizeof (float), 1, fp);
	if (bytes_read != sizeof (float) && ferror(fp)) {
		debug("Reading error in getFloat.\n");
	}
//#ifdef	__BIG_ENDIAN__
//	return floatSwap(f);
//#else
	return f;
//#endif
}

void SludgeEngine::makeLanguageTable (FILE *table)
{
	languageTable = new int[gameSettings.numLanguages + 1];
	//if (! checkNew (languageTable)) return;

	languageName = new char*[gameSettings.numLanguages + 1];
	//if (! checkNew (languageName)) return;

	for (unsigned int i = 0; i <= gameSettings.numLanguages; i ++) {
		languageTable[i] = i ? get2bytes (table) : 0;
		debug(kSludgeDebugDataInit, "languageTable %i: %i\n", i, languageTable[i]);
		languageName[i] = 0;
		if (gameVersion >= VERSION(2,0)) {
			if (gameSettings.numLanguages) {
				languageName[i] = readString (table);
			}
		}
	}
}

void SludgeEngine::setFileIndices(FILE * fp, int numLanguages, unsigned int skipBefore) {
	if (fp) {
		// Keep hold of the file handle, and let things get at it
		bigDataFile = fp;
		startIndex = ftell (fp);
	} else {
		// No file pointer - this means that we reuse the bigDataFile
		fp = bigDataFile;
		fseek (fp, startIndex, 0);
	}
	sliceBusy = false;

	if (skipBefore > numLanguages) {
		debug ("Not a valid language ID! Using default instead.");
		skipBefore = 0;
	}

	// STRINGS
	int skipAfter = numLanguages - skipBefore;
	while (skipBefore) {
		fseek (fp, get4bytes (fp), 0);
		skipBefore --;
	}
	startOfTextIndex = ftell (fp) + 4;

	fseek (fp, get4bytes (fp), 0);

	while (skipAfter) {
		fseek (fp, get4bytes (fp), 0);
		skipAfter --;
	}

	startOfSubIndex = ftell (fp) + 4;
	fseek (fp, get4bytes (fp), 1);

	startOfObjectIndex = ftell (fp) + 4;
	fseek (fp, get4bytes (fp), 1);

	// Remember that the data section starts here
	startOfDataIndex = ftell (fp);
}

} // End of namespace Sludge

