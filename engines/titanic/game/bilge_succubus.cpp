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

#include "titanic/game/bilge_succubus.h"

namespace Titanic {

CBilgeSuccUBus::CBilgeSuccUBus() : CSuccUBus(), _field1DC(0),
	_field1E0(0), _field1E4(0), _field1E8(0) {
}

void CBilgeSuccUBus::save(SimpleFile *file, int indent) {
	file->writeNumberLine(1, indent);
	file->writeNumberLine(_field1DC, indent);
	file->writeNumberLine(_field1E0, indent);
	file->writeNumberLine(_field1E4, indent);
	file->writeNumberLine(_field1E8, indent);

	CSuccUBus::save(file, indent);
}

void CBilgeSuccUBus::load(SimpleFile *file) {
	file->readNumber();
	_field1DC = file->readNumber();
	_field1E0 = file->readNumber();
	_field1E4 = file->readNumber();
	_field1E8 = file->readNumber();

	CSuccUBus::load(file);
}

} // End of namespace Titanic