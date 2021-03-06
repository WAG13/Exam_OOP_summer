#pragma once

template<typename T>
struct ForwardIteratorImpl
{
	virtual ~ForwardIteratorImpl() {};
	virtual void increment() = 0;
	virtual bool isEnd() const = 0;
	virtual T& getRef() = 0;
	virtual const T& getRef() const = 0;
	virtual T* getPtr() = 0;
	virtual const T* getPtr() const = 0;
};

template<typename T>
class ForwardIterator
{
	std::unique_ptr<ForwardIteratorImpl<T>> impl;
public:
	ForwardIterator(ForwardIteratorImpl<T>* impl)
		: impl(impl)
	{}
	ForwardIterator(ForwardIterator&& it)
		: impl(it.impl)
	{}
	ForwardIterator& operator=(ForwardIterator& it)
	{
		delete impl;
		impl = it.impl;
		return *this;
	}
	ForwardIterator& operator++(int)
	{
		impl->increment();
		return *this;
	}
	ForwardIterator& operator++()
	{
		impl->increment();
		return *this;
	}
	T& operator*()
	{
		return impl->getRef();
	}
	const T& operator*() const
	{
		return impl->getRef();
	}
	T* operator->()
	{
		return impl->getPtr();
	}
	const T* operator->() const
	{
		return impl->getPtr();
	}
	bool isEnd() const
	{
		return impl->isEnd();
	}
	bool operator==(const ForwardIterator& it2)
	{
		if (this->isEnd())
			return it2.isEnd();
		else if (it2.isEnd())
			return false;

		return impl->getPtr() == it2.impl->getPtr();
	}
	bool operator!=(const ForwardIterator& it2)
	{
		if (this->isEnd())
			return !it2.isEnd();
		else if (it2.isEnd())
			return true;

		return impl->getPtr() != it2.impl->getPtr();
	}
};