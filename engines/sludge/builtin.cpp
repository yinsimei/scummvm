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

#include "builtin.h"
#include "sludge.h"

namespace Sludge {

#define builtIn(a) static BuiltReturn builtIn_ ## a(int numParams, LoadedFunction *fun)
#define UNUSEDALL (void)(0 && sizeof(numParams) && sizeof(fun));

builtIn(addOverlay)
{
	UNUSEDALL
	debug("addOverlay is called");

	/*
	int fileNumber, xPos, yPos;
	if (!getValueType(yPos, SVT_INT, fun->stack->thisVar))
		return BR_ERROR;
	trimStack(fun->stack);
	if (!getValueType(xPos, SVT_INT, fun->stack->thisVar))
		return BR_ERROR;
	trimStack(fun->stack);
	if (!getValueType(fileNumber, SVT_FILE, fun->stack->thisVar))
		return BR_ERROR;
	trimStack(fun->stack);
	loadBackDrop(fileNumber, xPos, yPos);
	*/
	return BR_CONTINUE;
}

builtIn(playSound)
{
	debug("playSound is called");
	/*
	UNUSEDALL
	int fileNumber;
	if (!getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	trimStack(fun->stack);
	if (!startSound(fileNumber, false)) return BR_CONTINUE;	// Was BR_ERROR
	*/
	return BR_CONTINUE;
}

builtIn(pause)
{
	debug("pause is called");
	/*
	UNUSEDALL
	int theTime;
	if (!getValueType(theTime, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	trimStack(fun->stack);
	if (theTime > 0) {
		fun->timeLeft = theTime - 1;
		fun->isSpeech = false;
		return BR_KEEP_AND_PAUSE;
	}
	*/
	return BR_CONTINUE;
}

builtIn(quitGame)
{
	debug("quitGame is called");
	/*
	UNUSEDALL
	reallyWantToQuit = true;
	quit_event.type=SDL_QUIT;
	SDL_PushEvent(&quit_event);
	*/
	return BR_CONTINUE;
}

//-------------------------------------

#define FUNC(special,name) {builtIn_ ## name},
static BuiltInFunctionData builtInFunctionArray[] =
{
#include "functionlist.h"
};
#undef FUNC

#define FUNC(special,name) {#name},
char builtInFunctionNames[][25] =
{
#include "functionlist.h"
};
#undef FUNC

#define NUM_FUNCS (sizeof(builtInFunctionArray) / sizeof(builtInFunctionArray[0]))

Sludge::BuiltReturn SludgeEngine::callBuiltIn(int whichFunc, int numParams, LoadedFunction *fun) {
	debug(kSludgeDebugBuiltin, "Built-in function called!");
	/*
	if (_numBIFNames) {
		const char *usrFucName = (fun->originalNumber < _numUserFunc) ? _allUserFunc[fun->originalNumber] : "Unknown user function";
		const char *calledFucName = (whichFunc < _numBIFNames) ? _allBIFNames[whichFunc] : "Unknown built-in function";
		debug("Currently in this sub: %s\nCalling: %s", usrFucName, calledFucName);
	}

	if (whichFunc < NUM_FUNCS) {
		if (_paramNum[whichFunc] != -1) {
			if (_paramNum[whichFunc] != numParams) {
				debug("Built in function must have %i parameter%s", _paramNum[whichFunc], (_paramNum[whichFunc] == 1) ? "" : "s");
				return BR_ERROR;
			}
		}

		if (builtInFunctionArray[whichFunc].func) {
			return builtInFunctionArray[whichFunc].func(numParams, fun);
		}
	}
	debug("Unknown / unimplemented built-in function.");
	*/
	return BR_ERROR;
}

} // End of namespace Sludge
