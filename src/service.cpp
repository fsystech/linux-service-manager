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
// This file contains the 'main' function. Program execution begins and ends there.
// 9:44 AM 2/14/20251
// by Rajib Chy

#ifdef _MSC_VER
#pragma once
#endif //!_MSC_VER

#ifdef USE_PRODUCTION_BUILD

#include <signal.h>
#include <csignal>
#include <memory>
#include <atomic>
#include <stdexcept>
#include <exception>
#include <svc/handler.h>


static volatile std::sig_atomic_t _exit_signalled = 0;
static std::shared_ptr<service_handler_t> _handler;

static void on_signal_caught( int signaled ) {
    
    if (_exit_signalled) return;

    _exit_signalled = 1;

    if (!_handler) {
        fprintf(stderr, "Handler is null. Exiting.\n" );
        std::_Exit(EXIT_FAILURE);
        return;
    }

    _handler->_logger->info("Exit signal received; System Id: ", signaled);
    _handler->_logger->flush( );
    _handler->exit( );

}


static void _attach_signal( ) {
    // Interrupt application with ctrl+c or other(s)
    std::signal( SIGINT, on_signal_caught );
    // Software termination signal from kill
    std::signal( SIGTERM, on_signal_caught );
    // Abnormal termination triggered by abort call
    std::signal( SIGABRT, on_signal_caught );

#ifdef _WIN32
    // Ctrl-Break sequence
    std::signal( SIGBREAK, on_signal_caught );
#endif //!_WIN32
}

int main( int argc, char **argv ) {

    _attach_signal( );

    fprintf( stdout, "Initializing \"Service Manager\"\n" );

    fprintf( stdout, "Press Ctrl+C to exit...\n" );

    try {

        _handler = std::make_shared<service_handler_t>( );

    } catch( std::exception& w ) {

        fprintf( stderr, "%s\n", w.what( ) );
        return EXIT_FAILURE;

    }

    if( _handler->prepare( ) == 0 ) {
        _handler.reset();
        fprintf( stderr, "\"Service Manager\" exited with failed prepare call.\n" );
        return EXIT_FAILURE;
    }

    if ( _handler->block( ) == 0 ) {
        _handler.reset( );
        fprintf( stderr, "\"Service Manager\" exited with failed block call.\n" );
        return EXIT_FAILURE;
    }

    _handler->_logger->info( "\"Service Manager\" exited properly" );
    _handler.reset( );

    fprintf( stdout, "ALL IS WELL\n" );

    return EXIT_SUCCESS;
}

#endif // !USE_PRODUCTION_BUILD