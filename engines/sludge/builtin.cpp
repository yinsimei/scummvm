/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
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

typedef BuiltReturn (SludgeBuiltIn::*BuiltInSludgeFunc)(int, LoadedFunction*);
struct BuiltInFunctionData {
	char funcName[25];
	BuiltInSludgeFunc func;
	int numParam;
	bool inUse; // true or false states if it's a used function, but this value currently isn't used anywhere
};

#define builtIn(a) BuiltReturn SludgeBuiltIn::a(int numParams, LoadedFunction *fun)
#define UNUSEDALL (void)(0 && sizeof(numParams) && sizeof(fun));

bool SludgeBuiltIn::failSecurityCheck(char *fn) {
	if (fn == NULL) return true;
	int a = 0;
	while (fn[a]) {
		switch (fn[a]) {
			case ':': // Fall Through intended
			case '\\': // Fall Through intended
			case '/': // Fall Through intended
			case '*': // Fall Through intended
			case '?': // Fall Through intended
			case '"': // Fall Through intended
			case '<': // Fall Through intended
			case '>': // Fall Through intended
			case '|': // Fall Through intended
				debug("Filenames may not contain the following characters: \n\n\\  /  :  \"  <  >  |  ?  *\n\nConsequently, the following filename is not allowed:", fn);
				return true;
		}
		++a;
	}
	return false;
}

