#pragma once

template<typename T>
struct ForwardIteratorImpl
{
	virtual ~ForwardIteratorImpl() {};
	virtual void increment() = 0;
	virtual bool isEnd() = 0;
	virtual T& getRef() = 0;
	virtual T* getPtr() = 0;
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
	T* operator->()
	{
		return impl->getPtr();
	}
	bool isEnd()
	{
		return impl->isEnd();
	}
};