/*!
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
// 11:22 AM 2/13/2025
// by Rajib Chy

#ifdef _MSC_VER
#pragma once
#endif//_MSC_VER

#ifndef _fsys_svc_config_h
#define _fsys_svc_config_h

#include <cstring>
#include <string>
#include <vector>
#include <svc/time-range.h>
#include <svc/dust-cleaner.h>

/**
 * @enum service_state
 * @brief Represents the state of a service.
 */
enum class service_state {
    ACTIVE,   ///< Service is currently active and running.
    INACTIVE, ///< Service is not running.
    ERROR     ///< Service encountered an error state.
};

struct svc_config {
    bool is_restarted = false;
    bool required_workday = false;
    bool is_restart_support = false;
    bool has_dependent_service = false;
    std::string service_name; // fixc_dse.service
    std::string start_time;// "08:30:15";
    std::string end_time;// "23:10:15";
    std::string restart_time;
    time_range_t* time_range = nullptr; // time_range_t
    service_state state = service_state::INACTIVE;
    std::vector<std::string> dependent; /**< List of dependent service. */
};


#ifdef USE_HTTP_DAY_STATUS

void _load_config( 
    std::vector<svc_config*>& svc_configs, 
    std::vector<dust_clean_config*>& dust_configs,
    std::string& http_server, std::string& http_port 
);

#else

void _load_config( 
    std::vector<svc_config*>& svc_configs, 
    std::vector<dust_clean_config*>& dust_configs
);

#endif //!USE_HTTP_DAY_STATUS

#endif //!_fsys_svc_config_h