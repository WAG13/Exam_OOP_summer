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
		virtual void printAll(std::ostream& os) const = 0;
		virtual bool remove(const Key& key) = 0;
		virtual bool contains(const Key& key) const = 0;
		//virtual std::vector<T> find_all(const Key& min, const Key& max) const = 0;
		virtual void forEach(std::function<void(const T&)> func) const = 0;
	};


	namespace detail
	{
		template<typename T>
		T getValueAsKey(const T& value)
		{
			return value;
		}
	}

	template<typename T>
	using SearchTreeSimple = SearchTree<T, T, detail::getValueAsKey<T>>;
}

#endif // SORTEDLIST_H
