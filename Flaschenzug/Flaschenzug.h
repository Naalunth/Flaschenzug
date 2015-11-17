#pragma once

#include "bigint/BigIntegerLibrary.hh"

#include <vector>
#include <map>
#include <cstdint>

namespace Flaschenzug
{
	class Flaschenzug
	{
	public:
		Flaschenzug(const std::vector<uint32_t>& containers);
		~Flaschenzug();
		BigUnsigned GetNumberOfPermutations(uint32_t items);
	private:
		std::vector<uint32_t> containers;
		BigUnsigned GetNumberOfPermutationsInternal(uint32_t items, uint32_t containerIndex);
		std::map<std::pair<uint32_t, uint32_t>, BigUnsigned> memoMap;
	};
}
