/**
 * Custom/UslRelay.h
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

#pragma once

#include "Device.h"

namespace IO::Network::Unifi
{
class UslRelay : public Device
{
public:
	static constexpr unsigned outputCount{2};

	class Factory : public FactoryTemplate<UslRelay>
	{
	public:
		const FlashString& deviceClass() const override
		{
			DEFINE_FSTR_LOCAL(DEVICE_CLASSNAME, "usl-relay")
			return DEVICE_CLASSNAME;
		}
	};

	static const Factory factory;

	using Device::Device;

	uint16_t maxNodes() const override
	{
		return outputCount;
	}

	DevNode::States getNodeStates(DevNode node) const override
	{
		if(node == DevNode_ALL) {
			DevNode::States res;
			for(auto state : states) {
				res |= state;
			}
			return res;
		}
		if(node.id < outputCount) {
			return states[node.id];
		}
		return DevNode::State::unknown;
	}

	bool isRequestRequired(const Request& request) const override;
	String getPath(const Request& request) const override;
	String getBody(const Request& request) const override;
	ErrorCode parseResponse(Request& request, const HttpResponse& response) override;
	void getRequestJson(const Request& request, JsonObject json) const override;

private:
	DevNode::State states[outputCount]{};
};

} // namespace IO::Network::Unifi
