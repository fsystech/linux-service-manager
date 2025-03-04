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
// 11:20 AM 2/17/2025
// by Rajib Chy
#ifdef _MSC_VER
#pragma once
#endif//_MSC_VER

#ifndef svc_dust_cleaner_h
#define svc_dust_cleaner_h

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <atomic>
#include <chrono>
#include <nlohmann/json.hpp>
#include <memory>
#include <svc/logger.h>

namespace fs = std::filesystem;

/**
 * @brief Configuration for dust cleaning operation.
 */
struct dust_clean_config {
    bool is_cache = false; /**< Indicates whether the file is a cache file. */
    std::string dust_dir; /**< Directory path to clean. */
    bool delete_empty_dir = false; /**< Flag to remove empty directories. */
    std::vector<std::string> extensions; /**< List of file extensions to clean. */
    // JSON serialization/deserialization
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(dust_clean_config, is_cache, dust_dir, delete_empty_dir, extensions)
};

/**
 * @brief Parses a JSON object into a dynamically allocated dust_clean_config structure.
 * 
 * @param j The JSON object containing the configuration data.
 * @param cfg A pointer to a dust_clean_config structure. If nullptr, a new instance is allocated.
 * 
 * @note If the "dir" field is missing or empty, the function sets it to the current directory.
 */
void _dust_config_from_json(const nlohmann::json& j, dust_clean_config*& cfg);

/**
 * @brief Handles cleaning of log files and empty directories.
 */
class dust_cleaner_t {

public:
    dust_cleaner_t( ) { }
    ~dust_cleaner_t( );
    dust_cleaner_t( dust_cleaner_t&& ) = default;
    dust_cleaner_t& operator=( dust_cleaner_t&& ) = delete;

private:
    std::atomic<bool> _is_disposed{false}; /**< Indicates if the cleaner has been disposed. */
    std::vector<dust_clean_config*> _dust_configs; /**< List of configurations for cleaning. */

    /**
     * @brief Determines if a file is old enough to be deleted.
     * @param creation_time File creation time.
     * @return True if the file should be deleted, false otherwise.
     */
    bool is_deletable( const fs::file_time_type& creation_time );

    /**
     * @brief Checks if a file needs to be deleted based on extension and age.
     * @param file_path Path to the file.
     * @param ext File extension to match.
     * @return True if the file should be deleted, false otherwise.
     */
    bool need_delete( const fs::path& file_path, const std::string& ext );

    /**
     * @brief Deletes log files from the specified directory.
     * @param logger Output stream for logging.
     * @param dir Directory to clean.
     * @param ext File extension to delete.
     * @param is_cache Indicates if the file is a cache file.
     */
    void delete_log_files( std::shared_ptr<svc_logger>& logger, const fs::path& dir, const std::string& ext, bool is_cache );

    /**
     * @brief Recursively deletes empty directories.
     * @param dir Directory to check.
     * @param logger Output stream for logging.
     */
    void clean_empty_dirs( const fs::path& dir, std::shared_ptr<svc_logger>& logger );

    /**
     * @brief Starts the cleaning process in a separate thread.
     */
    void start_clean( std::shared_ptr<svc_logger>& logger );

public:
    bool is_empty( ) const;
    /**
     * @brief Initiates the cleaning process.
     */
    void clean( std::shared_ptr<svc_logger>& logger );

    /**
     * @brief Sets the configuration for dust cleaning.
     * @param configs List of cleaning configurations.
     */
    void set_dust_config( std::vector<dust_clean_config*>& configs );

    /**
     * @brief Disposes the cleaner instance.
     */
    void dispose( );
};

#endif // svc_dust_cleaner_h
