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
 * file: guessers.h
 * Guesser thread definition and factory
 */


#ifndef GUESSERS_H_
#define GUESSERS_H_


#include "main.h"

#include <map>
#include <string>

#include "threads.h"

class Buffer;
class Memblock;


namespace Guessers
{

class Guesser : public SysUtils::Thread
{
	public:
		Guesser(Buffer *buffer);
		virtual ~Guesser() { }

		virtual void setup(const std::map<std::string, std::string> &options);

	protected:
		void run();

		virtual bool init();
		virtual bool guess(Memblock *m) = 0;

	private:
		Buffer *m_buffer;
};


Guesser *guesser(const std::string &name, Buffer *buffer);

} // namespace Guessers


#endif // GUESSERS_H_
