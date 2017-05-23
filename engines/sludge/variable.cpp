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

#include "moreio.h"
#include "sludge.h"
#include "variable.h"

namespace Sludge {

const char *typeName[] = {"undefined", "number", "user function", "string",
							"built-in function", "file", "stack",
							"object type", "animation", "costume"};

bool copyMain(const Variable &from, Variable &to) {
	to.varType = from.varType;
	switch (to.varType) {
		case SVT_INT:
		case SVT_FUNC:
		case SVT_BUILT:
		case SVT_FILE:
		case SVT_OBJTYPE:
		to.varData.intValue = from.varData.intValue;
		return true;

		case SVT_FASTARRAY:
		to.varData.fastArray = from.varData.fastArray;
		++to.varData.fastArray->timesUsed;
		return true;

		case SVT_STRING:
		to.varData.theString = copyString(from.varData.theString);
		return to.varData.theString ? true : false;

		case SVT_STACK:
		to.varData.theStack = from.varData.theStack;
		++to.varData.theStack->timesUsed;
		return true;

		case SVT_COSTUME:
		to.varData.costumeHandler = from.varData.costumeHandler;
		return true;

		// TODO: don't have animations at present
		//case SVT_ANIM:
		//to.varData.animHandler = copyAnim(from.varData.animHandler);
		//return true;

		case SVT_NULL:
		return true;

		default:
		break;
	}
	debug("Unknown value type");
	return false;
}

int compareVars (const Variable & var1, const Variable & var2) {
	int re = 0;
	if (var1.varType == var2.varType) {
		switch (var1.varType) {
			case SVT_NULL:
			re = 1;
			break;

			case SVT_COSTUME:
			re = (var1.varData.costumeHandler == var2.varData.costumeHandler);
			break;

			case SVT_ANIM:
			re = (var1.varData.animHandler == var2.varData.animHandler);
			break;

			case SVT_STRING:

			re = (strcmp(var1.varData.theString, var2.varData.theString) == 0);
			break;

			case SVT_STACK:
			re = (var1.varData.theStack == var2.varData.theStack);
			break;

			default:
			re = (var1.varData.intValue == var2.varData.intValue);
		}
	}
	return re;
}

void SludgeEngine::initVarNew(Variable &thisVar) {
	thisVar.varType = SVT_NULL;
}

void SludgeEngine::setVariable(Variable &thisVar, VariableType vT, int value) {
	unlinkVar(thisVar);
	thisVar.varType = vT;
	thisVar.varData.intValue = value;
}

bool SludgeEngine::copyVariable(const Variable &from, Variable &to) {
	unlinkVar(to);
	return copyMain(from, to);
}

bool SludgeEngine::loadStringToVar(Variable &thisVar, int value) {
	makeTextVar(thisVar, getNumberedString(value));
	return (bool)(thisVar.varData.theString != nullptr);
}

void SludgeEngine::makeTextVar(Variable &thisVar, const char *txt) {
	unlinkVar(thisVar);
	thisVar.varType = SVT_STRING;
	thisVar.varData.theString = copyString(txt);
}

void SludgeEngine::addVariablesInSecond(Variable &var1, Variable &var2) {
	if (var1.varType == SVT_INT && var2.varType == SVT_INT) {
			var2.varData.intValue += var1.varData.intValue;
		} else {
			char *string1 = getTextFromAnyVar(var1);
			char *string2 = getTextFromAnyVar(var2);

			unlinkVar(var2);
			var2.varData.theString = joinStrings(string1, string2);
			var2.varType = SVT_STRING;
			delete[] string1;
			delete[] string2;
		}
}

void SludgeEngine::compareVariablesInSecond(const Variable & var1, Variable & var2) {
	setVariable(var2, SVT_INT, compareVars(var1, var2));
}


void SludgeEngine::unlinkVar(Variable &thisVar) {
	switch (thisVar.varType) {
		case SVT_STRING:
			delete[] thisVar.varData.theString;
			thisVar.varData.theString = nullptr;
			break;

		case SVT_STACK:
			--thisVar.varData.theStack->timesUsed;
			if (thisVar.varData.theStack->timesUsed <= 0) {
				while (thisVar.varData.theStack->first) {
					trimStack(thisVar.varData.theStack->first);
				}
				delete thisVar.varData.theStack;
				thisVar.varData.theStack = nullptr;
			}
			break;

		case SVT_FASTARRAY:
			--thisVar.varData.fastArray->timesUsed;
			if (thisVar.varData.theStack->timesUsed <= 0) {
				delete thisVar.varData.fastArray->fastVariables;
				delete[] thisVar.varData.fastArray;
				thisVar.varData.fastArray = nullptr;
			}
			break;

		// TODO: don't have animations for now
		//case SVT_ANIM:
		//deleteAnim(thisVar.varData.animHandler);
		//break;

		default:
			break;
	}
}

char *SludgeEngine::getTextFromAnyVar(const Variable &from) {
	char *builder = nullptr;
	char *builder2 = nullptr;
	char *grabText = nullptr;
	switch (from.varType) {
		case SVT_STRING:
			return copyString(from.varData.theString);

		case SVT_FASTARRAY:
			builder = copyString("FAST:");
			for (int i = 0; i < from.varData.fastArray->size; i ++) {
				builder2 = joinStrings(builder, " ");
				if (!builder2) return nullptr;
				delete builder;
				grabText = getTextFromAnyVar(from.varData.fastArray->fastVariables[i]);
				builder = joinStrings(builder2, grabText);
				if (!builder) return nullptr;
				delete grabText;
				grabText = nullptr;
				delete builder2;
				builder2 = nullptr;
			}
			return builder;

		case SVT_STACK: {
			builder = copyString("ARRAY:");
			VariableStack *stacky = from.varData.theStack->first;
			while (stacky) {
				builder2 = joinStrings(builder, " ");
				if (!builder2) return nullptr;
				delete builder;
				grabText = getTextFromAnyVar(stacky->thisVar);
				builder = joinStrings(builder2, grabText);
				if (!builder) return nullptr;
				delete grabText;
				grabText = nullptr;
				delete builder2;
				builder2 = nullptr;
				stacky = stacky->next;
			}
			return builder;
		}

		case SVT_INT: {
			char *buff = new char[10];
			//if(!checkNew(buff)) return nullptr;
			sprintf(buff, "%i", from.varData.intValue);
			return buff;
		}

		case SVT_FILE:
			return joinStrings("", resourceNameFromNum(from.varData.intValue));

		/*
		case SVT_ANIM:
		{
			char *buff = new char[20];
			if (!checkNew(buff)) return nullptr;
			sprintf(buff, "%p", from.varData.animHandler);
			return buff;
		}*/

		//TODO: don't have object at present
		/*
		case SVT_OBJTYPE:
			objectType *thisType = findObjectType(from.varData.intValue);
			if (thisType)
				return copyString(thisType->screenName);
			break;
		*/
		default:
		break;
	}

	return copyString(typeName[from.varType]);
}

bool SludgeEngine::getBoolean(const Variable &from) {
	switch (from.varType) {
		case SVT_NULL:
		return false;

		case SVT_INT:
		return (bool)(from.varData.intValue != 0);

		case SVT_STACK:
		return (bool)(from.varData.theStack->first != nullptr);

		case SVT_STRING:
		return (bool)(from.varData.theString[0] != 0);

		case SVT_FASTARRAY:
		return (bool)(from.varData.fastArray->size != 0);

		default:
		break;
	}
	return true;
}

bool SludgeEngine::getValueType(int &toHere, VariableType vT, const Variable &v) {
	//if (!v) return false;
		if (v.varType != vT) {
			char *e1 = joinStrings("Can only perform specified operation on a value which is of type ", typeName[vT]);
			char *e2 = joinStrings("... value supplied was of type ", typeName[v.varType]);
			debug("%s %s", e1, e2);
			return false;
		}
		toHere = v.varData.intValue;
		return true;
}

void SludgeEngine::trimStack(VariableStack *&stack) {
	VariableStack *killMe = stack;
	stack = stack->next;

	// When calling this, we've ALWAYS checked that stack != NULL
	unlinkVar(killMe->thisVar);
	delete killMe;
	killMe = nullptr;
}

bool SludgeEngine::addVarToStack(const Variable &va, VariableStack * &thisStack) {
	VariableStack *newStack = new VariableStack;
	//if (!checkNew(newStack)) return false;

	if (!copyMain(va, newStack->thisVar))
		return false;
	newStack->next = thisStack;
	thisStack = newStack;
	return true;
}

bool SludgeEngine::addVarToStackQuick(Variable &va, VariableStack * &thisStack) {
	VariableStack *newStack = new VariableStack;
	//if (!checkNew(newStack)) return false;

	memcpy(&(newStack->thisVar), &va, sizeof(Variable));
	va.varType = SVT_NULL;
	newStack->next = thisStack;
	thisStack = newStack;
	return true;
}

bool SludgeEngine::stackSetByIndex(VariableStack *vS, unsigned int theIndex, const Variable &va) {
	while (--theIndex) {
		vS = vS->next;
		if (!vS) {
			debug("Index past end of stack.");
			return false;
		}
	}
	return copyVariable(va, vS->thisVar);
}

Variable *SludgeEngine::stackGetByIndex(VariableStack *vS, unsigned int theIndex) {
	while (--theIndex) {
		vS = vS->next;
		if (!vS) {
			return nullptr;
		}
	}
	return &(vS->thisVar);
}

Variable *SludgeEngine::fastArrayGetByIndex(FastArrayHandler *vS, int theIndex) {
	if (theIndex < 0 || theIndex >= vS->size)
		return nullptr;
	return &vS->fastVariables[theIndex];
}
} // End of namespace Sludge
