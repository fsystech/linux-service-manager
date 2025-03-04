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
// 10:47 AM 2/13/2025
// by Rajib Chy

#ifdef _MSC_VER
#pragma once
#endif//_MSC_VER

#ifndef _fsys_svc_time_range_h
#define _fsys_svc_time_range_h

#include <iostream>
#include <chrono>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <memory>
#include <svc/logger.h>

class time_range_t {
public:
    /**
     * @brief Constructor to initialize time range and start printing thread.
     * @param start_time Start time in "HH:MM:SS" format.
     * @param end_time End time in "HH:MM:SS" format.
     */
    time_range_t( const std::string& start_time, const std::string& end_time, const std::string& restart_time );

    /**
     * @brief Checks if the given timestamp falls within the specified time range.
     *
     * This function verifies whether a provided timestamp (`now_time`) is within 
     * the start (`_start_epoch`) and end (`_end_epoch`) epoch values.
     * If either `_start_epoch` or `_end_epoch` is `0`, the function returns `true`,
     * assuming no restriction on the time range.
     *
     * @param[in] now_time The current time as a `std::time_t` value to check against the range.
     * @return `true` if `now_time` falls within the range, or if either epoch limit is `0`.
     *         Otherwise, returns `false`.
     */
    bool is_between_times( const std::time_t& now_time ) const;

    /**
     * @brief Determines if a restart is needed based on the current time.
     *
     * This function checks whether the provided timestamp (`now_time`) falls within 
     * 60 seconds of `_restart_epoch`. If `_restart_epoch` is `0`, it returns `false`.
     *
     * @param[in] now_time The current time as a `std::time_t` reference.
     * @return `true` if `now_time` is within 60 seconds of `_restart_epoch`, otherwise `false`.
     */
    bool need_restart( const std::time_t& now_time ) const;

    /**
     * @brief Prepares the time range by converting string representations of time 
     *        into epoch values (time_t) for comparison.
     * 
     * This function checks whether the start or end time is empty or set to a 
     * predefined "empty time" value. If so, it sets both `_start_epoch` and 
     * `_end_epoch` to 0. Otherwise, it converts the start and end times from 
     * string format to `tm` structures and then to epoch time.
     */
    void prepare( );

    bool is_restart_supported( ) const;

    void print( std::shared_ptr<svc_logger>& logger ) const;

private:
    std::string _restart_time; ///< Re-Start time in "HH:MM:SS" format.
    std::string _start_time;   ///< Start time in "HH:MM:SS" format.
    std::string _end_time;     ///< End time in "HH:MM:SS" format.
    //std::tm _start_tm;         ///< Parsed start time.
    //std::tm _end_tm;           ///< Parsed end time.
    std::time_t _restart_epoch;  ///< Re-Start time as time_t for comparison.
    std::time_t _start_epoch;  ///< Start time as time_t for comparison.
    std::time_t _end_epoch;    ///< End time as time_t for comparison.
    std::string _last_error;   ///< Stores the last error message.
};

/**
 * @brief Retrieves the current date in YYYY-mm-dd format.
 * 
 * This function fetches the current system date and formats it as a string
 * in the "YYYY-mm-dd" format. It ensures thread safety by using `localtime_s`
 * on Windows and `localtime_r` on POSIX systems.
 * 
 * @param[out] result Reference to a string where the formatted date will be stored.
 */
void _get_current_date( std::string& result );

/**
 * @brief Checks if a given string is a valid date in YYYY-mm-dd format.
 * 
 * This function first verifies that the input matches the YYYY-mm-dd pattern using regex.
 * It then ensures that the year, month, and day form a valid calendar date, 
 * considering leap years.
 * 
 * @param date_str The input date string to validate.
 * @return true if the date is valid, false otherwise.
 */
bool _is_valid_date( const std::string& date_str );

#endif //!_fsys_svc_time_range_h