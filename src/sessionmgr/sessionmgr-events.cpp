//  OpenVPN 3 Linux client -- Next generation OpenVPN client
//
//  SPDX-License-Identifier: AGPL-3.0-only
//
//  Copyright (C) 2020 - 2023  OpenVPN Inc <sales@openvpn.net>
//  Copyright (C) 2020 - 2023  David Sommerseth <davids@openvpn.net>
//

/**
 * @file   sessionmgr-events.cpp
 *
 * @brief  Implementation of SessionManager::Event signalling class
 */

#include <gio/gio.h>

#include "sessionmgr-events.hpp"
#include "dbus/glibutils.hpp"


SessionManager::Event::Event(const std::string &path,
                             EventType type,
                             uid_t owner)
    : path(path), type(type), owner(owner){};


SessionManager::Event::Event(GVariant *params)
{
    std::string g_type(g_variant_get_type_string(params));
    if ("(oqu)" != g_type)
    {
        THROW_SESSIONMGR("Invalid data type for SessionManager::Event()");
    }

    path = GLibUtils::ExtractValue<std::string>(params, 0);
    type = (SessionManager::EventType)GLibUtils::ExtractValue<uint16_t>(params, 1);
    owner = GLibUtils::ExtractValue<uid_t>(params, 2);

    if (type > SessionManager::EventType::SESS_DESTROYED
        || type < SessionManager::EventType::SESS_CREATED)
    {
        THROW_SESSIONMGR("Invalid SessionManager::EventType value");
    }
}


bool SessionManager::Event::empty() const noexcept
{
    return path.empty()
           && (SessionManager::EventType::UNSET == type)
           && 65535 == owner;
}


GVariant *SessionManager::Event::GetGVariant() const noexcept
{
    GVariantBuilder *b = g_variant_builder_new(G_VARIANT_TYPE("(oqu)"));
    g_variant_builder_add(b, "o", path.c_str());
    g_variant_builder_add(b, "q", (guint16)type);
    g_variant_builder_add(b, "u", owner);

    GVariant *ret = g_variant_builder_end(b);
    g_variant_builder_clear(b);
    g_variant_builder_unref(b);
    return ret;
}


bool SessionManager::Event::operator==(const SessionManager::Event &compare) const
{
    return ((compare.type == (const SessionManager::EventType)type)
            && (compare.owner == owner)
            && (0 == compare.path.compare(path)));
}


bool SessionManager::Event::operator!=(const SessionManager::Event &compare) const
{
    return !(this->operator==(compare));
}
