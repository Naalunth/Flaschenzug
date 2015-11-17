#include "Flaschenzug.h"

#include "bigint/BigIntegerLibrary.hh"

#include <numeric>
#include <algorithm>
#include <cstdint>

Flaschenzug::Flaschenzug::Flaschenzug(const std::vector<uint32_t>& containers)
	:containers{ containers }
{
}

Flaschenzug::Flaschenzug::~Flaschenzug()
{
}

BigUnsigned Flaschenzug::Flaschenzug::GetNumberOfPermutations(uint32_t items)
{
	return GetNumberOfPermutationsInternal(items, 0);
}

BigUnsigned Flaschenzug::Flaschenzug::GetNumberOfPermutationsInternal(uint32_t items, uint32_t containerIndex)
{
	auto key = std::make_pair(items, containerIndex);
	auto& memoEntry = memoMap.find(key);
	if (memoEntry == memoMap.end())
	{ //if we haven't calculated these values yet
		BigUnsigned result = 0;
		switch (containers.size() - containerIndex)
		{
		case 1: //last container
			result = 1; break;
		case 2: //two containers left
		{
			uint32_t a = containers[containerIndex], b = containers[containerIndex + 1];
			uint32_t options[] = { a, b, items, a + b - items };
			result = BigUnsigned(*std::min_element(options, options + 4) + 1U);
		} break;
		default: //any other number of containers
			int combinedRestContainerCapacity = std::accumulate(containers.begin() + containerIndex + 1, containers.end(), 0ul);
			for (int i = std::max(0, (int) items - combinedRestContainerCapacity); i <= containers[containerIndex] && i <= items; i++)
				result += GetNumberOfPermutationsInternal(items - i, containerIndex + 1);
		}
		memoMap[key] = result;
		return result;
	}
	return memoEntry->second;
}
