#ifndef SORTEDLIST_H
#define SORTEDLIST_H

#include <iostream>
#include <functional>
#include <vector>

namespace lists
{
	template<typename T, typename Key>
	using GetKeyFuncType = Key(*)(const T&);

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	struct SearchTree
	{
		SearchTree() {}
		virtual ~SearchTree() {}

		virtual void add(const T& element) = 0;
		virtual void print_all(std::ostream& os) const = 0;
		virtual bool remove(const Key& key) = 0;
		virtual bool contains(const Key& key) const = 0;
		virtual std::vector<T> find_all(const Key& min, const Key& max) const = 0;
		virtual void for_each(std::function<void(const T&)> func) const = 0;
	};
}

#endif // SORTEDLIST_H
