//  OpenVPN 3 Linux client -- Next generation OpenVPN client
//
//  SPDX-License-Identifier: AGPL-3.0-only
//
//  Copyright (C) 2018 - 2023  OpenVPN Inc <sales@openvpn.net>
//  Copyright (C) 2018 - 2023  David Sommerseth <davids@openvpn.net>
//  Copyright (C) 2018 - 2023  Arne Schwabe <arne@openvpn.net>
//  Copyright (C) 2019 - 2023  Lev Stipakov <lev@openvpn.net>
//  Copyright (C) 2021 - 2023  Antonio Quartulli <antonio@openvpn.net>
//  Copyright (C) 2021 - 2023  Heiko Hund <heiko@openvpn.net>
//

/**
 * @file   proxy-netcfg-mgr.hpp
 *
 * @brief  Declaration of the D-Bus proxy for the main manager object
 *         of the net.openvpn.v3.netcfg service
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "dbus/core.hpp"
#include "dbus/proxy.hpp"
#include "netcfg-changeevent.hpp"
#include "netcfg-exception.hpp"
#include "netcfg-subscriptions.hpp"

using namespace openvpn;


namespace NetCfgProxy {
class Device;

class Manager : public DBusProxy
{
  public:
    using Ptr = std::shared_ptr<Manager>;

    /**
     *  Initialize the Network Configuration proxy for the
     *  main management interface
     *
     * @param dbuscon  D-Bus connection to use for D-Bus calls
     */
    Manager(GDBusConnection *dbuscon);

    const std::string GetConfigFile();

    const std::string CreateVirtualInterface(const std::string &device_name);

#ifdef OPENVPN3_NETCFGPRX_DEVICE
    Device *getVirtualInterface(const std::string &path)
    {
        return new Device(GetConnection(), path);
    }
#endif

    std::vector<std::string> FetchInterfaceList();
    bool ProtectSocket(int socket, const std::string &remote, bool ipv6, const std::string &devpath);
    bool DcoAvailable();
    void Cleanup();

    void NotificationSubscribe(NetCfgChangeType filter_flags);
    void NotificationUnsubscribe(const std::string &subscriber);
    void NotificationUnsubscribe();
    NetCfgSubscriptions::NetCfgNotifSubscriptions NotificationSubscriberList();
};
} // namespace NetCfgProxy
