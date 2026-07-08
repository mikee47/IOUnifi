/**
 * Unifi/Controller.cpp
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

#include "include/IO/Network/Unifi/Controller.h"
#include "include/IO/Network/Unifi/Request.h"
#include <Network/HttpClient.h>
#include <Data/Stream/LimitedMemoryStream.h>

namespace IO::Network::Unifi
{
DEFINE_FSTR(CONTROLLER_CLASSNAME, "unifi")

HttpClient httpClient;

void sslRequestInit(Ssl::Session& session, HttpRequest&)
{
	session.cipherSuites = &Ssl::CipherSuites::full;
	session.options.verifyLater = true;
}

constexpr unsigned requestTimeout = 2000;

ErrorCode Controller::init(JsonObjectConst config)
{
	ipAddr = config["ip"].as<const char*>();
	apiKey = config["api-key"].as<const char*>();
	return Error::success;
}

void Controller::handleEvent(IO::Request* request, Event event)
{
	switch(event) {
	case Event::Execute: {
		// TODO: Form Http request and submit it
		auto& req = static_cast<Request&>(*request);
		auto err = submitRequest(req);
		if(err) {
			request->complete(err);
			return;
		}

		// Put a timeout on the overall transaction
		timer.initializeMs<requestTimeout>(
			[](void* param) {
				auto request = static_cast<Request*>(param);
				request->handleEvent(Event::Timeout);
			},
			request);
		timer.startOnce();

		break;
	}

	case Event::RequestComplete:
		timer.stop();
		break;

	case Event::Timeout:
		httpClient.cleanup();
		break;

	case Event::TransmitComplete:
	case Event::ReceiveComplete:
		break;
	}

	IO::Controller::handleEvent(request, event);
}

ErrorCode Controller::submitRequest(Request& request)
{
	auto& device = request.getDevice();

	Url url;
	url.Scheme = URI_SCHEME_HTTP_SECURE;
	url.Host = toString(ipAddr);
	url.Path = device.getPath(request);

	if(!url.Path) {
		return Error::bad_param;
	}

	debug_i("Sending request %p to %s", &request, toString(url).c_str());

	HttpRequest* req = new HttpRequest(url);
	req->headers[HTTP_HEADER_USER_AGENT] = _F("HttpClient/Sming");
	req->headers[F("X-API-KEY")] = apiKey.c_str();
	req->headers[HTTP_HEADER_ACCEPT] = toString(MIME_JSON);
	req->onSslInit(sslRequestInit);
	req->setResponseStream(new LimitedMemoryStream(1024));
	req->onRequestComplete([&request](HttpConnection& conn, bool success) -> int {
		String body = conn.getResponse()->getBody();
		debug_i("UNIFI request complete %p, success %d: \r\n%s", &request, success, body.c_str());

		// TODO: For query we need to parse received response (but only on success)
		// device.parserResponse(body, request);

		request.complete(Error::success);
		return 0;
	});
	String body = device.getBody(request);
	if(body) {
		req->setBody(body);
		req->method = HTTP_POST;
	}

	// Second: We have to send that request using our httpClient
	if(!httpClient.send(req)) {
		return Error::busy;
	}

	return Error::success;
}

} // namespace IO::Network::Unifi
