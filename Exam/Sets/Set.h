#ifndef SET_H
#define SET_H

#include <vector>
#include <memory>
#include <utility>

template<typename T>
class Set
{
public:
	virtual void insert(const T& element) = 0;
	virtual void remove(const T& element) = 0;
	virtual bool contains(const T& element) const = 0;

	//TODO: set operatrions (merge, diff, ...)

private:
	class IteratorImpl
	{
		virtual ~IteratorImpl() {};
		virtual void increment() = 0;
		virtual bool isEnd() = 0;
		virtual T& getRef() = 0;
		virtual T* getPtr() = 0;
		virtual bool operator==(const IteratorImpl& impl2) = 0;
		virtual bool operator!=(const IteratorImpl& impl2) = 0;
	};

public:
	class Iterator
	{
		Set* set;
		std::unique_ptr<IteratorImpl> impl;
	public:
		Iterator(Set* set, IteratorImpl* impl)
			: set(set), impl(impl)
		{}
		void operator==(const Iterator& it2)
		{
			return *impl == *(it2->impl)
		}
		Iterator& operator++()
		{
			impl->increment();
			return *this;
		}
		T& operator*()
		{
			return impl->getRef();
		}
		T* operator->()
		{
			return impl->getPtr();
		}
		bool isEnd()
		{
			return impl->isEnd();
		}
	};

	virtual Iterator begin() = 0;
	virtual Iterator end() = 0;

};


#endif
