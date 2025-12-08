#include "Events.h"
#include <Common/Globals.h>
#include <mumble/Mumble.h>

namespace Events {
	void OnMumbleIdentityUpdated(void *eventData) {
		G::MumbleIdentity = static_cast<Mumble::Identity *>(eventData);
		if (!G::MumbleIdentity)
			return;

		const uint32_t updatedMapID = G::MumbleIdentity->MapID;
		if (updatedMapID == WorldState::CurrentMapData.id)
			return;

		const auto mapEntry = WorldState::MapDataMap.find(updatedMapID);
		const bool mapFound = mapEntry != WorldState::MapDataMap.end();

		if (mapFound) {
			const auto &mapData = mapEntry->second;
			WorldState::CurrentContinent = mapData.continent_id == 1 ? Continent::Tyria : Continent::Mists;
			WorldState::CurrentMapData = mapData;
		} else {
			WorldState::CurrentContinent = Continent::Unknown;
			WorldState::CurrentMapData = {};
		}

		WorldState::IsOnSupportedMap = mapFound;
		WorldState::CurrentSectorID = 0; // actual sector detected elsewhere
	}
}