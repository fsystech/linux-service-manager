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
// 9:56 PM 2/11/2025
// by Rajib Chy

#ifdef USE_HTTP_DAY_STATUS

#include <svc/httpc.h>
#include <svc/http.h>

const char* http_client::get_host() const {
    return _host.c_str();
}

int http_client::get( const std::string&path, std::string& result ) {

    int sock = http_create_connection(_host.c_str(), _port.c_str());
    
    if (sock < 0) {
        set_last_error("Failed to connect to ", _host.c_str());
        return 0;
    }

    if (http_send_request(sock, _host.c_str(), path.c_str()) < 0) {
        set_last_error("Failed to send request to ", _host.c_str());
        close(sock);
        return 0;
    }

    char *response = NULL;
    if (http_read_response(sock, &response) < 0) {
        set_last_error("Failed to read response from ", _host.c_str());
        close(sock);
        return 0;
    }

    char *body = http_extract_body(response);
    std::string(body).swap(result);

    http_cleanup(sock, response);
    return 1;
}

const char* http_client::get_last_error() const{
    if(_last_error.empty()){
        return "";
    }
    return _last_error.c_str();
}

void http_client::set_last_error( const char* prefix, const char* error_detail ) {
    std::string(prefix).append(" ").append(error_detail).swap(_last_error);
}

#endif //!USE_HTTP_DAY_STATUS