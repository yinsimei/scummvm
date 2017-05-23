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

#ifndef SLUDGE_H
#define SLUDGE_H

// file header
#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include <inttypes.h>
#include <stdio.h>

#include "common/random.h"
#include "engines/engine.h"
#include "gui/debugger.h"

#include "builtin.h"
#include "console.h"
#include "function.h"
#include "language.h"
#include "variable.h"

namespace Sludge {
 
class SludgeConsole;
struct SludgeGameDescription;

// debug channels
enum {
	kSludgeDebugScript = 1 << 0,
	kSludgeDebugDataInit = 1 << 1,
	kSludgeDebugBuiltin = 1 << 2,
	kSludgeDebugStackMachine = 1 << 3,
	kSludgeDebugFunction = 1 << 4
};

class SludgeEngine : public Engine {
protected:
	// Engine APIs
	virtual Common::Error run();

	// Game Data
//#ifndef _WIN32
	struct FileTime {
		uint32_t dwLowDateTime;
		uint32_t dwHighDateTime;
	};
//#endif
	int _numBIFNames;
	char **_allBIFNames;
	int _numUserFunc;
	char **_allUserFunc;
	int _numResourceNames;
	char **_allResourceNames;
	int _selectedLanguage;
	int _languageNum;
	unsigned int _winWidth, _winHeight;
	unsigned char * _gameIcon;
	int _iconW, _iconH;
	unsigned char * _gameLogo;
	int _logoW, _logoH;

	int _numGlobals;
	Variable *_globalVars;
	int _gameVersion;
	int _specialSettings;
	int _desiredfps;

	FileTime _fileTime;
	FILE * openAndVerify(const char *filename, const char *er, int &fileVersion);
	const char *resourceNameFromNum(int i);

	// Language Setting
	settingsStruct _gameSettings;
	char **_languageName;
	int *_languageTable;
	void makeLanguageTable(FILE *table);

	// Init Sludge
	bool initSludge();

	// Run Sludge
	bool runSludge();


	// File indices
	FILE *_bigDataFile;
	bool _sliceBusy;
	uint32_t _startIndex, _startOfDataIndex, _startOfTextIndex, _startOfSubIndex, _startOfObjectIndex;
	void setFileIndices(FILE *fp, unsigned int numLanguages, unsigned int skipBefore);
	unsigned int openFileFromNum(int num);
	bool openSubSlice(int num);
	bool openObjectSlice(int num);
	char *getNumberedString(int value);
	bool startAccess();
	void finishAccess();

	// Variables
	// Setting variables
	void initVarNew(Variable &thisVar);
	void setVariable(Variable &thisVar, VariableType vT, int value);
	bool copyVariable(const Variable &from, Variable &to);
	bool loadStringToVar(Variable &thisVar, int value);
	void addVariablesInSecond(Variable &var1, Variable &var2);
	void compareVariablesInSecond(const Variable &var1, Variable &var2);
	void makeTextVar(Variable &thisVar, const char *txt);
	// Misc.
	void unlinkVar(Variable &thisVar);
	char *getTextFromAnyVar(const Variable &from);
	bool getBoolean(const Variable &from);
	bool getValueType(int &toHere, VariableType vT, const Variable &v);
	// Stack
	void trimStack(VariableStack * &stack);
	bool addVarToStack(const Variable &va, VariableStack * &thisStack);
	bool addVarToStackQuick(Variable &va, VariableStack * &thisStack);
	Variable *stackGetByIndex(VariableStack *vS, unsigned int theIndex);
	bool stackSetByIndex(VariableStack *vS, unsigned int theIndex, const Variable &va);
	Variable *fastArrayGetByIndex(FastArrayHandler *vS, int theIndex);

	// Functions
	VariableStack *_noStack;
	int _dialogValue;
	LoadedFunction *_allRunningFunctions;
	int startNewFunctionNum(unsigned int funcNum, unsigned int numParamsExpected, LoadedFunction *calledBy, VariableStack * &vStack, bool returnSommet = true);
	bool loadFunctionCode(LoadedFunction *newFunc);
	void restartFunction(LoadedFunction *fun);
	bool continueFunction(LoadedFunction *fun);
	void abortFunction(LoadedFunction *fun);
	void pauseFunction(LoadedFunction *fun);
	void finishFunction(LoadedFunction *fun);

	// Built-ins
	BuiltReturn callBuiltIn(int whichFunc, int numParams, LoadedFunction *fun);

public:
	SludgeEngine(OSystem *syst, const SludgeGameDescription *gameDesc);
	virtual ~SludgeEngine();
 
	int getGameType() const;
	const char *getGameId() const;
	uint32 getFeatures() const;
	Common::Language getLanguage() const;

	const SludgeGameDescription *_gameDescription;
 
private:
	SludgeConsole *_console;
 	Common::RandomSource *_rnd;
};

} // End of namespace Sludge
 
#endif
