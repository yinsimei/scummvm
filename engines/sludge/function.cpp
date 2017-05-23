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

#include "errors.h"
#include "function.h"
#include "moreio.h"
#include "sludge.h"

namespace Sludge {

const char *SLUDGE_TEXT[] = {"?????",
							"RETURN", "BRANCH", "BR_ZERO", "SET_GLOBAL",
							"SET_LOCAL", "LOAD_GLOBAL", "LOAD_LOCAL",
							"PLUS", "MINUS", "MULT", "DIVIDE",
							"AND", "OR", "EQUALS", "NOT_EQ", "MODULUS", "LOAD_VALUE",
							"LOAD_BUILT", "LOAD_FUNC", "CALLIT", "LOAD_STRING", "LOAD_FILE",
							"LOAD_OBJ_TYPE", "NOT", "LOAD_NULL", "STACK_PUSH",
							"LESS_THAN", "MORE_THAN", "NEGATIVE", "UNREGMESSAGE", "LESS_EQUAL", "MORE_EQUAL",
							"INCREMENT_LOCAL", "DECREMENT_LOCAL", "INCREMENT_GLOBAL", "DECREMENT_GLOBAL",
							"INDEXSET", "INDEXGET", "INCREMENT_INDEX", "DECREMENT_INDEX", "QUICK_PUSH"};

int SludgeEngine::startNewFunctionNum(unsigned int funcNum, unsigned int numParamsExpected, LoadedFunction *calledBy, VariableStack * &vStack, bool returnSommet) {
	LoadedFunction *newFunc = new LoadedFunction;
	//checkNew(newFunc);
	newFunc->originalNumber = funcNum;

	loadFunctionCode(newFunc);

	if (newFunc->numArgs != (int)numParamsExpected) {
		debug("Wrong number of parameters!");
		return 0;
	}
	if (newFunc->numArgs > newFunc->numLocals) {
		debug(kSludgeDebugFunction, "More arguments than local variable space!");
		return 0;
	}

	// Now, let's copy the parameters from the calling function's stack...
	while (numParamsExpected) {
		--numParamsExpected;
		if (vStack == nullptr) {
			debug(kSludgeDebugFunction, "Corrupted file!The stack's empty and there were still parameters expected");
			return 0;
		}
		copyVariable(vStack->thisVar, newFunc->localVars[numParamsExpected]);
		trimStack(vStack);
	}

	newFunc->cancelMe = false;
	newFunc->timeLeft = 0;
	newFunc->returnSomething = returnSommet;
	newFunc->calledBy = calledBy;
	newFunc->stack = nullptr;
	newFunc->freezerLevel = 0;
	newFunc->runThisLine = 0;
	newFunc->isSpeech = 0;
	initVarNew(newFunc->reg);

	restartFunction(newFunc);
	return 1;
}

bool SludgeEngine::loadFunctionCode(LoadedFunction *newFunc) {
	debug(kSludgeDebugDataInit, "Load function code");

	unsigned int numLines;

	if (!openSubSlice(newFunc->originalNumber)) return false;

	newFunc->unfreezable = fgetc(_bigDataFile);
	numLines = get2bytes(_bigDataFile);
	debug(kSludgeDebugDataInit, "numLines: %i", numLines);

	newFunc->numArgs = get2bytes(_bigDataFile);
	debug(kSludgeDebugDataInit, "numArgs: %i", newFunc->numArgs);

	newFunc->numLocals = get2bytes(_bigDataFile);
	debug(kSludgeDebugDataInit, "numLocals: %i", newFunc->numLocals);

	newFunc->compiledLines = new LineOfCode[numLines];
	//if (!checkNew(newFunc->compiledLines)) return false;

	for (unsigned int numLinesRead = 0; numLinesRead < numLines; numLinesRead ++) {
		newFunc->compiledLines[numLinesRead].theCommand = (SludgeCommand)fgetc(_bigDataFile);
		newFunc->compiledLines[numLinesRead].param = get2bytes(_bigDataFile);
		debug(kSludgeDebugDataInit, "command line %i: %i", numLinesRead, newFunc->compiledLines[numLinesRead].theCommand);
	}

	finishAccess();

	// Now we need to reserve memory for the local variables
	newFunc->localVars = new Variable[newFunc->numLocals];
	//if (!checkNew(newFunc->localVars)) return false;
	for (int a = 0; a < newFunc->numLocals; ++a) {
		initVarNew(newFunc->localVars[a]);
	}
	return true;
}

void SludgeEngine::restartFunction(LoadedFunction *fun) {
	fun->next = _allRunningFunctions;
	_allRunningFunctions = fun;
}

bool SludgeEngine::continueFunction(LoadedFunction *fun) {
	bool keepLooping = true;
	bool advanceNow;
	unsigned int param;
	SludgeCommand com;

	if (fun->cancelMe) {
		abortFunction(fun);
		return true;
	}

	while (keepLooping) {
		advanceNow = true;
		param = fun->compiledLines[fun->runThisLine].param;
		com = fun->compiledLines[fun->runThisLine].theCommand;

		if (_numBIFNames) {
			const char *usrFucName = (fun->originalNumber < _numUserFunc) ? _allUserFunc[fun->originalNumber] : "Unknown user function";
			const char *cmdCode = (com < kNumSludgeCommands) ? SLUDGE_TEXT[com] : ERROR_UNKNOWN_MCODE;
			debug(kSludgeDebugFunction, "Currently in this sub: %s\nCalling: %s", usrFucName, cmdCode);
		}

		switch (com) {
			case SLU_RETURN:
				debug(kSludgeDebugStackMachine, "SLU_RETURN");
				if (fun->calledBy) {
					LoadedFunction *returnTo = fun->calledBy;
					if (fun->returnSomething) copyVariable(fun->reg, returnTo->reg);
					finishFunction(fun);
					fun = returnTo;
					restartFunction(fun);
				} else {
					finishFunction(fun);
					advanceNow = false;		// So we don't do anything else with "fun"
					keepLooping = false;	// So we drop out of the loop
				}
				break;

			case SLU_CALLIT:
				debug(kSludgeDebugStackMachine, "SLU_CALLIT");
				switch (fun->reg.varType) {
					case SVT_FUNC:
						debug(kSludgeDebugStackMachine, "SVT_FUNC");
						pauseFunction(fun);
						if (_numBIFNames) {
							const char *usrFucName = (fun->originalNumber < _numUserFunc) ? _allUserFunc[fun->originalNumber] : "Unknown user function";
							const char *calledFucName = (fun->reg.varData.intValue < _numUserFunc) ? _allUserFunc[fun->reg.varData.intValue] : "Unknown user function";
							debug(kSludgeDebugBuiltin, "Currently in this sub: %s\nCalling: %s", usrFucName, calledFucName);
						}

						if (!startNewFunctionNum(fun->reg.varData.intValue, param, fun, fun->stack)) return false;
						fun = _allRunningFunctions;
						advanceNow = false;		// So we don't do anything else with "fun"
						break;

					case SVT_BUILT: {
						debug(kSludgeDebugStackMachine, "SVT_BUILT");
						BuiltReturn br = callBuiltIn(fun->reg.varData.intValue, param, fun);
						switch (br) {
							case BR_ERROR:
								debug(kSludgeDebugStackMachine, "Failing to call built-in function. Unknown error. This shouldn't happen.");
								return false;

							case BR_PAUSE:
								pauseFunction(fun);
								// Fall Through intended

							case BR_KEEP_AND_PAUSE:
								keepLooping = false;
								break;

							case BR_ALREADY_GONE:
								keepLooping = false;
								advanceNow = false;
								break;

							case BR_CALLAFUNC: {
								int i = fun->reg.varData.intValue;
								setVariable(fun->reg, SVT_INT, 1);
								pauseFunction(fun);
								if (_numBIFNames)  {
									const char *usrFucName = (fun->originalNumber < _numUserFunc) ? _allUserFunc[fun->originalNumber] : "Unknown user function";
									const char *calledFucName = (i < _numUserFunc) ? _allUserFunc[i] : "Unknown user function";
									debug(kSludgeDebugFunction, "Currently in this sub: %s\nCalling: %s", usrFucName, calledFucName);
								}

								if (!startNewFunctionNum(i, 0, fun, _noStack, false)) return false;
								fun = _allRunningFunctions;
								advanceNow = false;		// So we don't do anything else with "fun"
								break;
							}

							default:
								break;
						}
					break;
					}

					default:
						debug(kSludgeDebugStackMachine, ERROR_CALL_NONFUNCTION);
						return false;
				}
				break;

			// These all grab things and shove 'em into the register
			case SLU_LOAD_NULL:
				setVariable(fun->reg, SVT_NULL, 0);
				break;

			case SLU_LOAD_FILE:
				setVariable(fun->reg, SVT_FILE, param);
				break;

			case SLU_LOAD_VALUE:
				setVariable(fun->reg, SVT_INT, param);
				break;

			case SLU_LOAD_LOCAL:
				if(!copyVariable(fun->localVars[param], fun->reg)) return false;
				break;

			case SLU_AND:
				setVariable(fun->reg, SVT_INT, getBoolean(fun->reg) && getBoolean(fun->stack->thisVar));
				trimStack(fun->stack);
				break;

			case SLU_OR:
				setVariable(fun->reg, SVT_INT, getBoolean(fun->reg) || getBoolean(fun->stack->thisVar));
				trimStack(fun->stack);
				break;

			case SLU_LOAD_FUNC:
				setVariable(fun->reg, SVT_FUNC, param);
				break;

			case SLU_LOAD_BUILT:
				setVariable(fun->reg, SVT_BUILT, param);
				break;

			case SLU_LOAD_OBJTYPE:
				setVariable(fun->reg, SVT_OBJTYPE, param);
				break;

			case SLU_UNREG:
				if (_dialogValue != 1)
					debug(kSludgeDebugStackMachine, ERROR_HACKER);
				break;

				case SLU_LOAD_STRING:
					if (!loadStringToVar(fun->reg, param)) {
						return false;
					}
				break;

			case SLU_INDEXGET:
				// Fall Through intended

			case SLU_INCREMENT_INDEX:
				// Fall Through intended

			case SLU_DECREMENT_INDEX:
				switch (fun->stack->thisVar.varType) {
					case SVT_NULL:
						if (com == SLU_INDEXGET) {
							setVariable(fun->reg, SVT_NULL, 0);
							trimStack(fun->stack);
						} else {
							debug(kSludgeDebugStackMachine, ERROR_INCDEC_UNKNOWN);
							return false;
						}
						break;

					case SVT_FASTARRAY:
						// Fall Through intended

					case SVT_STACK:
						if (fun->stack->thisVar.varData.theStack->first == NULL) {
							debug(kSludgeDebugStackMachine, ERROR_INDEX_EMPTY);
							return false;
						} else {
							int ii;
							if (!getValueType(ii, SVT_INT, fun->reg)) return false;
							Variable *grab = (fun->stack->thisVar.varType == SVT_FASTARRAY) ?
								fastArrayGetByIndex(fun->stack->thisVar.varData.fastArray, ii)
									:
								stackGetByIndex(fun->stack->thisVar.varData.theStack->first, ii);

							trimStack(fun->stack);
							if (!grab) {
								setVariable(fun->reg, SVT_NULL, 0);
							} else {
								int ii;
								switch (com) {
									case SLU_INCREMENT_INDEX:
										if (!getValueType(ii, SVT_INT, *grab))
											return false;
										setVariable(fun->reg, SVT_INT, ii);
										grab->varData.intValue = ii + 1;
										break;

									case SLU_DECREMENT_INDEX:
										if (!getValueType(ii, SVT_INT, *grab))
											return false;
										setVariable(fun->reg, SVT_INT, ii);
										grab->varData.intValue = ii - 1;
										break;

									default:
										if (!copyVariable(*grab, fun->reg))
											return false;
										break;
								}
							}
						}
						break;

					default:
						debug(kSludgeDebugStackMachine, ERROR_INDEX_NONSTACK);
						return false;
				}
				break;

			case SLU_INDEXSET:
				switch (fun->stack->thisVar.varType) {
					case SVT_STACK:
						if (fun->stack->thisVar.varData.theStack->first == NULL) {
							debug(kSludgeDebugStackMachine, ERROR_INDEX_EMPTY);
							return false;
						} else {
							int ii;
							if (!getValueType(ii, SVT_INT, fun->reg)) return false;
							if (!stackSetByIndex(fun->stack->thisVar.varData.theStack->first, ii, fun->stack->next->thisVar)) {
								return false;
							}
							trimStack(fun->stack);
							trimStack(fun->stack);
						}
						break;

					case SVT_FASTARRAY: {
						int ii;
						if (!getValueType(ii, SVT_INT, fun->reg)) return false;
						Variable *v = fastArrayGetByIndex(fun->stack->thisVar.varData.fastArray, ii);
						if (v == nullptr) {
							debug(kSludgeDebugStackMachine, "Not within bounds of fast array.");
							return false;
						}
						if (!copyVariable(fun->stack->next->thisVar, *v)) return false;
						trimStack(fun->stack);
						trimStack(fun->stack);
						break;
					}


					default:
						debug(kSludgeDebugStackMachine, ERROR_INDEX_NONSTACK);
						return false;
				}
				break;

			// What can we do with the register? Well, we can copy it into a local
			// variable, a global or onto the stack...
			case SLU_INCREMENT_LOCAL: {
				int ii;
				if (!getValueType(ii, SVT_INT, fun->localVars[param])) return false;
				setVariable(fun->reg, SVT_INT, ii);
				setVariable(fun->localVars[param], SVT_INT, ii + 1);
				break;
			}

			case SLU_INCREMENT_GLOBAL: {
				int ii;
				if (!getValueType(ii, SVT_INT, _globalVars[param])) return false;
				setVariable(fun->reg, SVT_INT, ii);
				setVariable(_globalVars[param], SVT_INT, ii + 1);
				break;
			}

			case SLU_DECREMENT_LOCAL: {
				int ii;
				if (!getValueType(ii, SVT_INT, fun->localVars[param])) return false;
				setVariable(fun->reg, SVT_INT, ii);
				setVariable(fun->localVars[param], SVT_INT, ii - 1);
				break;
			}

			case SLU_DECREMENT_GLOBAL:
			{
				int ii;
				if (!getValueType(ii, SVT_INT, _globalVars[param])) return false;
				setVariable(fun->reg, SVT_INT, ii);
				setVariable(_globalVars[param], SVT_INT, ii - 1);
				break;
			}

			case SLU_SET_LOCAL:
				if (!copyVariable(fun->reg, fun->localVars[param])) return false;
				break;

			case SLU_SET_GLOBAL:
				if (!copyVariable(fun->reg, _globalVars[param])) return false;
				break;

			case SLU_LOAD_GLOBAL:
				if (!copyVariable(_globalVars[param], fun->reg)) return false;
				break;

			case SLU_STACK_PUSH:
				if (!addVarToStack(fun->reg, fun->stack)) return false;
				break;

			case SLU_QUICK_PUSH:
				if (!addVarToStackQuick(fun->reg, fun->stack)) return false;
				break;

			case SLU_NOT:
				setVariable(fun->reg, SVT_INT, !getBoolean(fun->reg));
				break;

			case SLU_BR_ZERO:
				if (!getBoolean(fun->reg)) {
					advanceNow = false;
					fun->runThisLine = param;
				}
				break;

			case SLU_BRANCH:
				advanceNow = false;
				fun->runThisLine = param;
				break;

			case SLU_NEGATIVE: {
				int i;
				if (!getValueType(i, SVT_INT, fun->reg)) return false;
				setVariable(fun->reg, SVT_INT, -i);
				break;
			}

			// All these things rely on there being somet' on the stack
			case SLU_MULT:
				// Fall Through intended
			case SLU_PLUS:
				// Fall Through intended
			case SLU_MINUS:
				// Fall Through intended
			case SLU_MODULUS:
				// Fall Through intended
			case SLU_DIVIDE:
				// Fall Through intended
			case SLU_EQUALS:
				// Fall Through intended
			case SLU_NOT_EQ:
				// Fall Through intended
			case SLU_LESSTHAN:
				// Fall Through intended
			case SLU_MORETHAN:
				// Fall Through intended
			case SLU_LESS_EQUAL:
				// Fall Through intended
			case SLU_MORE_EQUAL:
				if (fun->stack) {
					int firstValue, secondValue;
					switch (com) {
						case SLU_PLUS:
							addVariablesInSecond(fun->stack->thisVar, fun->reg);
							trimStack(fun->stack);
							break;

						case SLU_EQUALS:
							compareVariablesInSecond(fun->stack->thisVar, fun->reg);
							trimStack(fun->stack);
							break;

						case SLU_NOT_EQ:
							compareVariablesInSecond(fun->stack->thisVar, fun->reg);
							trimStack(fun->stack);
						   fun->reg.varData.intValue = !fun->reg.varData.intValue;
							break;

						default:
							if (!getValueType(firstValue, SVT_INT, fun->stack->thisVar)) return false;
							if (!getValueType(secondValue, SVT_INT, fun->reg)) return false;
							trimStack(fun->stack);
							switch (com) {
								case SLU_MULT:
									setVariable(fun->reg, SVT_INT, firstValue *secondValue);
									break;

								case SLU_MINUS:
									setVariable(fun->reg, SVT_INT, firstValue - secondValue);
									break;

								case SLU_MODULUS:
									setVariable(fun->reg, SVT_INT, firstValue % secondValue);
									break;

								case SLU_DIVIDE:
									setVariable(fun->reg, SVT_INT, firstValue / secondValue);
									break;

								case SLU_LESSTHAN:
									setVariable(fun->reg, SVT_INT, firstValue < secondValue);
									break;

								case SLU_MORETHAN:
									setVariable(fun->reg, SVT_INT, firstValue > secondValue);
									break;

								case SLU_LESS_EQUAL:
									setVariable(fun->reg, SVT_INT, firstValue <= secondValue);
									break;

								case SLU_MORE_EQUAL:
									setVariable(fun->reg, SVT_INT, firstValue >= secondValue);
									break;

								default:
								break;
						}
						break;
					}
				} else {
					debug(kSludgeDebugStackMachine, ERROR_NOSTACK);
					return false;
				}
				break;

			default:
				debug(kSludgeDebugStackMachine, ERROR_UNKNOWN_CODE);
				return false;
		}

		if (advanceNow)
			++fun->runThisLine;
	}
	return true;
}

void SludgeEngine::abortFunction(LoadedFunction *fun) {
	pauseFunction(fun);
	while (fun->stack)
		trimStack(fun->stack);
	delete fun->compiledLines;
	fun->compiledLines = nullptr;
	for (int a = 0; a < fun->numLocals; ++a)
		unlinkVar(fun->localVars[a]);
	delete fun->localVars;
	fun->localVars = nullptr;
	unlinkVar(fun->reg);
	if (fun->calledBy)
		abortFunction(fun->calledBy);
	delete fun;
	fun = nullptr;
}

void SludgeEngine::pauseFunction(LoadedFunction *fun) {
	LoadedFunction **huntAndDestroy = &_allRunningFunctions;
	while (*huntAndDestroy) {
		if (fun == *huntAndDestroy) {
			(*huntAndDestroy) = (*huntAndDestroy)->next;
			fun->next = nullptr;
		} else {
			huntAndDestroy = &(*huntAndDestroy)->next;
		}
	}
}

void SludgeEngine::finishFunction(LoadedFunction *fun) {
	pauseFunction(fun);
	if (fun->stack)
		debug(kSludgeDebugFunction, ERROR_NON_EMPTY_STACK);
	delete fun->compiledLines;
	for (int a = 0; a < fun->numLocals; ++a)
		unlinkVar(fun->localVars[a]);
	delete fun->localVars;
	unlinkVar(fun->reg);
	delete fun;
	fun = nullptr;
}

} // End of namespace Sludge
