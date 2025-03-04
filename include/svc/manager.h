/*
* Copyright FSys Tech Limited [FSys]. All rights reserved.
*
* This software owned by FSys Tech Limited [FSys] and is protected by copyright law
* and international copyright treaties.
*
* Access to and use of the software is governed by the terms of the applicable FSys Software
* Services Agreement (the Agreement) and Customer end user license agreements granting
* a non-assignable, non-transferable and non-exclusive license to use the software
* for it's own data processing purposes under the terms defined in the Agreement.
*
* Except as otherwise granted within the terms of the Agreement, copying or reproduction of any part
* of this source code or associated reference material to any other location for further reproduction
* or redistribution, and any amendments to this copyright notice, are expressly prohibited.
*
* Any reproduction or redistribution for sale or hiring of the Software not in accordance with
* the terms of the Agreement is a violation of copyright law.
*/
// 8:26 PM 1/26/2025
// by Rajib Chy
#ifdef _MSC_VER
#pragma once
#endif//_MSC_VER

#ifndef _fsys_svc_manager_h
#define _fsys_svc_manager_h
#include <iostream>
#include <memory>
#include <cstring>
#include <sdbus-c++/sdbus-c++.h>

/**
 * @class service_manager_t
 * @brief A class to manage systemd services via the D-Bus API.
 *
 * The `service_manager_t` class provides an interface for interacting with systemd services.
 * It allows starting, stopping, restarting services, and querying their status using D-Bus.
 */
class service_manager_t {
public:
    /**
     * @brief Constructs the service manager and establishes a D-Bus connection.
     */
    service_manager_t( );

    /**
     * @brief Starts a systemd service.
     *
     * @param serviceName The name of the service to start (e.g., "example.service").
     * @return 0 if the service starts successfully, or -1 on failure.
     */
    int start( const std::string& serviceName );

    /**
     * @brief Stops a systemd service.
     *
     * @param serviceName The name of the service to stop (e.g., "example.service").
     * @return 0 if the service stops successfully, or -1 on failure.
     */
    int stop( const std::string& serviceName );

    /**
     * @brief Restarts a systemd service.
     *
     * @param serviceName The name of the service to restart (e.g., "example.service").
     * @return 0 if the service restarts successfully, or -1 on failure.
     */
    int restart( const std::string& serviceName );

	/**
	 * @brief Retrieves the status of a systemd service.
	 *
	 * @param serviceName The name of the service (e.g., "example.service").
	 * @param result A string to store the status of the service.
	 *              Expected values include:
	 *              - "active"       : The service is running or has successfully completed.
	 *              - "reloading"    : The service is reloading its configuration.
	 *              - "inactive"     : The service is not running.
	 *              - "failed"       : The service encountered an error during execution.
	 *              - "activating"   : The service is in the process of starting.
	 *              - "deactivating" : The service is in the process of stopping.
	 *              - "maintenance"  : The service entered maintenance mode due to repeated failures.
	 * @return 0 if the status is retrieved successfully, or -1 on failure.
	 */
	int get_status( const std::string& serviceName, std::string& result );

    /**
     * @brief Gets the last error message.
     *
     * @return A C-string representing the last error message.
     */
    const char* get_last_error( );

private:
    /**
     * @brief Sets the last error message.
     *
     * @param method The method name where the error occurred.
     * @param message A description of the error.
     */
    void set_last_error( const std::string& method, const std::string& message );

    /**
     * @brief Helper function to call systemd methods like StartUnit, StopUnit, or RestartUnit.
     *
     * @param method The method name to call (e.g., "StartUnit").
     * @param serviceName The name of the service (e.g., "example.service").
     * @param mode The mode for the operation (e.g., "replace").
     * @return 0 if the method call succeeds, or -1 on failure.
     */
    int call_systemd_method( const std::string& method, const std::string& serviceName, const std::string& mode );

private:
    std::string _last_error; ///< Stores the last error message encountered.
    std::unique_ptr<sdbus::IConnection> _connection; ///< The D-Bus connection instance.
};

/**
 * @brief Ensures that a service name has the required extension.
 *
 * This function checks whether the given service name contains a period ('.') 
 * to determine if it already has an extension. If not, it appends `SERVICE_EXT` 
 * (which is presumably `.service`).
 *
 * @param service_name The name of the service to normalize.
 */
void _normalized_service_name( std::string& serviceName );

#endif //!_fsys_svc_manager_h