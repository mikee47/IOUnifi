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

namespace IO::Network::Unifi
{
const UslRelay::Factory UslRelay::factory;

bool UslRelay::isRequestRequired(const Request& request) const
{
	switch(request.getCommand()) {
	case Command::off:
		return states[request.getNode().id] != DevNode::State::off;
	case Command::on:
		return states[request.getNode().id] != DevNode::State::on;
	case Command::undefined:
	case Command::query:
	case Command::toggle:
	case Command::latch:
	case Command::momentary:
	case Command::delay:
	case Command::set:
	case Command::adjust:
	case Command::update:
		break;
	};

	return true;
}

String UslRelay::getPath(const Request& request) const
{
	String path;
	path += F("/proxy/protect/integration/v1/relays/");
	path += unifi_id.c_str();

	switch(request.getCommand()) {
	case Command::query:
		break;
	case Command::off:
	case Command::on:
		path += F("/outputs/");
		path += request.getNode().id;
		path += F("/activate");
		break;
	case Command::undefined:
	case Command::toggle:
	case Command::latch:
	case Command::momentary:
	case Command::delay:
	case Command::set:
	case Command::adjust:
	case Command::update:
		return nullptr;
	};

	return path;
}

String UslRelay::getBody(const Request& request) const
{
	String s;

	switch(request.getCommand()) {
	case Command::off:
		s = "off";
		break;
	case Command::on:
		s = "on";
		break;
	case Command::undefined:
	case Command::query:
	case Command::toggle:
	case Command::latch:
	case Command::momentary:
	case Command::delay:
	case Command::set:
	case Command::adjust:
	case Command::update:
		return nullptr;
	};

	return F("{\"state\":\"") + s + F("\",\"pulseDuration\":0}");
}

void UslRelay::parseResponse(String& body, const Request& request)
{
	switch(request.getCommand()) {
	case Command::undefined:
	case Command::query: {
		DynamicJsonDocument doc(1024);
		Json::deserialize(doc, body);
		auto json = doc.as<JsonObject>();
		for(JsonObject output : json["outputs"].as<JsonArray>()) {
			unsigned id = output["id"];
			if(id < outputCount) {
				String state = output["state"].as<const char*>();
				if(state == "on") {
					states[id] = DevNode::State::on;
				} else if(state == "off") {
					states[id] = DevNode::State::off;
				} else {
					states[id] = DevNode::State::unknown;
				}
			}
		}
		break;
	}
	case Command::off:
		states[request.getNode().id] = DevNode::State::off;
		break;
	case Command::on:
		states[request.getNode().id] = DevNode::State::on;
		break;
	case Command::toggle:
	case Command::latch:
	case Command::momentary:
	case Command::delay:
	case Command::set:
	case Command::adjust:
	case Command::update:
		break;
	};
}

void UslRelay::getRequestJson(const Request& request, JsonObject json) const
{
	for(unsigned i = 0; i < outputCount; ++i) {
		String tag = F("output") + i;
		switch(states[i]) {
		case DevNode::State::off:
			json[tag] = "off";
			break;
		case DevNode::State::on:
			json[tag] = "on";
			break;
		case DevNode::State::unknown:
		default:
			json[tag] = "unknown";
			break;
		}
	}
}

} // namespace IO::Network::Unifi
