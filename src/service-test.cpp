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

#ifndef USE_PRODUCTION_BUILD

#include <cstring>
#include <iostream>
#include <svc/httpc.h>
#include <svc/logger.h>
#include <svc/manager.h>

int main( int argc, char** argv ) {

    svc_logger logger;
    
    if( logger.open() < 0 ) {
        return EXIT_FAILURE;
    }
   
    logger.info("Test http request\n");
    logger.flush();

    http_client http("snm.fsys.tech","80");

    std::string body;

    if( http.get("/", body) == 0) {
        
        logger.error( http.get_last_error(), "\n" );
        logger.close();

        return EXIT_FAILURE;
    }

    logger.info( body.c_str(), "\n" );

    if ( argc < 2 ) {
        logger.error("Invalid arguments. Service Name and Task requried.\n" );
        logger.close();
        return EXIT_FAILURE;
    }
    int result = 0;
    service_manager_t svc_manager;

    std::string svc_task = std::string( argv[1] );
    std::string svc_name = std::string( argv[2] );

    _normalized_service_name( svc_name );

    if ( svc_task == "start" ) {

        result = svc_manager.start( svc_name );

    } else if ( svc_task == "restart" ) {

        result = svc_manager.restart( svc_name );

    } else if ( svc_task == "stop" ) {

        result = svc_manager.stop( svc_name );

    } else if ( svc_task == "status" ) {

        std::string status;
        result = svc_manager.get_status( svc_name, status );
        if ( result > 0 ) {
            logger.info( svc_name.c_str( ), " status ", status.c_str( ), "\n" );
        }

    }
    if ( result < 0 ) {

        logger.error( "We are unable to process ", svc_name.c_str( ), " ", svc_task.c_str( ), "\n" );
        logger.error( "Due to Error: ", svc_manager.get_last_error( ), "\n" );

    } else {

        logger.info( svc_name.c_str( ), " status changed to ", svc_task.c_str( ) ," success\n" );
    }
    
    logger.close();

    return EXIT_SUCCESS;
}

#endif //!USE_PRODUCTION_BUILD