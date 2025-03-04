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

#ifndef _fsys_http_h
#define _fsys_http_h

#ifdef USE_HTTP_DAY_STATUS

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netdb.h>
    #include <unistd.h>
#endif //!_WIN32

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

/**
 * @brief Establishes an HTTP connection to the specified host and port.
 *
 * @param host The hostname or IP address of the server.
 * @param port The port number as a string (e.g., "80" for HTTP).
 * @return A socket file descriptor on success, or -1 on failure.
 */
int http_create_connection(const char *host, const char *port);

/**
 * @brief Sends an HTTP GET request to the specified host and path.
 *
 * @param sock The socket file descriptor obtained from http_create_connection().
 * @param host The hostname to include in the HTTP request header.
 * @param path The resource path to request (e.g., "/index.html").
 * @return 0 on success, or -1 on failure.
 */
int http_send_request(int sock, const char *host, const char *path);

/**
 * @brief Reads the HTTP response from the server.
 *
 * @param sock The socket file descriptor used for the connection.
 * @param response A pointer to a dynamically allocated string to store the response.
 *                 The caller must free this memory after use.
 * @return The number of bytes read on success, or -1 on failure.
 */
int http_read_response(int sock, char **response);

/**
 * @brief Extracts the body from an HTTP response.
 *
 * @param response The full HTTP response, including headers.
 * @return A pointer to the start of the response body within the given string.
 *         The original response string must not be modified or freed before using this pointer.
 */
char *http_extract_body(char *response);

/**
 * @brief Cleans up resources associated with the HTTP connection.
 *
 * @param sock The socket file descriptor to close.
 * @param response The allocated response buffer to free. Can be NULL.
 */
void http_cleanup(int sock, char *response);

#endif //!USE_HTTP_DAY_STATUS

#endif //!_fsys_http_h