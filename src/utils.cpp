/*
 * pgpry - PGP private key recovery
 * Copyright (C) 2010 Jonas Gehring
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * file: utils.cpp
 * Miscellaneous utility functions
 */


#include <cerrno>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>

#include <arpa/inet.h>

#include "utils.h"


namespace Utils
{

// Converts a number to big endian format
uint32_t toBigEndian(uint32_t i)
{
	return htons(i);
}

// Converts a number from big endian format (to the host format)
uint32_t fromBigEndian(uint32_t i)
{
	return ntohs(i);
}

// Wrapper for strtol()
template<typename T>
static bool tstr2int(const std::string &str, T *i)
{
	char *end;
	long val = strtol(str.c_str(), &end, 0);

	if (errno == ERANGE || str.c_str() == end
		|| val > std::numeric_limits<int32_t>::max()
	    || val < std::numeric_limits<int32_t>::min()) {
		return false;
	}

	*i = (T)val;
	return true;
}

// Wrapper for strtol()
bool str2int(const std::string &str, int32_t *i)
{
	return tstr2int<int32_t>(str, i);
}

// Wrapper for strtol()
bool str2int(const std::string &str, uint32_t *i)
{
	return tstr2int<uint32_t>(str, i);
}

// Converts an interger to a string
std::string int2str(int32_t i)
{
	std::stringstream out;
	out << i;
	return out.str();
}

// sprintf for std::string
std::string strprintf(const char *format, ...)
{
	va_list vl;
	va_start(vl, format);

	std::ostringstream os;

	const char *ptr = format-1;
	while (*(++ptr) != '\0') {
		if (*ptr != '%') {
			os << *ptr;
			continue;
		}

		++ptr;

		// Only a subset of format specifiers is supported
		switch (*ptr) {
			case 'd':
			case 'i':
				os << va_arg(vl, int);
				break;

			case 'c':
				os << (unsigned char)va_arg(vl, int);
				break;

			case 'e':
			case 'E':
			case 'f':
			case 'F':
			case 'g':
			case 'G':
				os << va_arg(vl, double);
				break;

			case 's':
				os << va_arg(vl, const char *);
				break;

			case '%':
				os << '%';
				break;

			default:
#ifndef NDEBUG
				std::cerr << "Error in strprintf(): unknown format specifier " << *ptr << std::endl;
				exit(1);
#endif
				break;
		}
	}

	va_end(vl);
	return os.str();
}

// Returns an option from the given map or a default value
std::string defaultOption(const std::map<std::string, std::string> &options, const std::string name, const std::string &def)
{
	std::map<std::string, std::string>::const_iterator it = options.find(name);
	if (it != options.end()) {
		return (*it).second;
	} else {
		return def;
	}
}

// Returns an option from the given map or a default value
int32_t defaultOption(const std::map<std::string, std::string> &options, const std::string name, int32_t def)
{
	int32_t i = 0;
	if (str2int(defaultOption(options, name, int2str(def)), &i)) {
		return i;
	} else {
		return def;
	}
}

} // namespace Utils
