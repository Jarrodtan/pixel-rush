/*******************************************************************/
/*!
	File:		Sequence.h
	Project:	Pixel Rush
	Author(s):  Mira Tristan Dumlao
	Brief: Pressure Plates function to use for GameStateLevel.cpp file to activate the door based off pressure plates has pressed
	by either/both players in the game.

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "AEEngine.h"
// check for special sequence using enums names
enum SpecialSequence { PRESSURE_PLATE_STEPPED, PRESSURE_PLATE_RELEASED, MOVE_LEFT, MOVE_RIGHT, OPEN, CLOSE};
// activate pressure plates feature using class sequence
class Sequence {
public:

	Sequence() = default;

	Sequence(bool isRunning, bool isFinished, f32 targetValue, f32 startingValue, f32 currentValue, s32 sequenceType) :
		isRunning{ isRunning }, isFinished{ isFinished },
		targetValue{ targetValue }, startingValue{ startingValue },
		currentValue{ currentValue }, sequenceType{ sequenceType }
	{

	}

	inline void increaseCurrent(f32 value) {
		if (this->currentValue < targetValue) {
			this->currentValue += value;
		}
		else {
			this->isFinished = true;
		}
	}

	inline void executeSequence(f32 currentVal) {
		this->isRunning = true;
		this->currentValue = currentVal;
	}

	inline bool isSequenceRunning() {
		return this->isRunning;
	}

	inline s32 getSequenceType() {
		return this->sequenceType;
	}



	bool isRunning;
	bool isFinished;
	f32 targetValue;
	f32 startingValue;
	f32 currentValue;
	s32 sequenceType;


};
// special sequence call
struct SequenceCollection {
	int sequenceCount;
	Sequence* sequence;
	bool finished;

	inline Sequence* getSequence(SpecialSequence specialSeq) {
		for (int x = 0; x < this->sequenceCount; ++x) {
			Sequence* sequenceColl = this->sequence + x;
			if (sequenceColl->getSequenceType() == specialSeq) {
				return sequenceColl;
			}
		}

		return nullptr;
		
	}
};

#endif
	
