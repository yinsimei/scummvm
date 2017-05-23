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


#ifndef SLUDGE_VARIABLE_H
#define SLUDGE_VARIABLE_H

namespace Sludge {

struct Variable;
struct VariableStack;

enum VariableType {SVT_NULL, SVT_INT, SVT_FUNC, SVT_STRING,
				   SVT_BUILT, SVT_FILE, SVT_STACK,
				   SVT_OBJTYPE, SVT_ANIM, SVT_COSTUME,
				   SVT_FASTARRAY, SVT_NUM_TYPES};

struct FastArrayHandler {
	Variable *fastVariables;
	int size;
	int	timesUsed;
};

struct StackHandler {
	VariableStack *first;
	VariableStack *last;
	int	timesUsed;
};

union VariableData {
	signed int intValue;
	char *theString;
	StackHandler *theStack;
	struct personaAnimation *animHandler;
	struct persona *costumeHandler;
	FastArrayHandler *fastArray;
};

struct Variable {
	VariableType varType;
	VariableData varData;
};

struct VariableStack {
	Variable thisVar;
	VariableStack *next;
};

bool copyMain(const Variable &from, Variable &to);
int compareVars(const Variable & var1, const Variable & var2);

} // End of namespace Sludge

#endif
