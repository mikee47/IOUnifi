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

#include <IO/Device.h>
#include "Controller.h"

namespace IO::Network::Unifi
{
class Request;

class Device : public IO::Device
{
	friend Request;

public:
	template <class DeviceClass> class FactoryTemplate : public IO::Device::Factory
	{
	public:
		IO::Device* createDevice(IO::Controller& controller, const char* id) const override
		{
			return new DeviceClass(static_cast<Controller&>(controller), id);
		}

		const FlashString& controllerClass() const override
		{
			return CONTROLLER_CLASSNAME;
		}
	};

	struct Config {
		IO::Device::Config base;
		String unifi_id;
	};

	ErrorCode init(const Config& config);
	ErrorCode init(JsonObjectConst config) override;

	Device(Controller& controller, const char* id) : IO::Device(controller, id)
	{
	}

	const DeviceType type() const override
	{
		return DeviceType::Custom;
	}

	IO::Request* createRequest() override;

	/**
	 * @brief Request may be redundant if it doesn't result in a change of device state
	 */
	virtual bool isRequestRequired(const Request& request) const = 0;

	/**
	 * @brief Get URI path for HTTP request
	 */
	virtual String getPath(const Request& request) const = 0;

	/**
	 * @brief Get body/content of POST request
	 * @retval String Return nullptr for GET request
	 */
	virtual String getBody(const Request& request) const = 0;

	virtual void parseResponse(String& body, const Request& request) = 0;

	virtual void getRequestJson(const Request& request, JsonObject json) const = 0;

protected:
	void parseJson(JsonObjectConst json, Config& cfg);

protected:
	CString unifi_id;
};

} // namespace IO::Network::Unifi
