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
#ifndef OPENXCOM__MUSIC_H
#define OPENXCOM__MUSIC_H

#include "SDL_mixer.h"
#include <string>
#include <sys/stat.h>

using namespace std;

/**
 * @file Music.h
 * Container for music tracks.
 * Handles loading and playing various formats through SDL_mixer.
 */
class Music
{
private:
	Mix_Music *_music;
public:
	/// Creates a blank music track.
	Music();
	/// Cleans up the music track.
	~Music();
	/// Loads music from the specified file.
	bool load(string filename);
	/// Plays the music.
	void play();
};

#endif