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

#ifndef SLUDGE_BUILTIN_H
#define SLUDGE_BUILTIN_H

#include "function.h"

namespace Sludge {

class SludgeEngine;
struct BuiltInFunctionData;

enum BuiltReturn {BR_KEEP_AND_PAUSE, BR_ERROR, BR_CONTINUE, BR_PAUSE, BR_CALLAFUNC, BR_ALREADY_GONE};

class SludgeBuiltIn {
private:
	BuiltInFunctionData *_builtinFuncs;
	int _numBuiltinFuncs;

	SludgeEngine *_engine;

	// tool functions
	bool failSecurityCheck(char *fn);
	BuiltReturn sayCore(int numParams, LoadedFunction *fun, bool sayIt);
	bool getRGBParams(int &red, int &green, int &blue, LoadedFunction *fun);
	BuiltReturn moveChr(int numParams, LoadedFunction *fun, bool force, bool immediate);
	bool getFuncNumForCallback(int numParams, LoadedFunction *fun, int &functionNum);

public:
	// built-in functions
	#define builtIn(a) BuiltReturn a(int numParams, LoadedFunction *fun)
	builtIn(say);
	builtIn(think);
	builtIn(freeze);
	builtIn(unfreeze);
	builtIn(howFrozen);
	// Cursor
	builtIn(setCursor);
	builtIn(getMouseX);
	builtIn(getMouseY);
	builtIn(getMouseScreenX);
	builtIn(getMouseScreenY);
	builtIn(getStatusText);
	// Files
	builtIn(getMatchingFiles);
	builtIn(saveGame);
	builtIn(fileExists);
	builtIn(loadGame);
	builtIn(deleteFile);
	builtIn(renameFile);
	// Images
	builtIn(blankScreen);
	builtIn(blankArea);
	builtIn(darkBackground);
	builtIn(addOverlay);
	builtIn(mixOverlay);
	builtIn(pasteImage);
	// Scrolling
	builtIn(setSceneDimensions);
	builtIn(aimCamera);
	builtIn(zoomCamera);
	builtIn(pickOne);
	builtIn(hardScroll);
	// String
	builtIn(substring);
	builtIn(stringLength);
	builtIn(pasteString);
	builtIn(stringWidth);
	builtIn(burnString);
	builtIn(setFontSpacing);
	// Stack
	builtIn(newStack);
	builtIn(wait);
	builtIn(stackSize);
	builtIn(copyStack);
	builtIn(pushToStack);
	builtIn(enqueue);
	builtIn(deleteFromStack);
	builtIn(deleteAllFromStack);
	builtIn(popFromStack);
	builtIn(peekStart);
	builtIn(peekEnd);
	// Random
	builtIn(random);
	// Colour
	builtIn(setStatusColour);
	builtIn(setLitStatusColour);
	builtIn(setPasteColour);
	builtIn(setBlankColour);
	builtIn(setBurnColour);
	builtIn(getPixelColour);
	// Font
	builtIn(setFont);
	builtIn(inFont);
	// Animation
	builtIn(anim);
	// Costume
	builtIn(costume);
	// Game
	builtIn(launch);
	builtIn(pause);
	builtIn(completeTimers);
	builtIn(callEvent);
	builtIn(quitGame);
	builtIn(quitWithFatalError);
	builtIn(quitWithdebugError);
	// Movie
	builtIn(_rem_movieStart);
	builtIn(_rem_movieAbort);
	builtIn(_rem_moviePlaying);
	builtIn(playMovie);
	builtIn(stopMovie);
	builtIn(pauseMovie);
	// Sound & Music
	builtIn(startMusic);
	builtIn(stopMusic);
	builtIn(setMusicVolume);
	builtIn(setDefaultMusicVolume);
	builtIn(playSound);
	builtIn(loopSound);
	builtIn(stopSound);
	builtIn(setDefaultSoundVolume);
	builtIn(setSoundVolume);
	builtIn(setSoundLoopPoints);
	builtIn(freeSound);
	builtIn(cacheSound);
	builtIn(getSoundCache);
	// Floor & Buffer
	builtIn(setFloor);
	builtIn(showFloor);
	builtIn(setZBuffer);
	builtIn(setLightMap);
	// Speech
	builtIn(setSpeechMode);
	builtIn(somethingSpeaking);
	builtIn(skipSpeech);
	builtIn(setSpeechSpeed);
	// Object
	builtIn(getOverObject);
	builtIn(rename);
	builtIn(getObjectX);
	builtIn(getObjectY);
	// Region
	builtIn(addScreenRegion);
	builtIn(removeScreenRegion);
	builtIn(showBoxes);
	builtIn(removeAllScreenRegions);
	builtIn(isScreenRegion);
	// Character
	builtIn(addCharacter);
	builtIn(getCharacterScale);
	builtIn(hideCharacter);
	builtIn(showCharacter);
	builtIn(removeAllCharacters);
	builtIn(setCharacterDrawMode);
	builtIn(setCharacterTransparency);
	builtIn(setCharacterColourise);
	builtIn(setScale);
	builtIn(stopCharacter);
	builtIn(pasteCharacter);
	builtIn(animate);
	builtIn(setCostume);
	builtIn(floatCharacter);
	builtIn(setCharacterWalkSpeed);
	builtIn(turnCharacter);
	builtIn(setCharacterExtra);
	builtIn(removeCharacter);
	builtIn(moveCharacter);
	builtIn(forceCharacter);
	builtIn(jumpCharacter);
	builtIn(spinCharacter);
	builtIn(getCharacterDirection);
	builtIn(isCharacter);
	builtIn(normalCharacter);
	builtIn(isMoving);
	builtIn(setCharacterSpinSpeed);
	builtIn(setCharacterAngleOffset);
	// Status
	builtIn(clearStatus);
	builtIn(removeLastStatus);
	builtIn(addStatus);
	builtIn(statusText);
	builtIn(lightStatus);
	builtIn(positionStatus);
	builtIn(alignStatus);
	// Mouse
	builtIn(onLeftMouse);
	builtIn(onLeftMouseUp);
	builtIn(onRightMouse);
	builtIn(onRightMouseUp);
	builtIn(onFocusChange);
	builtIn(onMoveMouse);
	builtIn(onKeyboard);
	// Sub
	builtIn(spawnSub);
	builtIn(cancelSub);
	// Parallax
	builtIn(parallaxAdd);
	builtIn(parallaxClear);
	// Transition
	builtIn(transitionLevel);
	builtIn(transitionMode);
	// Save & Load
	builtIn(saveCustomData);
	builtIn(loadCustomData);
	builtIn(setCustomEncoding);
	// Others
	builtIn(captureAllKeys);
	builtIn(fetchEvent);
	builtIn(makeFastArray);
	builtIn(getLanguageID);
	builtIn(getFramesPerSecond);
	builtIn(showThumbnail);
	builtIn(setThumbnailSize);
	builtIn(hasFlag);
	// Snapshot
	builtIn(snapshotGrab);
	builtIn(snapshotClear);
	builtIn(bodgeFilenames);
	// Removed function
	builtIn(_rem_updateDisplay);
	builtIn(_rem_launchWith);
	builtIn(_rem_registryGetString);
	builtIn(_rem_setCharacterAA);
	builtIn(_rem_setMaximumAA);
	// Background
	builtIn(setBackgroundEffect);
	builtIn(doBackgroundEffect);

	#undef builtIn

public:
	BuiltReturn callBuiltIn(int whichFunc, int numParams, LoadedFunction *fun);

	SludgeBuiltIn(SludgeEngine *e);
	~SludgeBuiltIn(){};
};

} // End of namespace Sludge

#endif
