/*
 * pgpry alpha - PGP private key password recovery
 * Copyright (C) 2010 Jonas Gehring
 *
 * file: attack.h
 * Static attack context
 */


#ifndef ATTACK_H_
#define ATTACK_H_


#include "main.h"

#include <vector>

#include "key.h"
#include "memblock.h"
#include "threads.h"

class Buffer;
class Options;
class RegexFilter;
namespace Crackers {
	class Cracker;
}
namespace Guessers {
	class Guesser;
}


class Attack
{
	public:
		static int32_t run(const Key &key, const Options &options);

		static void phraseFound(const Memblock &mblock);
		static bool successful();

	private:
		static std::vector<Guessers::Guesser *> setupGuessers(Buffer *out, const Options &options);
        static std::vector<RegexFilter *> setupRegexFilters(Buffer *in, Buffer *out, const Options &options);
		static std::vector<Crackers::Cracker *> setupCrackers(const Key &key, Buffer *in, const Options &options);

	private:
		static Key m_key;
		static Memblock m_phrase;
		static std::vector<Guessers::Guesser *> m_guessers;
        static std::vector<RegexFilter *> m_regexFilters;
		static std::vector<Crackers::Cracker *> m_crackers;
		static bool m_success;
		static SysUtils::Mutex m_mutex;
		static SysUtils::WaitCondition m_condition;
};


// Inlined functions
inline bool Attack::successful()
{
	bool success;
	Attack::m_mutex.lock();
	success = Attack::m_success;
	Attack::m_mutex.unlock();
	return success;
}


#endif // ATTACK_H_
