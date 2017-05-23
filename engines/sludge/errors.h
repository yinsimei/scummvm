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
#ifndef SLUDGE_ERRORS_H
#define SLUDGE_ERRORS_H

namespace Sludge {

#define _NO_MEMORY_GENERAL_			"\nTry closing down any programs you don't really need running (or freeing up a bit of disk space, which will give you more virtual memory - that should help too)."

//---------------------------------------
// Fatal errors
//---------------------------------------

#define ERROR_VERSION_TOO_LOW_1		"This SLUDGE file requires a more recent version of the SLUDGE engine"
#define ERROR_VERSION_TOO_LOW_2		"(it was created for v%i.%i).\n\nVisit http://opensludge.github.io/ to download the most recent version."
#define ERROR_VERSION_TOO_HIGH_1 	"This SLUDGE file was created for an older version of the SLUDGE engine"
#define ERROR_VERSION_TOO_HIGH_2	"(v%i.%i).\n\nPlease contact the author of this game to obtain a version compatible with your SLUDGE engine (v" TEXT_VERSION ")."
#define ERROR_BAD_HEADER			"Bad header information... this isn't a valid SLUDGE game"
#define ERROR_HACKER				"What have you been up to? Think we're a hacker, do we? Nice try."

#define ERROR_NON_EMPTY_STACK		"Returning from function with non-empty stack"
#define ERROR_UNKNOWN_MCODE			"Unknown SLUDGE machine code"
#define ERROR_CALL_NONFUNCTION		"Call of non-function"
#define ERROR_INCDEC_UNKNOWN		"Tried to increment/decrement index of an undefined variable"
#define ERROR_INDEX_EMPTY			"Tried to index an empty stack"
#define ERROR_INDEX_NONSTACK		"Tried to index a non-stack variable"
#define ERROR_NOSTACK				"Corrupt file - no stack"
#define ERROR_UNKNOWN_CODE			"Unimplemented internal SLUDGE command code."
#define ERROR_OUT_OF_MEMORY			"Out of memory!" _NO_MEMORY_GENERAL_

} // End of namespace Sludge

#endif
