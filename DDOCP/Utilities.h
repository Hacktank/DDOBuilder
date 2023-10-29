#pragma once

#include <vector>

namespace Utilities {

	// DOES NOT PRESERVE ORDER OF ELEMENTS!!
	template<typename ContainerType>
	void vectorQuickErase(ContainerType& container, size_t index)
	{
		container[index] = std::move(container.back());
		container.pop_back();
	}

	template<typename ContainerType>
	typename ContainerType::iterator vectorQuickErase(ContainerType& container, const typename ContainerType::iterator it)
	{
		const size_t index = it - container.begin();
		vectorQuickErase(container, index);
		return container.begin() + index;
	}
}