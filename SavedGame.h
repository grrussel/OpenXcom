/*
 * Copyright 2010 Daniel Albano
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OPENXCOM__SAVEDGAME_H
#define OPENXCOM__SAVEDGAME_H

#include <map>
#include "GameTime.h"
#include "Country.h"

enum GameDifficulty { DIFF_BEGINNER, DIFF_EXPERIENCED, DIFF_VETERAN, DIFF_GENIUS, DIFF_SUPERHUMAN };

class SavedGame
{
private:
	GameDifficulty _diff;
	GameTime *_time;
	map<LangString, Country*> _countries;
public:
	SavedGame();
	~SavedGame();
	GameTime *getTime();
	map<LangString, Country*> *getCountries();
};

#endif