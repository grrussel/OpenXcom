/*
 * Copyright 2012 OpenXcom Developers.
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
#include "AlienStrategy.h"
#include "SavedGame.h"
#include "WeightedOptions.h"
#include "../Engine/RNG.h"
#include "../Ruleset/Ruleset.h"
#include "../Ruleset/RuleRegion.h"

namespace OpenXcom
{

typedef std::map<std::string, WeightedOptions*> MissionsByRegion;

/**
 * Create an AlienStrategy with no values.
 * Running a game like this will most likely crash.
 */
AlienStrategy::AlienStrategy()
{
	// Empty by design!
}

/**
 * Free all resources used by this AlienStrategy.
 */
AlienStrategy::~AlienStrategy()
{
	// Free allocated memory.
	for (MissionsByRegion::iterator ii = _regionMissions.begin(); ii != _regionMissions.end(); ++ii)
	{
		delete ii->second;
	}
}

/**
 * Get starting values from the rules.
 * @param rules Pointer to the game ruleset.
 */
void AlienStrategy::init(const Ruleset *rules)
{
	std::vector<std::string> regions = rules->getRegionsList();

	for (std::vector<std::string>::const_iterator rr = regions.begin(); rr != regions.end(); ++rr)
	{
		RuleRegion *region = rules->getRegion(*rr);
		_regionChances.set(*rr, region->getWeight());
		WeightedOptions *missions = new WeightedOptions(region->getAvailableMissions());
		_regionMissions.insert(std::make_pair(*rr, missions));
	}
}

/**
 * Loads the data from a YAML file.
 * @param rules Pointer to the game ruleset.
 * @param node YAML node.
 */
void AlienStrategy::load(const Ruleset *rules, const YAML::Node &node)
{
	// Free allocated memory.
	for (MissionsByRegion::iterator ii = _regionMissions.begin(); ii != _regionMissions.end(); ++ii)
	{
		delete ii->second;
	}
	_regionMissions.clear();
	_regionChances.clear();
	_regionChances.load(node["regions"]);
	const YAML::Node &strat = node["possibleMissions"];
	for (YAML::Iterator nn = strat.begin(); nn != strat.end(); ++nn)
	{
		std::string region;
		(*nn)["region"] >> region;
		const YAML::Node &missions = (*nn)["missions"];
		std::auto_ptr<WeightedOptions> options(new WeightedOptions());
		options->load(missions);
		_regionMissions.insert(std::make_pair(region, options.release()));
	}
}

/**
 * Saves the alien data to a YAML file.
 * @param out YAML emitter.
 */
void AlienStrategy::save(YAML::Emitter &out) const
{
	out << YAML::BeginMap;
	out << YAML::Key << "regions" << YAML::Value;
	_regionChances.save(out);
	out << YAML::Key << "possibleMissions" << YAML::Value;
	out << YAML::BeginSeq;
	for (MissionsByRegion::const_iterator ii = _regionMissions.begin(); ii != _regionMissions.end(); ++ii)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "region" << YAML::Value << ii->first;
		out << YAML::Key << "missions" << YAML::Value;
		ii->second->save(out);
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
}

/**
 * Choose one of the regions for a mission.
 * @return The region id.
 */
const std::string &AlienStrategy::chooseRandomRegion() const
{
	return _regionChances.choose();
}

/**
 * Choose one missions available for @a region.
 * @param region The region id.
 * @return The mission id.
 */
const std::string &AlienStrategy::chooseRandomMission(const std::string &region) const
{
	MissionsByRegion::const_iterator found = _regionMissions.find(region);
	assert(found != _regionMissions.end());
	return found->second->choose();
}

/**
 * Remove @a mission from the list of possible missions for @a region.
 * @param region The region id.
 * @param mission The mission id.
 * @return If there are no more regions with missions available.
 */
bool AlienStrategy::removeMission(const std::string &region, const std::string &mission)
{
	MissionsByRegion::iterator found = _regionMissions.find(region);
	assert(found != _regionMissions.end());
	found->second->set(mission, 0);
	if (found->second->empty())
	{
		_regionMissions.erase(found);
		_regionChances.set(region, 0);
	}
	return _regionMissions.empty();
}

}
