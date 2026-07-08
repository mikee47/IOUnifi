/**
 * Unifi/UslRelay.cpp
 *
 * Copyright 2026 mikee47 <mike@sillyhouse.net>
 *
 * This file is part of the IOControl Library
 *
 * This library is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, version 3 or later.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this library.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 ****/

#include "include/IO/Network/Unifi/UslRelay.h"
#include "include/IO/Network/Unifi/Request.h"
#include <IO/Strings.h>

namespace IO::Network::Unifi
{
const UslRelay::Factory UslRelay::factory;

String UslRelay::getPath(const Request& request) const
{
	String path;
	path += F("/proxy/protect/integration/v1/relays/");
	path += unifi_id.c_str();

	switch(request.getCommand()) {
	case Command::undefined: // Undefined or invalid
		return nullptr;
	case Command::query: // Query node states
		break;
	case Command::off: // Turn node off or set to minimum
	case Command::on:  // Turn node on or set to maximum
		path += F("/outputs/");
		path += request.getNode().id;
		path += F("/activate");
		break;
	case Command::toggle:	// Toggle node(s) between on and off
	case Command::latch:	 // Relay nodes
	case Command::momentary: // Relay nodes
	case Command::delay:	 // Relay nodes
	case Command::set:		 // Set value
	case Command::adjust:	// Adjust value
	case Command::update:	// Perform update cycle (e.g. DMX512)
		return nullptr;
	};

	return path;
}

String UslRelay::getBody(const Request& request) const
{
	String s;
	s += "{\"state\":\"";

	switch(request.getCommand()) {
	case Command::undefined: // Undefined or invalid
	case Command::query:	 // Query node states
		return nullptr;
	case Command::off: // Turn node off or set to minimum
		s += "off";
		break;
	case Command::on: // Turn node on or set to maximum
		s += "on";
		break;
	case Command::toggle:	// Toggle node(s) between on and off
	case Command::latch:	 // Relay nodes
	case Command::momentary: // Relay nodes
	case Command::delay:	 // Relay nodes
	case Command::set:		 // Set value
	case Command::adjust:	// Adjust value
	case Command::update:	// Perform update cycle (e.g. DMX512)
		return nullptr;
	};
	s += "\",\"pulseDuration\":0}";
	return s;
}

} // namespace IO::Network::Unifi
