/**
 * Custom/Device.h
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

	virtual int getNodeValue(IO::DevNode) const
	{
		return 0;
	}
};

} // namespace IO::Network::Unifi
