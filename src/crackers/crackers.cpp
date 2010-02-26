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
 * file: crackers.cpp
 * Cracker thread definition and factory
 */


#include <cassert>
#include <cstring>
#include <iostream>

#include "attack.h"
#include "buffer.h"
#include "string2key.h"

#include "sha1cracker.h"

#include "crackers.h"


namespace Crackers
{

// Constructor
Cracker::Cracker(const Key &key, Buffer *buffer)
	: Thread(), m_key(key), m_buffer(buffer)
{

}

// Main thread loop
void Cracker::run()
{
	if (!init()) {
		std::cerr << "Error initializing cracker!" << std::endl;
		return;
	}

	uint32_t n = 0, numBlocks = 0;
	Memblock blocks[8];

	while (true) {
		numBlocks = m_buffer->taken(8, blocks);

		for (uint32_t i = 0; i < numBlocks; i++) {
			if (blocks[i].length > 0 && check(blocks[i].data, blocks[i].length)) {
				Attack::phraseFound(blocks[i]);
			}
		}

		// Avoid constant status querying
		if (++n > 128) {
			switch (Attack::status()) {
				case Attack::STATUS_SUCCESS:
					return;
				case Attack::STATUS_FAILURE:
					if (blocks[0].length == 0) {
						return;
					}
					break;
				default:
					break;
			}
			n = 0;
		}
	}
}

// Cracker initialization routine
bool Cracker::init()
{
	// The default implementation does nothing
	return true;
}


// Returns a cracker for the given key
Cracker *crackerFor(const Key &key, Buffer *buffer)
{
	const String2Key &s2k = key.string2Key();
	switch (s2k.hashAlgorithm())
	{
		case CryptUtils::HASH_SHA1:
			return new SHA1Cracker(key, buffer);
			break;

		default: break;
	}

	return NULL;
}

} // namespace Crackers