BuiltReturn SludgeBuiltIn::sayCore(int numParams, LoadedFunction *fun, bool sayIt)
{
	int fileNum = -1;
	char *newText;
	int objT, p;
	_engine->killSpeechTimers();

	switch (numParams) {
		case 3:
			if (!_engine->getValueType(fileNum, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
			_engine->trimStack(fun->stack);
			// No break; here

		case 2:
			newText = _engine->getTextFromAnyVar(fun->stack->thisVar);
			if (!newText) return BR_ERROR;
			_engine->trimStack(fun->stack);
			if (!_engine->getValueType(objT, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
			_engine->trimStack(fun->stack);
			//p = wrapSpeech(newText, objT, fileNum, sayIt);
			fun->timeLeft = p;
			//debugOut("BUILTIN: sayCore: %s(%i)\n", newText, p);
			fun->isSpeech = true;
			delete newText;
			newText = NULL;
			return BR_KEEP_AND_PAUSE;
	}

	debug("Function should have either 2 or 3 parameters");
	return BR_ERROR;
}

builtIn(say)
{
	UNUSEDALL
	return sayCore(numParams, fun, true);
}

builtIn(think)
{
	UNUSEDALL
	return sayCore(numParams, fun, false);
}

builtIn(freeze)
{
	UNUSEDALL
	//freeze();
	//freezeSubs();
	fun->freezerLevel = 0;
	return BR_CONTINUE;
}

builtIn(unfreeze)
{
	UNUSEDALL
	//unfreeze ();
	//unfreezeSubs ();
	return BR_CONTINUE;
}

builtIn(howFrozen)
{
	UNUSEDALL
	_engine->setVariable(fun->reg, SVT_INT, 0/*howFrozen()*/);
	return BR_CONTINUE;
}

builtIn(setCursor)
{
	UNUSEDALL
	//personaAnimation *aa = getAnimationFromVar(fun->stack->thisVar);
	//pickAnimCursor(aa);
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(getMouseX)
{
	UNUSEDALL
	_engine->setVariable(fun->reg, SVT_INT, 0/*input.mouseX + cameraX*/);
	return BR_CONTINUE;
}

builtIn(getMouseY)
{
	UNUSEDALL
	_engine->setVariable(fun->reg, SVT_INT, 0/*input.mouseY + cameraY*/);
	return BR_CONTINUE;
}

builtIn(getMouseScreenX)
{
	UNUSEDALL
	_engine->setVariable(fun->reg, SVT_INT, 0/*input.mouseX * cameraZoom*/);
	return BR_CONTINUE;
}

builtIn(getMouseScreenY)
{
	UNUSEDALL
	_engine->setVariable(fun->reg, SVT_INT, 0/*input.mouseY * cameraZoom*/);
	return BR_CONTINUE;
}

builtIn(getStatusText)
{
	UNUSEDALL
	_engine->makeTextVar(fun->reg, ""/*statusBarText()*/);
	return BR_CONTINUE;
}

builtIn(getMatchingFiles)
{
	UNUSEDALL
	//char *newText = getTextFromAnyVar(fun->stack->thisVar);
	//if (!newText) return BR_ERROR;
	_engine->trimStack(fun->stack);
	_engine->unlinkVar(fun->reg);

	// Return value
//	fun->reg.varType = SVT_STACK;
//	fun->reg.varData.theStack = new stackHandler;
//	//if (!checkNew(fun->reg.varData.theStack)) return BR_ERROR;
//	fun->reg.varData.theStack->first = NULL;
//	fun->reg.varData.theStack->last = NULL;
//	fun->reg.varData.theStack->timesUsed = 1;
//	if (!getSavedGamesStack(fun->reg.varData.theStack, newText)) return BR_ERROR;
//	delete newText;
//	newText = NULL;
//	return BR_CONTINUE;
	//TODO: stack change here, warning if arrive here
	return BR_ERROR;
}

builtIn(saveGame)
{
	UNUSEDALL

//	if (frozenStuff) {
//		debug("Can't save game state while the engine is frozen");
//	}
//
//	loadNow = getTextFromAnyVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);

//	char *aaaaa = encodeFilename(loadNow);
//	delete[] loadNow;
//	if (failSecurityCheck(aaaaa)) return BR_ERROR;		// Won't fail if encoded, how cool is that? OK, not very.
//
//	loadNow = joinStrings(":", aaaaa);
//	delete[] aaaaa;

	_engine->setVariable(fun->reg, SVT_INT, 0);
//	saverFunc = fun;
	return BR_KEEP_AND_PAUSE;
}

builtIn(fileExists)
{
	UNUSEDALL
//	loadNow = getTextFromAnyVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
//	char *aaaaa = encodeFilename(loadNow);
//	delete loadNow;
//	if (failSecurityCheck(aaaaa)) return BR_ERROR;
//	FILE *fp = fopen(aaaaa, "rb");
//	if (!fp) {
//		char currentDir[1000];
//		if (!getcwd(currentDir, 998)) {
//			debugOut( "Can't get current directory.\n");
//		}
//
//		if (chdir(gamePath)) {
//			debugOut("Error: Failed changing to directory %s\n", gamePath);
//		}
//		fp = fopen(aaaaa, "rb");
//		if (chdir(currentDir)) {
//			debugOut("Error: Failed changing to directory %s\n", currentDir);
//		}
//	}
//	// Return value
	_engine->setVariable(fun->reg, SVT_INT, 0/*(fp != NULL)*/);
//	if (fp) fclose(fp);
//	delete[] aaaaa;
//	loadNow = NULL;
	return BR_CONTINUE;
}

builtIn(loadGame)
{
	UNUSEDALL
	//char *aaaaa = getTextFromAnyVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
	//loadNow = encodeFilename(aaaaa);
	//delete aaaaa;

//	if (frozenStuff) {
//		debug("Can't load a saved game while the engine is frozen");
//	}

//	if (failSecurityCheck(loadNow)) return BR_ERROR;
//	FILE *fp = fopen(loadNow, "rb");
//	if (fp) {
//		fclose(fp);
//		return BR_KEEP_AND_PAUSE;
//	}
//	delete loadNow;
//	loadNow = NULL;
	return BR_CONTINUE;
}

//--------------------------------------
#pragma mark -
#pragma mark Background image - Painting

builtIn(blankScreen)
{
	UNUSEDALL
	//blankScreen(0, 0, sceneWidth, sceneHeight);
	return BR_CONTINUE;
}

builtIn(blankArea)
{
	UNUSEDALL
	int x1, y1, x2, y2;
	if (!_engine->getValueType(y2, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x2, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(y1, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x1, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	//blankScreen(x1, y1, x2, y2);
	return BR_CONTINUE;
}

builtIn(darkBackground)
{
	UNUSEDALL
	//darkScreen();
	return BR_CONTINUE;
}

builtIn(addOverlay)
{
	UNUSEDALL
	debug("addOverlay is called");

	int fileNumber, xPos, yPos;
	if (!_engine->getValueType(yPos, SVT_INT, fun->stack->thisVar))
		return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(xPos, SVT_INT, fun->stack->thisVar))
		return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar))
		return BR_ERROR;
	_engine->trimStack(fun->stack);
	//loadBackDrop(fileNumber, xPos, yPos);
	return BR_CONTINUE;
}

builtIn(mixOverlay)
{
	UNUSEDALL
	int fileNumber, xPos, yPos;
	if (!_engine->getValueType(yPos, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(xPos, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	//mixBackDrop(fileNumber, xPos, yPos);
	return BR_CONTINUE;
}

builtIn(pasteImage)
{
	UNUSEDALL
	int x, y;
	if (!_engine->getValueType(y, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	personaAnimation *pp = getAnimationFromVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
//	if (pp == NULL) return BR_CONTINUE;

//	pasteCursor(x, y, pp);
	return BR_CONTINUE;
}

#pragma mark -
#pragma mark Background Image - Scrolling

builtIn(setSceneDimensions)
{
	UNUSEDALL
	int x, y;
	if (!_engine->getValueType(y, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	if (resizeBackdrop(x, y)) {
//		blankScreen(0, 0, x, y);
//		return BR_CONTINUE;
//	}
//	debug("Out of memory creating new backdrop.");
//	return BR_ERROR;
	return BR_CONTINUE;
}

builtIn(aimCamera)
{
	UNUSEDALL
	int cameraY, cameraX; // TODO: temp var to remove
	if (!_engine->getValueType(cameraY, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(cameraX, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	cameraX -= (float)(winWidth >> 1)/ cameraZoom;
//	cameraY -= (float)(winHeight >> 1)/ cameraZoom;
//
//	if (cameraX < 0) cameraX = 0;
//	else if (cameraX > sceneWidth - (float)winWidth/ cameraZoom) cameraX = sceneWidth - (float)winWidth/ cameraZoom;
//	if (cameraY < 0) cameraY = 0;
//	else if (cameraY > sceneHeight - (float)winHeight/ cameraZoom) cameraY = sceneHeight - (float)winHeight/ cameraZoom;
	return BR_CONTINUE;
}


builtIn(zoomCamera)
{
	UNUSEDALL
	int z;
	if (!_engine->getValueType(z, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	input.mouseX = input.mouseX * cameraZoom;
//	input.mouseY = input.mouseY * cameraZoom;
//
//
//	cameraZoom = (float)z * 0.01;
//	if ((float) winWidth / cameraZoom > sceneWidth) cameraZoom = (float)winWidth / sceneWidth;
//	if ((float) winHeight / cameraZoom > sceneHeight) cameraZoom = (float)winHeight / sceneHeight;
//	setPixelCoords(false);
//
//	input.mouseX = input.mouseX / cameraZoom;
//	input.mouseY = input.mouseY / cameraZoom;

	return BR_CONTINUE;
}

#pragma mark -
#pragma mark Variables

builtIn(pickOne)
{
	UNUSEDALL
	if (!numParams) {
		debug("Built-in function should have at least 1 parameter");
		return BR_ERROR;
	}
	int i = rand() % numParams;

	// Return value
	while (numParams --) {
		if (i == numParams)
			_engine->copyVariable(fun->stack->thisVar, fun->reg);
		_engine->trimStack(fun->stack);
	}
	return BR_CONTINUE;
}

builtIn(substring)
{
	UNUSEDALL
	char *wholeString;
	char *newString;
	int start, length;

	//debugOut("BUILTIN: substring\n");

	if (!_engine->getValueType(length, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(start, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	wholeString = _engine->getTextFromAnyVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);

//	if (u8_strlen(wholeString) < start+length) {
//		length = u8_strlen(wholeString) - start;
//		if (u8_strlen(wholeString) < start) {
//			start = 0;
//		}
//	}
//	if (length<0) {
//		length=0;
//	}
//
//	int startoffset = u8_offset(wholeString, start);
//	int endoffset = u8_offset(wholeString, start+length);
//
//	newString = new char[endoffset - startoffset + 1];
//	if (!checkNew(newString)) {
//		return BR_ERROR;
//	}
//
//	memcpy(newString, wholeString + startoffset, endoffset - startoffset);
//	newString[endoffset - startoffset] = 0;

	_engine->makeTextVar(fun->reg, ""/*newString*/);
	delete newString;
	return BR_CONTINUE;
}

builtIn(stringLength)
{
	UNUSEDALL
	char *newText = _engine->getTextFromAnyVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 0/*stringLength(newText)*/);
	delete[] newText;
	return BR_CONTINUE;
}

builtIn(newStack)
{
	UNUSEDALL
	_engine->unlinkVar(fun->reg);

	// Return value
	fun->reg.varType = SVT_STACK;
	fun->reg.varData.theStack = new StackHandler;
	//if (!checkNew(fun->reg.varData.theStack)) return BR_ERROR;
	fun->reg.varData.theStack->first = NULL;
	fun->reg.varData.theStack->last = NULL;
	fun->reg.varData.theStack->timesUsed = 1;
	while (--numParams) {
		if (!_engine->addVarToStack(fun->stack->thisVar, fun->reg.varData.theStack->first)) return BR_ERROR;
		if (fun->reg.varData.theStack->last == NULL) {
			fun->reg.varData.theStack->last = fun->reg.varData.theStack->first;
		}
		_engine->trimStack(fun->stack);
	}
	return BR_CONTINUE;
}

// wait is exactly the same function, but limited to 2 parameters
builtIn(wait)
{
	newStack(numParams, fun);
}

builtIn(stackSize)
{
	UNUSEDALL
	switch (fun->stack->thisVar.varType) {
		case SVT_STACK:
			// Return value
			_engine->setVariable(fun->reg, SVT_INT, _engine->stackSize(fun->stack->thisVar.varData.theStack));
			_engine->trimStack(fun->stack);
			return BR_CONTINUE;

		case SVT_FASTARRAY:
			// Return value
			_engine->setVariable(fun->reg, SVT_INT, fun->stack->thisVar.varData.fastArray->size);
			_engine->trimStack(fun->stack);
			return BR_CONTINUE;

		default:
			break;
	}
	debug("Parameter isn't a stack or a fast array.");
	return BR_ERROR;
}

builtIn(copyStack)
{
	UNUSEDALL
	if (fun->stack->thisVar.varType != SVT_STACK) {
		debug("Parameter isn't a stack.");
		return BR_ERROR;
	}
	// Return value
	if (!_engine->copyStack(fun->stack->thisVar, fun->reg)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(pushToStack)
{
	UNUSEDALL
	if (fun->stack->next->thisVar.varType != SVT_STACK) {
		debug("Parameter isn't a stack");
		return BR_ERROR;
	}

	if (!_engine->addVarToStack(fun->stack->thisVar, fun->stack->next->thisVar.varData.theStack->first))
		return BR_ERROR;

	if (fun->stack->next->thisVar.varData.theStack->first->next == NULL)
		fun->stack->next->thisVar.varData.theStack->last = fun->stack->next->thisVar.varData.theStack->first;

	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(enqueue)
{
	UNUSEDALL
	if (fun->stack->next->thisVar.varType != SVT_STACK) {
		debug("Parameter isn't a stack");
		return BR_ERROR;
	}

	if (fun->stack->next->thisVar.varData.theStack->first == NULL)
	{
		if (!_engine->addVarToStack(fun->stack->thisVar, fun->stack->next->thisVar.varData.theStack->first))
			return BR_ERROR;

		fun->stack->next->thisVar.varData.theStack->last = fun->stack->next->thisVar.varData.theStack->first;
	}
	else
	{
		if (!_engine->addVarToStack(fun->stack->thisVar,
							 fun->stack->next->thisVar.varData.theStack->last->next))
			return BR_ERROR;
		fun->stack->next->thisVar.varData.theStack->last = fun->stack->next->thisVar.varData.theStack->last->next;
	}

	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(deleteFromStack)
{
	UNUSEDALL
	if (fun->stack->next->thisVar.varType != SVT_STACK) {
		debug("Parameter isn't a stack.");
		return BR_ERROR;
	}

	// Return value
	_engine->setVariable(fun->reg, SVT_INT,
						_engine->deleteVarFromStack(fun->stack->thisVar,
													fun->stack->next->thisVar.varData.theStack->first, false));

	// Horrible hacking because 'last' value might now be wrong!
	fun->stack->next->thisVar.varData.theStack->last = _engine->stackFindLast(fun->stack->next->thisVar.varData.theStack->first);

	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(deleteAllFromStack)
{
	UNUSEDALL
	if (fun->stack->next->thisVar.varType != SVT_STACK) {
		debug("Parameter isn't a stack.");
		return BR_ERROR;
	}

	// Return value
	_engine->setVariable(fun->reg, SVT_INT,
						_engine->deleteVarFromStack(fun->stack->thisVar,
									 	 	 	 	 fun->stack->next->thisVar.varData.theStack->first, true));

	// Horrible hacking because 'last' value might now be wrong!
	fun->stack->next->thisVar.varData.theStack->last = _engine->stackFindLast(fun->stack->next->thisVar.varData.theStack->first);

	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(popFromStack)
{
	UNUSEDALL
	if (fun->stack->thisVar.varType != SVT_STACK) {
		debug("Parameter isn't a stack.");
		return BR_ERROR;
	}
	if (fun->stack->thisVar.varData.theStack->first == NULL) {
		debug("The stack's empty.");
		return BR_ERROR;
	}

	// Return value
	_engine->copyVariable(fun->stack->thisVar.varData.theStack->first->thisVar, fun->reg);
	_engine->trimStack(fun->stack->thisVar.varData.theStack->first);
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(peekStart)
{
	UNUSEDALL
	if (fun->stack->thisVar.varType != SVT_STACK) {
		debug("Parameter isn't a stack.");
		return BR_ERROR;
	}
	if (fun->stack->thisVar.varData.theStack->first == NULL) {
		debug("The stack's empty.");
		return BR_ERROR;
	}

	// Return value
	_engine->copyVariable(fun->stack->thisVar.varData.theStack->first->thisVar, fun->reg);
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(peekEnd)
{
	UNUSEDALL
	if (fun->stack->thisVar.varType != SVT_STACK) {
		debug("Parameter isn't a stack.");
		return BR_ERROR;
	}
	if (fun->stack->thisVar.varData.theStack->first == NULL) {
		debug("The stack's empty.");
		return BR_ERROR;
	}

	// Return value
	_engine->copyVariable(fun->stack->thisVar.varData.theStack->last->thisVar, fun->reg);
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(random)
{
	UNUSEDALL
	int num;

	if (!_engine->getValueType(num, SVT_INT, fun->stack->thisVar))
		return BR_ERROR;

	_engine->trimStack(fun->stack);
	if (num <= 0) num = 1;
	_engine->setVariable(fun->reg, SVT_INT, rand() % num);
	return BR_CONTINUE;
}

bool SludgeBuiltIn::getRGBParams(int &red, int &green, int &blue, LoadedFunction *fun)
{
	if (!_engine->getValueType(blue, SVT_INT, fun->stack->thisVar)) return false;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(green, SVT_INT, fun->stack->thisVar)) return false;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(red, SVT_INT, fun->stack->thisVar)) return false;
	_engine->trimStack(fun->stack);
	return true;
}

builtIn(setStatusColour)
{
	UNUSEDALL
	int red, green, blue;

	if (!getRGBParams(red, green, blue, fun))
		return BR_ERROR;

//	statusBarColour((byte) red, (byte) green, (byte) blue);
	return BR_CONTINUE;
}

builtIn(setLitStatusColour)
{
	UNUSEDALL
	int red, green, blue;

	if (!getRGBParams(red, green, blue, fun))
		return BR_ERROR;

//	statusBarLitColour((byte) red, (byte) green, (byte) blue);
	return BR_CONTINUE;
}

builtIn(setPasteColour)
{
	UNUSEDALL
	int red, green, blue;

	if (!getRGBParams(red, green, blue, fun))
		return BR_ERROR;

//	setFontColour(pastePalette, (byte) red, (byte) green, (byte) blue);
	return BR_CONTINUE;
}

builtIn(setBlankColour)
{
	UNUSEDALL
	int red, green, blue;

	if (!getRGBParams(red, green, blue, fun))
		return BR_ERROR;

//	currentBlankColour = makeColour(red & 255, green & 255, blue & 255);
	_engine->setVariable(fun->reg, SVT_INT, 1);
	return BR_CONTINUE;
}

builtIn(setBurnColour)
{
	UNUSEDALL
	int red, green, blue;

	if (!getRGBParams(red, green, blue, fun))
		return BR_ERROR;

//	currentBurnR = red;
//	currentBurnG = green;
//	currentBurnB = blue;
	_engine->setVariable(fun->reg, SVT_INT, 1);
	return BR_CONTINUE;
}


builtIn(setFont)
{
	UNUSEDALL
	int fileNumber, newHeight;
	if (!_engine->getValueType(newHeight, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	//				newDebug("  Height:", newHeight);
	_engine->trimStack(fun->stack);
	char *newText = _engine->getTextFromAnyVar(fun->stack->thisVar);
	if (!newText) return BR_ERROR;
	//				newDebug("  Character supported:", newText);
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	//				newDebug("  File:", fileNumber);
	_engine->trimStack(fun->stack);
//	if (!loadFont(fileNumber, newText, newHeight)) return BR_ERROR;
	//				newDebug("  Done!");
	delete newText;

	return BR_CONTINUE;
}

builtIn(inFont)
{
	UNUSEDALL
	char *newText = _engine->getTextFromAnyVar(fun->stack->thisVar);
	if (!newText) return BR_ERROR;
	_engine->trimStack(fun->stack);

	// Return value
	_engine->setVariable(fun->reg, SVT_INT, 0 /*isInFont(newText)*/);
	return BR_CONTINUE;
}

builtIn(pasteString)
{
	UNUSEDALL
	char *newText = _engine->getTextFromAnyVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
	int y, x;
	if (!_engine->getValueType(y, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	if (x == IN_THE_CENTRE) x = (winWidth - stringWidth(newText)) >> 1;
//	fixFont(pastePalette);
//	pasteStringToBackdrop(newText, x, y, pastePalette);
	delete[] newText;
	return BR_CONTINUE;
}

builtIn(anim)
{
	UNUSEDALL
	if (numParams < 2) {
		debug("Built-in function anim() must have at least 2 parameters.");
		return BR_ERROR;
	}

	// First store the frame numbers and take 'em off the stack
//	personaAnimation *ba = createPersonaAnim (numParams - 1, fun->stack);

	// Only remaining paramter is the file number
	int fileNumber;
	if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	// Load the required sprite bank
//	loadedSpriteBank *sprBanky = loadBankForAnim (fileNumber);
//	if (!sprBanky) return BR_ERROR;	// File not found, debug done already
//	setBankFile(ba, sprBanky);
//
//	// Return value
//	newAnimationVariable(fun->reg, ba);
//
//	return BR_CONTINUE;
}

builtIn(costume)
{
	UNUSEDALL
//	persona *newPersona = new persona;
//	if (!checkNew(newPersona)) return BR_ERROR;
//	newPersona->numDirections = numParams / 3;
//	if (numParams == 0 || newPersona->numDirections * 3 != numParams) {
//		debug("Illegal number of parameters(should be greater than 0 and divisible by 3)");
//		return BR_ERROR;
//	}
//	int iii;
//	newPersona->animation = new personaAnimation *[numParams];
//	if (!checkNew (newPersona->animation)) return BR_ERROR;
//	for(iii = numParams - 1; iii >= 0; iii --) {
//		newPersona->animation[iii] = getAnimationFromVar(fun->stack->thisVar);
//		_engine->trimStack(fun->stack);
//	}
//
//	// Return value
//	newCostumeVariable(fun->reg, newPersona);
	return BR_CONTINUE;
}

builtIn(launch)
{
	UNUSEDALL
//	char *newTextA = _engine->getTextFromAnyVar(fun->stack->thisVar);
//	if (!newTextA) return BR_ERROR;
//
//	char *newText = encodeFilename(newTextA);

	_engine->trimStack(fun->stack);
//	if (newTextA[0] == 'h' &&
//		newTextA[1] == 't' &&
//		newTextA[2] == 't' &&
//		newTextA[3] == 'p' &&
//		(newTextA[4] == ':' || (newTextA[4] == 's' && newTextA[5] == ':'))) {
//
//		// IT'S A WEBSITE!
//		launchMe = copyString(newTextA);
//	} else {
//		char *gameDir;
//#ifdef _WIN32
//		gameDir = joinStrings(gamePath, "\\");
//#else
//		gameDir = joinStrings(gamePath, "/");
//#endif
//		launchMe = joinStrings(gameDir, newText);
//		delete newText;
//		if (!launchMe) return BR_ERROR;
//	}
//	delete newTextA;
//	setGraphicsWindow(false);
//	_engine->setVariable(fun->reg, SVT_INT, 1);
//	launchResult = &fun->reg;

	return BR_KEEP_AND_PAUSE;
}

builtIn(pause)
{
	debug("pause is called");
	UNUSEDALL
	int theTime;
	if (!_engine->getValueType(theTime, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (theTime > 0) {
		fun->timeLeft = theTime - 1;
		fun->isSpeech = false;
		return BR_KEEP_AND_PAUSE;
	}
	return BR_CONTINUE;
}

builtIn(completeTimers)
{
	UNUSEDALL
	_engine->completeTimers();
	return BR_CONTINUE;
}

builtIn(callEvent)
{
	UNUSEDALL
	int obj1, obj2;
	if (!_engine->getValueType(obj2, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj1, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

	int fNum = 0/*getCombinationFunction(obj1, obj2)*/;

	// Return value
	if (fNum) {
		_engine->setVariable(fun->reg, SVT_FUNC, fNum);
		return BR_CALLAFUNC;
	}
	_engine->setVariable(fun->reg, SVT_INT, 0);
	return BR_CONTINUE;
}

//SDL_Event quit_event;
bool reallyWantToQuit = false;

builtIn(quitGame)
{
	debug("quitGame is called");
	UNUSEDALL
	//reallyWantToQuit = true;
	//quit_event.type=SDL_QUIT;
	//SDL_PushEvent(&quit_event);
	return BR_CONTINUE;
}

#pragma mark -
#pragma mark Movie functions

// The old movie functions are deprecated and does nothing.
builtIn(_rem_movieStart)
{
	UNUSEDALL
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;
}

builtIn(_rem_movieAbort)
{
	UNUSEDALL
	_engine->setVariable(fun->reg, SVT_INT, 0);
	return BR_CONTINUE;
}

builtIn(_rem_moviePlaying)
{
	UNUSEDALL
	_engine->setVariable(fun->reg, SVT_INT, 0);
	return BR_CONTINUE;
}

builtIn(playMovie)
{
	UNUSEDALL
	int fileNumber, r;
//	if (movieIsPlaying) return BR_PAUSE;
	if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	r = playMovie(fileNumber);
	_engine->setVariable(fun->reg, SVT_INT, r);
	if (r && (!fun->next)) {
		_engine->restartFunction(fun);
		return BR_ALREADY_GONE;
	}
	return BR_CONTINUE;
}

builtIn(stopMovie)
{
	UNUSEDALL
	int r;
//	r = stopMovie();
	_engine->setVariable(fun->reg, SVT_INT, 0);
	return BR_CONTINUE;
}

builtIn(pauseMovie)
{
	UNUSEDALL
	int r;
//	r = pauseMovie();
	_engine->setVariable(fun->reg, SVT_INT, 0);
	return BR_CONTINUE;
}


#pragma mark -
#pragma mark Audio functions

builtIn(startMusic)
{
	UNUSEDALL
	int fromTrack, musChan, fileNumber;
	if (!_engine->getValueType(fromTrack, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(musChan, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	if (!playMOd(fileNumber, musChan, fromTrack)) return BR_CONTINUE; //BR_ERROR;
	return BR_CONTINUE;
}

builtIn(stopMusic)
{
	UNUSEDALL
	int v;
	if (!_engine->getValueType(v, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	stopMOd(v);
	return BR_CONTINUE;
}

builtIn(setMusicVolume)
{
	UNUSEDALL
	int musChan, v;
	if (!_engine->getValueType(v, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(musChan, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setMusicVolume(musChan, v);
	return BR_CONTINUE;
}

builtIn(setDefaultMusicVolume)
{
	UNUSEDALL
	int v;
	if (!_engine->getValueType(v, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setDefaultMusicVolume(v);
	return BR_CONTINUE;
}

builtIn(playSound)
{
	debug("playSound is called");
	UNUSEDALL
	int fileNumber;
	if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	//if (!startSound(fileNumber, false)) return BR_CONTINUE;	// Was BR_ERROR
	return BR_CONTINUE;
}

builtIn(loopSound)
{
	UNUSEDALL
	int fileNumber;

	if (numParams < 1) {
		debug("Built-in function loopSound() must have at least 1 parameter.");
		return BR_ERROR;
	} else if (numParams < 2) {

		if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
		_engine->trimStack(fun->stack);
//		if (!_engine->startSound(fileNumber, true)) return BR_CONTINUE;	// Was BR_ERROR
		return BR_CONTINUE;
	} else {
		// We have more than one sound to play!
		int doLoop = 2;
//		soundList *s = NULL;
//		soundList *old = NULL;

		// Should we loop?
		if (fun->stack->thisVar.varType != SVT_FILE) {
			_engine->getValueType(doLoop, SVT_INT, fun->stack->thisVar);
			_engine->trimStack(fun->stack);
			--numParams;
		}
		while (numParams) {
			if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) {
				debug("Illegal parameter given built-in function loopSound().");
				return BR_ERROR;
			}
//			s = new soundList;
//			if (!checkNew (s)) return BR_ERROR;
//
//			s-> next = old;
//			s-> prev = NULL;
//			s-> sound = fileNumber;
//			old = s;

			_engine->trimStack(fun->stack);
			--numParams;
		}
//		while (s->next) s = s-> next;
//		if (doLoop > 1) {
//			s->next = old;
//			old->prev = s;
//		} else if (doLoop) {
//			s->next = s;
//		}
//		old->vol = -1;
//		playSoundList(old);
		return BR_CONTINUE;
	}
}

builtIn(stopSound)
{
	UNUSEDALL
	int v;
	if (!_engine->getValueType(v, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	huntKillSound(v);
	return BR_CONTINUE;
}

builtIn(setDefaultSoundVolume)
{
	UNUSEDALL
	int v;
	if (!_engine->getValueType(v, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setDefaultSoundVolume(v);
	return BR_CONTINUE;
}

builtIn(setSoundVolume)
{
	UNUSEDALL
	int musChan, v;
	if (!_engine->getValueType(v, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(musChan, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setSoundVolume(musChan, v);
	return BR_CONTINUE;
}


builtIn(setSoundLoopPoints)
{
	UNUSEDALL
	int musChan, theEnd, theStart;
	if (!_engine->getValueType(theEnd, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(theStart, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(musChan, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setSoundLoop(musChan, theStart, theEnd);
	return BR_CONTINUE;
}

#pragma mark -
#pragma mark Extra room bits

builtIn(setFloor)
{
	UNUSEDALL
	if (fun->stack->thisVar.varType == SVT_FILE) {
		int v;
		_engine->getValueType(v, SVT_FILE, fun->stack->thisVar);
		_engine->trimStack(fun->stack);
//		if (!setFloor(v)) return BR_ERROR;
	} else {
		_engine->trimStack(fun->stack);
//		setFloorNull ();
	}
	return BR_CONTINUE;
}

builtIn(showFloor)
{
	UNUSEDALL
//	drawFloor();
	return BR_CONTINUE;
}

builtIn(setZBuffer)
{
	UNUSEDALL
	if (fun->stack->thisVar.varType == SVT_FILE) {
		int v;
		_engine->getValueType(v, SVT_FILE, fun->stack->thisVar);
		_engine->trimStack(fun->stack);
//		if (!setZBuffer(v)) return BR_ERROR;
	} else {
		_engine->trimStack(fun->stack);
//		killZBuffer();
	}
	return BR_CONTINUE;
}

builtIn(setLightMap)
{
	int lightMapMode; //TODO: temp variables to be removed
	UNUSEDALL
	switch (numParams) {
		case 2:
			if (!_engine->getValueType(lightMapMode, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
			_engine->trimStack(fun->stack);
//			lightMapMode %= LIGHTMAPMODE_NUM;
			// No break;

		case 1:
			if (fun->stack->thisVar.varType == SVT_FILE) {
				int v;
				_engine->getValueType(v, SVT_FILE, fun->stack->thisVar);
				_engine->trimStack(fun->stack);
//				if (!loadLightMap(v)) return BR_ERROR;
				_engine->setVariable(fun->reg, SVT_INT, 1);
			} else {
				_engine->trimStack(fun->stack);
//				killLightMap();
				_engine->setVariable(fun->reg, SVT_INT, 0);
			}
			break;

		default:
			debug("Function should have either 2 or 3 parameters");
			return BR_ERROR;
	}
	return BR_CONTINUE;
}


#pragma mark -
#pragma mark Objects

builtIn(setSpeechMode)
{
	UNUSEDALL
	int speechMode; //TODO: temp variables to be removed
	if (!_engine->getValueType(speechMode, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	if (speechMode < 0 || speechMode > 2) {
//		debug("Valid parameters are be SPEECHANDTEXT, SPEECHONLY or TEXTONLY");
//		return BR_ERROR;
//	}
	return BR_CONTINUE;
}

builtIn(somethingSpeaking)
{
	UNUSEDALL
	int i = 0/*isThereAnySpeechGoingOn()*/;
	if (i == -1) {
		_engine->setVariable(fun->reg, SVT_INT, 0);
	} else {
		_engine->setVariable(fun->reg, SVT_OBJTYPE, i);
	}
	return BR_CONTINUE;
}

builtIn(skipSpeech)
{
	UNUSEDALL
	_engine->killSpeechTimers();
	return BR_CONTINUE;
}

builtIn(getOverObject)
{
	UNUSEDALL
	if (0/*overRegion*/)
		// Return value
		_engine->setVariable(fun->reg, SVT_OBJTYPE, 0/*overRegion->thisType->objectNum*/);
	else
		// Return value
		_engine->setVariable(fun->reg, SVT_INT, 0);
	return BR_CONTINUE;
}

builtIn(rename)
{
	UNUSEDALL
	char *newText = _engine->getTextFromAnyVar(fun->stack->thisVar);
	int objT;
	if (!newText) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(objT, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	objectType *o = findObjectType(objT);
//	delete o->screenName;
//	o->screenName = newText;
	return BR_CONTINUE;
}

builtIn(getObjectX)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	onScreenPerson *pers = findPerson(objectNumber);
	if (0/*pers*/) {
		_engine->setVariable(fun->reg, SVT_INT, 0/*pers->x*/);
	} else {
//		screenRegion *la = getRegionForObject (objectNumber);
		if (0/*la*/) {
			_engine->setVariable(fun->reg, SVT_INT, 0/*la->sX*/);
		} else {
			_engine->setVariable(fun->reg, SVT_INT, 0);
		}
	}
	return BR_CONTINUE;
}

builtIn(getObjectY)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	onScreenPerson *pers = findPerson(objectNumber);
	if (0/*pers*/) {
		_engine->setVariable(fun->reg, SVT_INT, 0/*pers->y*/);
	} else {
//		screenRegion *la = getRegionForObject (objectNumber);
		if (0/*la*/) {
			_engine->setVariable(fun->reg, SVT_INT, 0/*la->sY*/);
		} else {
			_engine->setVariable(fun->reg, SVT_INT, 0);
		}
	}
	return BR_CONTINUE;
}


builtIn(addScreenRegion)
{
	UNUSEDALL
	int sX, sY, x1, y1, x2, y2, di, objectNumber;
	if (!_engine->getValueType(di, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(sY, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(sX, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(y2, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x2, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(y1, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x1, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	if (addScreenRegion(x1, y1, x2, y2, sX, sY, di, objectNumber)) return BR_CONTINUE;
	return BR_ERROR;

}

builtIn(removeScreenRegion)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	removeScreenRegion(objectNumber);
	return BR_CONTINUE;
}

builtIn(showBoxes)
{
	UNUSEDALL
//	showBoxes();
	return BR_CONTINUE;
}

builtIn(removeAllScreenRegions)
{
	UNUSEDALL
//	killAllRegions();
	return BR_CONTINUE;
}

builtIn(addCharacter)
{
	UNUSEDALL
	persona *p;
	int x, y, objectNumber;

//	p = getCostumeFromVar(fun->stack->thisVar);
	if (p == NULL) return BR_ERROR;

	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(y, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	if (addPerson(x, y, objectNumber, p)) return BR_CONTINUE;
	return BR_ERROR;
}

builtIn(hideCharacter)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setShown(false, objectNumber);
	return BR_CONTINUE;
}

builtIn(showCharacter)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setShown(true, objectNumber);
	return BR_CONTINUE;
}

builtIn(removeAllCharacters)
{
	UNUSEDALL
	_engine->killSpeechTimers();
//	killMostPeople();
	return BR_CONTINUE;
}

builtIn(setCharacterDrawMode)
{
	UNUSEDALL
	int obj, di;
	if (!_engine->getValueType(di, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setDrawMode(di, obj);
	return BR_CONTINUE;
}
builtIn(setCharacterTransparency)
{
	UNUSEDALL
	int obj, x;
	if (!_engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setPersonTransparency (obj, x);
	return BR_CONTINUE;
}
builtIn(setCharacterColourise)
{
	UNUSEDALL
	int obj, r, g, b, mix;
	if (!_engine->getValueType(mix, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(b, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(g, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(r, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setPersonColourise(obj, r, g, b, mix);
	return BR_CONTINUE;
}

builtIn(setScale)
{
	UNUSEDALL
	int val1, val2;
	if (!_engine->getValueType(val2, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(val1, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	setScale((short int) val1, (short int) val2);
	return BR_CONTINUE;
}

builtIn(stopCharacter)
{
	UNUSEDALL
	int obj;
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

	// Return value
	_engine->setVariable(fun->reg, SVT_INT, 0/*stopPerson(obj)*/);
	return BR_CONTINUE;
}

builtIn(pasteCharacter)
{
	UNUSEDALL
	int obj;
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	onScreenPerson *thisPerson = findPerson(obj);
	if (0/*thisPerson*/) {
//		personaAnimation *myAnim;
//		myAnim = thisPerson->myAnim;
//		if (myAnim != thisPerson->lastUsedAnim) {
//			thisPerson->lastUsedAnim = myAnim;
//			thisPerson->frameNum = 0;
//			thisPerson->frameTick = myAnim->frames[0].howMany;
//		}
//
//		int fNum = myAnim->frames[thisPerson->frameNum].frameNum;
//		fixScaleSprite(thisPerson->x, thisPerson->y, myAnim->theSprites->bank.sprites[abs(fNum)], myAnim->theSprites->bank.myPalette, thisPerson, 0, 0, fNum < 0);
		_engine->setVariable(fun->reg, SVT_INT, 1);
	} else {
		_engine->setVariable(fun->reg, SVT_INT, 0);
	}
	return BR_CONTINUE;
}

builtIn(animate)
{
	UNUSEDALL
	int obj;
//	personaAnimation *pp = getAnimationFromVar(fun->stack->thisVar);
//	if (pp == NULL) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	animatePerson(obj, pp);
	_engine->setVariable(fun->reg, SVT_INT, 0/*timeForAnim(pp)*/);
	return BR_CONTINUE;
}

builtIn(setCostume)
{
	UNUSEDALL
	int obj;
//	persona *pp = getCostumeFromVar(fun->stack->thisVar);
//	if (pp == NULL) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	animatePerson(obj, pp);
	return BR_CONTINUE;
}

builtIn(floatCharacter)
{
	UNUSEDALL
	int obj, di;
	if (!_engine->getValueType(di, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 0/*floatCharacter(di, obj)*/);
	return BR_CONTINUE;
}

builtIn(setCharacterWalkSpeed)
{
	UNUSEDALL
	int obj, di;
	if (!_engine->getValueType(di, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 0/*setCharacterWalkSpeed(di, obj)*/);
	return BR_CONTINUE;
}

builtIn(turnCharacter)
{
	UNUSEDALL
	int obj, di;
	if (!_engine->getValueType(di, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 0/*turnPersonToFace(obj, di)*/);
	return BR_CONTINUE;
}

builtIn(setCharacterExtra)
{
	UNUSEDALL
	int obj, di;
	if (!_engine->getValueType(di, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 0/*setPersonExtra (obj, di)*/);
	return BR_CONTINUE;
}

builtIn(removeCharacter)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	removeOneCharacter(objectNumber);
	return BR_CONTINUE;
}

BuiltReturn SludgeBuiltIn::moveChr(int numParams, LoadedFunction *fun, bool force, bool immediate)
{
	switch (numParams) {
		case 3:
		{
			int x, y, objectNumber;

			if (!_engine->getValueType(y, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
			_engine->trimStack(fun->stack);
			if (!_engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
			_engine->trimStack(fun->stack);
			if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
			_engine->trimStack(fun->stack);

//			if (force) {
//				if (forceWalkingPerson(x, y, objectNumber, fun, -1)) return BR_PAUSE;
//			} else if (immediate) {
//				jumpPerson(x, y, objectNumber);
//			} else {
//				if (makeWalkingPerson(x, y, objectNumber, fun, -1)) return BR_PAUSE;
//			}
			return BR_CONTINUE;
		}

		case 2:
		{
			int toObj, objectNumber;
//			screenRegion *reggie;

			if (!_engine->getValueType(toObj, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
			_engine->trimStack(fun->stack);
			if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
			_engine->trimStack(fun->stack);
//			reggie = getRegionForObject (toObj);
//			if (reggie == NULL) return BR_CONTINUE;

//			if (force)
//			{
//				if (forceWalkingPerson(reggie->sX, reggie->sY,	objectNumber, fun, reggie->di)) return BR_PAUSE;
//			}
//			else if (immediate)
//			{
//				jumpPerson(reggie->sX, reggie->sY,	objectNumber);
//			}
//			else
//			{
//				if (makeWalkingPerson(reggie->sX, reggie->sY, objectNumber, fun, reggie->di)) return BR_PAUSE;
//			}
			return BR_CONTINUE;
		}

		default:
			debug("Built-in function must have either 2 or 3 parameters.");
			return BR_ERROR;
	}
}

builtIn(moveCharacter)
{
	UNUSEDALL
	return moveChr(numParams, fun, false, false);
}

builtIn(forceCharacter)
{
	UNUSEDALL
	return moveChr(numParams, fun, true, false);
}

builtIn(jumpCharacter)
{
	UNUSEDALL
	return moveChr(numParams, fun, false, true);
}

builtIn(clearStatus)
{
	UNUSEDALL
//	clearStatusBar();
	return BR_CONTINUE;
}

builtIn(removeLastStatus)
{
	UNUSEDALL
//	killLastStatus();
	return BR_CONTINUE;
}

builtIn(addStatus)
{
	UNUSEDALL
//	addStatusBar();
	return BR_CONTINUE;
}

builtIn(statusText)
{
	UNUSEDALL
	char *newText = _engine->getTextFromAnyVar(fun->stack->thisVar);
	if (!newText) return BR_ERROR;
	 _engine->trimStack(fun->stack);
//	setStatusBar(newText);
	delete newText;
	return BR_CONTINUE;
}

builtIn(lightStatus)
{
	UNUSEDALL
	int val;
	if (! _engine->getValueType(val, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	 _engine->trimStack(fun->stack);
//	setLitStatus(val);
	return BR_CONTINUE;
}

builtIn(positionStatus)
{
	UNUSEDALL
	int x, y;
	if (! _engine->getValueType(y, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	 _engine->trimStack(fun->stack);
	if (! _engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	 _engine->trimStack(fun->stack);
//	positionStatus(x, y);
	return BR_CONTINUE;
}

builtIn(alignStatus)
{
	UNUSEDALL
	int val;
	if (! _engine->getValueType(val, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	 _engine->trimStack(fun->stack);
//	nowStatus->alignStatus = (short)val;
	return BR_CONTINUE;
}

bool SludgeBuiltIn::getFuncNumForCallback(int numParams, LoadedFunction *fun, int &functionNum)
{
	switch (numParams) {
		case 0:
			functionNum = 0;
			break;

		case 1:
			if (! _engine->getValueType(functionNum, SVT_FUNC, fun->stack->thisVar)) return false;
			 _engine->trimStack(fun->stack);
			break;

		default:
			debug("Too many parameters.");
			return false;
	}
	return true;
}

builtIn(onLeftMouse)
{
	UNUSEDALL
	int functionNum;
	if (getFuncNumForCallback(numParams, fun, functionNum))
	{
//		currentEvents->leftMouseFunction = functionNum;
		return BR_CONTINUE;
	}
	return BR_ERROR;
}

builtIn(onLeftMouseUp)
{
	UNUSEDALL
	int functionNum;
	if (getFuncNumForCallback(numParams, fun, functionNum))
	{
//		currentEvents->leftMouseUpFunction = functionNum;
		return BR_CONTINUE;
	}
	return BR_ERROR;
}

builtIn(onRightMouse)
{
	UNUSEDALL
	int functionNum;
	if (getFuncNumForCallback(numParams, fun, functionNum))
	{
//		currentEvents->rightMouseFunction = functionNum;
		return BR_CONTINUE;
	}
	return BR_ERROR;
}

builtIn(onRightMouseUp)
{
	UNUSEDALL
	int functionNum;
	if (getFuncNumForCallback(numParams, fun, functionNum))
	{
//		currentEvents->rightMouseUpFunction = functionNum;
		return BR_CONTINUE;
	}
	return BR_ERROR;
}

builtIn(onFocusChange)
{
	UNUSEDALL
	int functionNum;
	if (getFuncNumForCallback(numParams, fun, functionNum))
	{
//		currentEvents->focusFunction = functionNum;
		return BR_CONTINUE;
	}
	return BR_ERROR;
}

builtIn(onMoveMouse)
{
	UNUSEDALL
	int functionNum;
	if (getFuncNumForCallback(numParams, fun, functionNum))
	{
//		currentEvents->moveMouseFunction = functionNum;
		return BR_CONTINUE;
	}
	return BR_ERROR;
}

builtIn(onKeyboard)
{
	UNUSEDALL
	int functionNum;
	if (getFuncNumForCallback(numParams, fun, functionNum))
	{
//		currentEvents->spaceFunction = functionNum;
		return BR_CONTINUE;
	}
	return BR_ERROR;
}

builtIn(spawnSub)
{
	UNUSEDALL
	int functionNum;
	if (getFuncNumForCallback(numParams, fun, functionNum))
	{
		if (!_engine->startNewFunctionNum (functionNum, 0, NULL, _engine->_noStack)) return BR_ERROR;
		return BR_CONTINUE;
	}
	return BR_ERROR;
}

builtIn(cancelSub)
{
	UNUSEDALL
	int functionNum;
	if (getFuncNumForCallback(numParams, fun, functionNum))
	{
		bool killedMyself;
		_engine->cancelAFunction(functionNum, fun, killedMyself);
		if (killedMyself) {
			_engine->abortFunction(fun);
			return BR_ALREADY_GONE;
		}
		return BR_CONTINUE;
	}
	return BR_ERROR;
}

builtIn(stringWidth)
{
	UNUSEDALL
	char *theText = _engine->getTextFromAnyVar(fun->stack->thisVar);
	if (!theText) return BR_ERROR;
	_engine->trimStack(fun->stack);

	// Return value
	_engine->setVariable(fun->reg, SVT_INT, 0/*stringWidth(theText)*/);
	delete theText;
	return BR_CONTINUE;
}

builtIn(hardScroll)
{
	UNUSEDALL
	int v;
	if (!_engine->getValueType(v, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	hardScroll (v);
	return BR_CONTINUE;
}


builtIn(isScreenRegion)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 0/*getRegionForObject(objectNumber) != NULL*/);
	return BR_CONTINUE;
}

builtIn(setSpeechSpeed)
{
	UNUSEDALL
	int number;
	if (!_engine->getValueType(number, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	speechSpeed = number * 0.01;
	_engine->setVariable(fun->reg, SVT_INT, 1);
	return BR_CONTINUE;
}

builtIn(setFontSpacing)
{
	UNUSEDALL
	int fontSpaceI;
	if (!_engine->getValueType(fontSpaceI, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
//	fontSpace = fontSpaceI;
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 1);
	return BR_CONTINUE;
}

builtIn(transitionLevel)
{
	UNUSEDALL
	int number;
	if (!_engine->getValueType(number, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	if (number < 0)
//		brightnessLevel = 0;
//	else if (number > 255)
//		brightnessLevel = 255;
//	else
//		brightnessLevel = number;

	_engine->setVariable(fun->reg, SVT_INT, 1);
	return BR_CONTINUE;
}

builtIn(captureAllKeys)
{
	UNUSEDALL
	bool captureAllKeys; //TODO: temp var, to be removed
	captureAllKeys = _engine->getBoolean(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, captureAllKeys);
	return BR_CONTINUE;
}


builtIn(spinCharacter)
{
	UNUSEDALL
	int number, objectNumber;
	if (!_engine->getValueType(number, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	onScreenPerson *thisPerson = findPerson(objectNumber);
	if (0/*thisPerson*/) {
//		thisPerson->wantAngle = number;
//		thisPerson->spinning = true;
//		thisPerson->continueAfterWalking = fun;
		_engine->setVariable(fun->reg, SVT_INT, 1);
		return BR_PAUSE;
	} else {
		_engine->setVariable(fun->reg, SVT_INT, 0);
		return BR_CONTINUE;
	}
}

builtIn(getCharacterDirection)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	onScreenPerson *thisPerson = findPerson(objectNumber);
	if (0/*thisPerson*/) {
		_engine->setVariable(fun->reg, SVT_INT, 0/*thisPerson->direction*/);
	} else {
		_engine->setVariable(fun->reg, SVT_INT, 0);
	}
	return BR_CONTINUE;
}

builtIn(isCharacter)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	onScreenPerson *thisPerson = findPerson(objectNumber);
	_engine->setVariable(fun->reg, SVT_INT, 0/*thisPerson != NULL*/);
	return BR_CONTINUE;
}

builtIn(normalCharacter)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	onScreenPerson *thisPerson = findPerson(objectNumber);
	if (0/*thisPerson*/)
	{
//		thisPerson->myAnim = thisPerson->myPersona->animation[thisPerson->direction];
		_engine->setVariable(fun->reg, SVT_INT, 1);
	} else {
		_engine->setVariable(fun->reg, SVT_INT, 0);
	}
	return BR_CONTINUE;
}

builtIn(isMoving)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	onScreenPerson *thisPerson = findPerson(objectNumber);
	if (0/*thisPerson*/)
	{
		_engine->setVariable(fun->reg, SVT_INT, 0/*thisPerson->walking*/);
	} else {
		_engine->setVariable(fun->reg, SVT_INT, 0);
	}
	return BR_CONTINUE;
}

builtIn(fetchEvent)
{
	UNUSEDALL
	int obj1, obj2;
	if (!_engine->getValueType(obj2, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(obj1, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

	int fNum = 0/*getCombinationFunction(obj1, obj2)*/;

	// Return value
	if (fNum) {
		_engine->setVariable(fun->reg, SVT_FUNC, fNum);
	} else {
		_engine->setVariable(fun->reg, SVT_INT, 0);
	}
	return BR_CONTINUE;
}

builtIn(deleteFile)
{
	UNUSEDALL

//	char *namNormal = _engine->getTextFromAnyVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
//	char *nam = encodeFilename(namNormal);
//	delete namNormal;
//	if (failSecurityCheck(nam)) return BR_ERROR;
	_engine->setVariable(fun->reg, SVT_INT, 0/*remove(nam)*/);
//	delete nam;

	return BR_CONTINUE;
}

builtIn(renameFile)
{
	UNUSEDALL
	char *temp;

//	temp = getTextFromAnyVar(fun->stack->thisVar);
//	char *newnam = encodeFilename(temp);
	_engine->trimStack(fun->stack);
//	if (failSecurityCheck(newnam)) return BR_ERROR;
//	delete temp;
//
//	temp = getTextFromAnyVar(fun->stack->thisVar);
//	char *nam = encodeFilename(temp);
//	trimStack(fun->stack);
//	if (failSecurityCheck(nam)) return BR_ERROR;
//	delete temp;

	_engine->setVariable(fun->reg, SVT_INT, 0/*rename(nam, newnam)*/);
//	delete nam;
//	delete newnam;
	return BR_CONTINUE;
}

builtIn(cacheSound)
{
	UNUSEDALL
	int fileNumber;
	if (!_engine->getValueType(fileNumber, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	if (cacheSound(fileNumber) == -1) return BR_ERROR;
	return BR_CONTINUE;
}

builtIn(burnString)
{
	UNUSEDALL
	char *newText = _engine->getTextFromAnyVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
	int y, x;
	if (!_engine->getValueType(y, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	if (x == IN_THE_CENTRE) x = (winWidth - stringWidth(newText)) >> 1;
//	fixFont (pastePalette);
//	burnStringToBackdrop(newText, x, y, pastePalette);
	delete[] newText;
	return BR_CONTINUE;
}

builtIn(setCharacterSpinSpeed)
{
	UNUSEDALL
	int speed, who;
	if (!_engine->getValueType(speed, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(who, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	onScreenPerson *thisPerson = findPerson(who);

	if (0/*thisPerson*/) {
//		thisPerson->spinSpeed = speed;
		_engine->setVariable(fun->reg, SVT_INT, 1);
	} else {
		_engine->setVariable(fun->reg, SVT_INT, 0);
	}
	return BR_CONTINUE;
}

builtIn(setCharacterAngleOffset)
{
	UNUSEDALL
	int angle, who;
	if (!_engine->getValueType(angle, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(who, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	onScreenPerson *thisPerson = findPerson(who);

	if (0/*thisPerson*/) {
//		thisPerson->angleOffset = angle;
		_engine->setVariable(fun->reg, SVT_INT, 1);
	} else {
		_engine->setVariable(fun->reg, SVT_INT, 0);
	}
	return BR_CONTINUE;
}


builtIn(transitionMode)
{
	UNUSEDALL
	int n;
	if (!_engine->getValueType(n, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
//	fadeMode = n;
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 1);
	return BR_CONTINUE;
}


// Removed function - does nothing
builtIn(_rem_updateDisplay)
{
	UNUSEDALL
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, true);
	return BR_CONTINUE;
}

builtIn(getSoundCache)
{
	UNUSEDALL
	fun->reg.varType = SVT_STACK;
	fun->reg.varData.theStack = new StackHandler;
	//if (!checkNew(fun->reg.varData.theStack)) return BR_ERROR;
	fun->reg.varData.theStack->first = NULL;
	fun->reg.varData.theStack->last = NULL;
	fun->reg.varData.theStack->timesUsed = 1;
//	if (!getSoundCacheStack(fun->reg.varData.theStack)) return BR_ERROR;
//	return BR_CONTINUE;
	//TODO: stack change, should not arrive here now
	return BR_ERROR;
}

builtIn(saveCustomData)
{
	UNUSEDALL
	// saveCustomData (thisStack, fileName);
//	char *fileNameB = _engine->getTextFromAnyVar(fun->stack->thisVar);
//	//if (!checkNew(fileNameB)) return BR_ERROR;
//
//	char *fileName = encodeFilename(fileNameB);
//	delete fileNameB;
//
//	if (failSecurityCheck(fileName)) return BR_ERROR;
//	_engine->trimStack(fun->stack);
//
//	if (fun->stack->thisVar.varType != SVT_STACK) {
//		debug("First parameter isn't a stack");
//		return BR_ERROR;
//	}
//	if (!stackToFile(fileName, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	delete fileName;
//	return BR_CONTINUE;
	//TODO: stack change, should not arrive here now
	return BR_ERROR;
}

builtIn(loadCustomData)
{
	UNUSEDALL

//	char *newTextA = _engine->getTextFromAnyVar(fun->stack->thisVar);
//	if (!checkNew(newTextA)) return BR_ERROR;
//
//	char *newText = encodeFilename(newTextA);
//	delete newTextA;
//
//	if (failSecurityCheck(newText)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	_engine->unlinkVar(fun->reg);
	fun->reg.varType = SVT_STACK;
	fun->reg.varData.theStack = new StackHandler;
//	if (!checkNew (fun->reg.varData.theStack)) return BR_ERROR;
	fun->reg.varData.theStack->first = NULL;
	fun->reg.varData.theStack->last = NULL;
	fun->reg.varData.theStack->timesUsed = 1;
//	if (!fileToStack(newText, fun->reg.varData.theStack)) return BR_ERROR;
//	delete newText;
//	return BR_CONTINUE;
	//TODO: stack change, should not arrive here now
	return BR_ERROR;
}

builtIn(setCustomEncoding)
{
	UNUSEDALL
	int n;
	if (!_engine->getValueType(n, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
//	saveEncoding = n;
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 1);
	return BR_CONTINUE;
}

builtIn(freeSound)
{
	UNUSEDALL
	int v;
	if (!_engine->getValueType(v, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	huntKillFreeSound(v);
	return BR_CONTINUE;
}

builtIn(parallaxAdd)
{
	UNUSEDALL
	if (0/*frozenStuff*/) {
		debug("Can't set background parallax image while frozen");
		return BR_ERROR;
	} else {
		int wrapX, wrapY, v;
		if (!_engine->getValueType(wrapY, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
		_engine->trimStack(fun->stack);
		if (!_engine->getValueType(wrapX, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
		_engine->trimStack(fun->stack);
		if (!_engine->getValueType(v, SVT_FILE, fun->stack->thisVar)) return BR_ERROR;
		_engine->trimStack(fun->stack);

//		if (!loadParallax (v, wrapX, wrapY)) return BR_ERROR;
		_engine->setVariable(fun->reg, SVT_INT, 1);
	}
	return BR_CONTINUE;
}

builtIn(parallaxClear)
{
	UNUSEDALL
//	killParallax ();
	_engine->setVariable(fun->reg, SVT_INT, 1);
	return BR_CONTINUE;
}

builtIn(getPixelColour)
{
	UNUSEDALL
	int x, y;
	if (!_engine->getValueType(y, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

	_engine->unlinkVar(fun->reg);
	fun->reg.varType = SVT_STACK;
	fun->reg.varData.theStack = new StackHandler;
	//if (!checkNew (fun->reg.varData.theStack)) return BR_ERROR;
	fun->reg.varData.theStack->first = NULL;
	fun->reg.varData.theStack->last = NULL;
	fun->reg.varData.theStack->timesUsed = 1;
//	if (!getRGBIntoStack(x, y, fun->reg.varData.theStack)) return BR_ERROR;
//	return BR_CONTINUE;
	//TODO: stack change, should not arrive here now
	return BR_ERROR;
}

builtIn(makeFastArray)
{
	UNUSEDALL
	switch (fun->stack->thisVar.varType) {
		case SVT_STACK: {
			bool success = _engine->makeFastArrayFromStack(fun->reg, fun->stack->thisVar.varData.theStack);
			_engine->trimStack(fun->stack);
			return success ? BR_CONTINUE : BR_ERROR;
			break;
		}

		case SVT_INT: {
			int i = fun->stack->thisVar.varData.intValue;
			_engine->trimStack(fun->stack);
			return _engine->makeFastArraySize(fun->reg, i) ? BR_CONTINUE : BR_ERROR;
			break;
		}

		default:
			break;
	}
	debug("Parameter must be a number or a stack.");
	return BR_ERROR;
}

builtIn(getCharacterScale)
{
	UNUSEDALL
	int objectNumber;
	if (!_engine->getValueType(objectNumber, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	onScreenPerson *pers = findPerson(objectNumber);
	if (0/*pers*/) {
		_engine->setVariable(fun->reg, SVT_INT, 0/*pers->scale * 100*/);
	} else {
		_engine->setVariable(fun->reg, SVT_INT, 0);
	}
	return BR_CONTINUE;
}

builtIn(getLanguageID)
{
	UNUSEDALL
	_engine->setVariable(fun->reg, SVT_INT, _engine->_gameSettings.languageID);
	return BR_CONTINUE;
}

// Removed function
builtIn(_rem_launchWith)
{
	UNUSEDALL
	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, false);
	return BR_CONTINUE;
}

builtIn(getFramesPerSecond)
{
	UNUSEDALL
	_engine->setVariable(fun->reg, SVT_INT, 0/*lastFramesPerSecond*/);
	return BR_CONTINUE;
}

builtIn(showThumbnail)
{
	UNUSEDALL
	int x, y;
	if (!_engine->getValueType(y, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(x, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);

//	// Encode the name!Encode the name!
//	char *aaaaa = getTextFromAnyVar(fun->stack->thisVar);
//	//				deb ("Got name:", aaaaa;)
//	trimStack(fun->stack);
//	//				deb ("About to encode", aaaaa);
//	char *file = encodeFilename(aaaaa);
//	//				deb ("Made new name", file);
//	//				deb ("aaaaa is still ", aaaaa);
//	delete[] aaaaa;
//	//				deb ("Deleted", "aaaaa");
//	showThumbnail(file, x, y);
//	delete[] file;
	return BR_CONTINUE;
}

builtIn(setThumbnailSize)
{
	UNUSEDALL
	int thumbHeight, thumbWidth; //TODO: temp vars, to be removed
	if (!_engine->getValueType(thumbHeight, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(thumbWidth, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (thumbWidth < 0 || thumbHeight < 0 || thumbWidth > _engine->_winWidth || thumbHeight > _engine->_winHeight) {
		char buff[50];
		sprintf (buff, "%d x %d", thumbWidth, thumbHeight);
		debug("Invalid thumbnail size", buff);
		return BR_ERROR;
	}
	return BR_CONTINUE;
}

builtIn(hasFlag)
{
	UNUSEDALL
	int objNum, flagIndex;
	if (!_engine->getValueType(flagIndex, SVT_INT, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
	if (!_engine->getValueType(objNum, SVT_OBJTYPE, fun->stack->thisVar)) return BR_ERROR;
	_engine->trimStack(fun->stack);
//	objectType *objT = findObjectType(objNum);
//	if (!objT) return BR_ERROR;
	_engine->setVariable(fun->reg, SVT_INT, 0/*objT->flags & (1 << flagIndex)*/);
	return BR_CONTINUE;
}

builtIn(snapshotGrab)
{
	UNUSEDALL
//	return snapshot() ? BR_CONTINUE : BR_ERROR;
	return BR_CONTINUE;
}

builtIn(snapshotClear)
{
	UNUSEDALL
//	nosnapshot();
	return BR_CONTINUE;
}

builtIn(bodgeFilenames)
{
	UNUSEDALL
//	bool lastValue = allowAnyFilename;
//	allowAnyFilename = getBoolean(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 0/*lastValue*/);
	return BR_CONTINUE;
}

builtIn(quitWithFatalError)
{
	UNUSEDALL
	char *mess =_engine->getTextFromAnyVar(fun -> stack -> thisVar);
	_engine->trimStack (fun -> stack);
	debug(mess);
	return BR_ERROR;
}


// Deprecated - does nothing.
builtIn(_rem_registryGetString)
{
	UNUSEDALL
	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, 0);
	return BR_CONTINUE;
}

builtIn(quitWithdebugError)
{
	UNUSEDALL
	char *mess = _engine->getTextFromAnyVar(fun->stack->thisVar);
	_engine->trimStack(fun->stack);
	debug(mess);
	return BR_ERROR;
}

builtIn(_rem_setCharacterAA)
{
	UNUSEDALL

	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);

	return BR_CONTINUE;
}

builtIn(_rem_setMaximumAA)
{
	UNUSEDALL
	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	_engine->trimStack(fun->stack);
	return BR_CONTINUE;

}

builtIn(setBackgroundEffect)
{
	UNUSEDALL
//	bool done = blur_createSettings(numParams, fun->stack);
	_engine->setVariable(fun->reg, SVT_INT, /*done ? 1 : */0);
	return BR_CONTINUE;
}

builtIn(doBackgroundEffect)
{
	UNUSEDALL
//	bool done = blurScreen();
	_engine->setVariable(fun->reg, SVT_INT, /*done ? 1 :*/0);
	return BR_CONTINUE;
}

#pragma mark -
#pragma mark Other functions
//-------------------------------------
#include "builtin_tables.h"
SludgeBuiltIn::SludgeBuiltIn(SludgeEngine *e) : _engine(e), _builtinFuncs(Sludge::builtinFuncs), _numBuiltinFuncs(Sludge::numBuiltinFuncs) {
}

BuiltReturn SludgeBuiltIn::callBuiltIn(int whichFunc, int numParams, LoadedFunction *fun) {
	debug(kSludgeDebugBuiltin, "Built-in function called!");
	if (_engine->_numBIFNames) {
		const char *usrFucName = (fun->originalNumber < _engine->_numUserFunc) ? _engine->_allUserFunc[fun->originalNumber] : "Unknown user function";
		const char *calledFucName = (whichFunc < _engine->_numBIFNames) ? _engine->_allBIFNames[whichFunc] : "Unknown built-in function";
		debug("Currently in this sub: %s\nCalling: %s", usrFucName, calledFucName);
	}

	if (whichFunc < _numBuiltinFuncs) {
		if (_builtinFuncs[whichFunc].numParam != -1) {
			if (_builtinFuncs[whichFunc].numParam != numParams) {
				debug("Built in function must have %i parameter%s", _builtinFuncs[whichFunc].numParam, (_builtinFuncs[whichFunc].numParam) ? "" : "s");
				return BR_ERROR;
			}
		}

		if (_builtinFuncs[whichFunc].func) {
			return (this->*_builtinFuncs[whichFunc].func)(numParams, fun);
		}
	}
	debug("Unknown / unimplemented built-in function.");
	return BR_ERROR;
}

} // End of namespace Sludge
