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

#include "language.h"
#include "moreio.h"
#include "sludge.h"
#include "version.h"

namespace Sludge {

void SludgeEngine::makeLanguageTable(FILE *table) {
	_languageTable = new int[_gameSettings.numLanguages + 1];
	//if (!checkNew(languageTable)) return;

	_languageName = new char*[_gameSettings.numLanguages + 1];
	//if (!checkNew(languageName)) return;

	for (unsigned int i = 0; i <= _gameSettings.numLanguages; ++i) {
		_languageTable[i] = i ? get2bytes(table) : 0;
		debug(kSludgeDebugDataInit, "languageTable %i: %i\n", i, _languageTable[i]);
		_languageName[i] = 0;
		if (_gameVersion >= VERSION(2,0)) {
			if (_gameSettings.numLanguages) {
				_languageName[i] = readString(table);
			}
		}
	}
}

} // End of namespace Sludge
