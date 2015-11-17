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
	return GetNumberOfPermutationsInternalIterative(items);
}

BigUnsigned Flaschenzug::Flaschenzug::GetNumberOfPermutationsInternalNaiveRecursive(uint32_t items, uint32_t containerIndex)
{
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
			result += GetNumberOfPermutationsInternalNaiveRecursive(items - i, containerIndex + 1);
	}
	return result;
}

BigUnsigned Flaschenzug::Flaschenzug::GetNumberOfPermutationsInternalRecursive(uint32_t items, uint32_t containerIndex)
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
				result += GetNumberOfPermutationsInternalRecursive(items - i, containerIndex + 1);
		}
		memoMap[key] = result;
		return result;
	}
	return memoEntry->second;
}

BigUnsigned Flaschenzug::Flaschenzug::GetNumberOfPermutationsInternalIterative(uint32_t items)
{
	uint32_t numContainers = containers.size();
	std::vector<uint32_t> combinedRestContainerCapacities(numContainers);
	std::partial_sum(containers.rbegin(), containers.rend(), combinedRestContainerCapacities.begin());
	std::vector<BigUnsigned> results(numContainers * (items + 1));
	for (int i = 0; i < numContainers * (items + 1); i++)
	{
		uint32_t containerIndex = numContainers - (i % numContainers) - 1;
		uint32_t currentItems = i / numContainers;

		switch (containers.size() - containerIndex)
		{
		case 1: //last container
			results[i] = 1U; break;
		case 2: //two containers left
		{
			uint32_t a = containers[containerIndex], b = containers[containerIndex + 1];
			uint32_t options[] = { a, b, currentItems, a + b - currentItems };
			results[i] = BigUnsigned(*std::min_element(options, options + 4) + 1U);
		} break;
		default: //any other number of containers
			int combinedRestContainerCapacity = combinedRestContainerCapacities[containers.size() - containerIndex - 2];
			for (int j = std::max(0, (int) currentItems - combinedRestContainerCapacity); j <= containers[containerIndex] && j <= currentItems; j++)
				results[i] += results[(currentItems - j) * numContainers + (i % numContainers - 1)];
		}
	}
	return results[numContainers * (items + 1) - 1];
}
