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
// 4:10 PM 4/23/2023
// by Rajib Chy

#ifdef _MSC_VER
#pragma once
#endif//_MSC_VER

#ifndef _fsys_json_config_h
#define _fsys_json_config_h
#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>

/**
 * @file json_config.h
 * @brief Defines the json_config_t class for managing JSON data configurations.
 */

using json = nlohmann::json;

/**
 * @enum json_value_t
 * @brief Enumeration of possible JSON value types.
 */
enum class json_value_t {
	null,             ///< Represents a null value.
	object,           ///< Represents a JSON object (unordered set of key-value pairs).
	array,            ///< Represents a JSON array (ordered collection of values).
	string,           ///< Represents a string value.
	boolean,          ///< Represents a boolean value.
	number_integer,   ///< Represents a signed integer value.
	number_unsigned,  ///< Represents an unsigned integer value.
	number_float,     ///< Represents a floating-point number.
	binary,           ///< Represents a binary array (ordered collection of bytes).
	discarded         ///< Represents a value discarded by the parser callback function.
};

/**
 * @class json_config_t
 * @brief A utility class for handling JSON-based configurations.
 */
class json_config_t {
public:
	/**
	 * @brief Default constructor.
	 */
	json_config_t() {}

	/**
	 * @brief Destructor.
	 */
	~json_config_t();

	/**
	 * @brief Constructs a json_config_t object from a JSON object.
	 * @param data Reference to a JSON object.
	 * @param is_array Flag indicating if the JSON should be treated as an array (default: 0).
	 */
	json_config_t(json& data, int is_array = 0);

	/**
	 * @brief Constructs a json_config_t object from a JSON file.
	 * @param path Path to the JSON file.
	 * @param is_array Flag indicating if the JSON should be treated as an array (default: 0).
	 */
	json_config_t(const std::string& path, int is_array = 0);

	/**
	 * @brief Retrieves a string value from the JSON object.
	 * @param key Key to look up.
	 * @param result Reference to store the retrieved string.
	 * @return 1 if the key exists, 0 otherwise.
	 */
	int get_string(const char* key, std::string& result);

	/**
	 * @brief Retrieves an integer value from the JSON object.
	 * @param key Key to look up.
	 * @param result Pointer to store the retrieved integer.
	 * @return 1 if the key exists, 0 otherwise.
	 */
	int get_int(const char* key, int* result);

	/**
	 * @brief Retrieves a boolean value from the JSON object.
	 * @param key Key to look up.
	 * @param result Pointer to store the retrieved boolean.
	 * @return 1 if the key exists, 0 otherwise.
	 */
	int get_bool(const char* key, bool* result);

	/**
	 * @brief Retrieves a double value from the JSON object.
	 * @param key Key to look up.
	 * @param result Pointer to store the retrieved double.
	 * @return 1 if the key exists, 0 otherwise.
	 */
	int get_double(const char* key, double* result);

	/**
	 * @brief Retrieves a dynamically-typed value from the JSON object.
	 * @tparam DynamicType The expected type of the retrieved value.
	 * @param key Key to look up.
	 * @param result Pointer to store the retrieved value.
	 * @return 1 if the key exists, 0 otherwise.
	 * @throws std::runtime_error If the JSON data is not an object.
	 */
	template<class DynamicType>
	inline int get_dynamic(const char* key, DynamicType* result) {
		throw_if_not_object();
		if (!_data.contains(key)) return 0;
		*result = _data[key].get<DynamicType>();
		return 1;
	}

	/**
	 * @brief Retrieves a dynamically-typed value from the JSON object.
	 * @tparam DynamicType The expected type of the retrieved value.
	 * @param key Key to look up.
	 * @param result Pointer to store the retrieved value.
	 * @return 1 if the key exists, 0 otherwise.
	 * @throws std::runtime_error If the JSON data is not an object.
	 */
	template<class DynamicType>
	inline int get_to(const char* key, DynamicType* result) {
		throw_if_not_object();

		if( !_data.contains( key ) ) {
			return 0;
		}
		
		_data.at( key ).get_to( *result );
		return 1;
	}
	/**
	 * @brief Retrieves a subpart of the JSON object.
	 * @param key Key to look up.
	 * @param part Reference to store the retrieved JSON subpart.
	 * @param is_array Flag indicating if the subpart should be treated as an array (default: 0).
	 * @return 1 if the key exists, 0 otherwise.
	 */
	int get_next_part(const char* key, json_config_t& part, int is_array = 0);

	/**
	 * @brief Iterates over each element in a JSON array and applies a function.
	 * @param next Function to apply to each json_config_t element.
	 */
	void each(std::function<void(json_config_t&)> next);
	
	/**
	 * @brief Iterates over each key-value pair in the JSON object.
	 * 
	 * This function takes a callback function and applies it to every key-value 
	 * pair in the JSON object. The callback receives the key as a `std::string` 
	 * and a reference to the corresponding JSON value.
	 * 
	 * @param next A callback function with the signature 
	 *        `void(const std::string& key, json &val)`, where:
	 *        - `key` is the JSON object's key.
	 *        - `val` is a reference to the corresponding value.
	 * 
	 * @note The function is useful for processing JSON objects dynamically.
	 */
	void each_keys(std::function<void(const std::string& key, json &val)> next);

	/**
	 * @brief Clears the JSON data.
	 */
	void clear();

private:

	/**
	 * @brief Throws an exception if the JSON data is not an array.
	 * @throws std::runtime_error If the JSON data is not an array.
	 */
	void throw_if_not_array();

	/**
	 * @brief Throws an exception if the JSON data is not an object.
	 * @throws std::runtime_error If the JSON data is not an object.
	 */
	void throw_if_not_object();

private:
	json _data;                     ///< Internal JSON data storage.
	json_value_t _data_type = json_value_t::null;  ///< Type of the JSON data.
};

#endif
