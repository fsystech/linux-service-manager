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
// 9:38 PM 2/11/2025
// by Rajib Chy
#ifdef _MSC_VER
#pragma once
#endif//_MSC_VER

#ifndef _fsys_httpw_h
#define _fsys_httpw_h

#ifdef USE_HTTP_DAY_STATUS

#include <cstring>
#include <string>

/**
 * @brief A simple HTTP client for sending GET requests.
 *
 * @details
 * This class provides a basic HTTP client that allows sending GET requests
 * to a specified host and port. It handles connection establishment, 
 * request sending, response reading, and error management.
 */
class http_client {
public:
    /**
     * @brief Constructs an HTTP client with the given host and port.
     *
     * @param host The hostname or IP address of the server.
     * @param port The port number as a string (default is "80" for HTTP).
     *
     * @details
     * Initializes the HTTP client with the target server's address and port.
     * The socket descriptor is initialized to -1.
     */
    http_client(const std::string& host, const std::string& port = "80")
        : _host(host), _port(port), _sock(-1) {}

    /**
     * @brief Sends an HTTP GET request to the specified path and retrieves the response body.
     *
     * @param path The resource path to request (e.g., "/index.html").
     * @param result A reference to a string where the response body will be stored.
     * @return 1 on success, 0 on failure.
     *
     * @details
     * This method establishes an HTTP connection using the stored host and port values,
     * sends a GET request, reads the response, extracts the response body, and stores
     * it in the provided `result` string. If any step fails, it sets an error message
     * via `set_last_error()` and returns 0.
     *
     * Resources are properly cleaned up before returning.
     */
    int get(const std::string& path, std::string& result);

    /**
     * @brief Retrieves the last error message.
     *
     * @return A C-string containing the last error message.
     *
     * @details
     * This method returns a pointer to the last error message set by the class.
     * The returned string is owned by the class and should not be freed by the caller.
     */
    const char* get_last_error() const;
    
    /**
     * @brief Retrieves the hostname of the HTTP client.
     * 
     * @return A pointer to a null-terminated string containing the hostname.
     *         The returned pointer is valid as long as the http_client instance exists.
     */
    const char* get_host() const;

private:
    /**
     * @brief Sets the last error message by concatenating a prefix and error details.
     *
     * @param prefix A brief description of the error source.
     * @param error_detail A specific error message or detail.
     *
     * @details
     * This method formats and stores the error message in `_last_error`, allowing 
     * retrieval via `get_last_error()`.
     */
    void set_last_error(const char* prefix, const char* error_detail);

private:
    int _sock;              ///< The socket file descriptor for the HTTP connection.
    std::string _host;      ///< The target hostname or IP address.
    std::string _port;      ///< The target port number as a string.
    std::string _last_error; ///< The last recorded error message.
};

#endif // !USE_HTTP_DAY_STATUS

#endif //!_fsys_httpw_h