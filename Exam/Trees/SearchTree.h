#ifndef SORTEDLIST_H
#define SORTEDLIST_H

#include <iostream>
#include <functional>
#include <vector>

namespace lists
{
	template<typename T, typename Key>
	using GetKeyFunc = std::function<Key(T)>;

	template<typename Key>
	using Comparator = std::function<bool(const Key&, const Key&)>;

	template<typename T, typename Key>
	struct SearchTree
	{
		SearchTree(GetKeyFunc<T, Key> getKeyFunc, Comparator<Key> comparatorFunc) {}
		virtual ~SearchTree() {}

		virtual void add(const T& element) = 0;
		virtual void printAll(std::ostream& os) const = 0;
		virtual bool remove(const Key& key) = 0;
		virtual bool contains(const Key& key) = 0;
		virtual T& get(const Key& key) = 0;
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

		template<typename T>
		bool lessThan(const T& value1, const T& value2)
		{
			return value1 < value2;
		}
	}

	template<typename T>
	using SearchTreeSimple = SearchTree<T, T>;
}

#endif // SORTEDLIST_H
