/**
 * Unifi/Device.cpp
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

} // namespace IO::Network::Unifi
