#pragma once

#include "PivotingStrategy.h"
#include "../Trees/SearchTree.h"
#include "../Trees/AVLTree.h"
#include <functional>
#include <stdexcept>
#include <vector>
using std::vector;

template <typename T>
class Sorting
{
protected:
	bool (*comparator)(T const &, T const &);

public:
	Sorting()
	{
		comparator = [](T const &left, T const &right) { return left < right; };
	}

	Sorting(const Sorting<T> *source)
	{
		comparator = source->comparator;
	}

	void setComparator(bool (*_comparator)(T const &, T const &))
	{
		if (_comparator(T(), T()))
		{
			throw std::invalid_argument("invalid comparator.");
		}
		comparator = _comparator;
	}

	virtual void sort(vector<T> &sample, size_t first, size_t last) const = 0;

	virtual Sorting<T> *clone() const = 0;

	virtual ~Sorting() {}
};

template <typename T>
class BubbleSort : public Sorting<T>
{
public:
	BubbleSort() {}

	BubbleSort(const BubbleSort<T> *source) : Sorting<T>(source) {}

	void sort(vector<T> &sample, size_t first, size_t last) const override
	{
		for (size_t i = last - first + 1; i > 0; i--)
		{
			size_t border = first + i - 1;
			for (size_t j = first; j < border; j++)
			{
				if (!Sorting<T>::comparator(sample[j], sample[j + 1]))
				{
					std::swap(sample[j], sample[j + 1]);
				}
			}
		}
	}

	Sorting<T> *clone() const override
	{
		return new BubbleSort<T>(this);
	}

	~BubbleSort() override {}
};

template <typename T>
class SelectionSort : public Sorting<T>
{
public:
	SelectionSort() {}

	SelectionSort(const SelectionSort<T> *source) : Sorting<T>(source) {}

	void sort(vector<T> &sample, size_t first, size_t last) const override
	{
		size_t min_index;

		for (size_t i = first; i <= last; i++)
		{
			min_index = i;
			for (size_t j = i + 1; j <= last; j++)
			{
				if (!Sorting<T>::comparator(sample[min_index], sample[j]))
				{
					min_index = j;
				}
			}
			std::swap(sample[i], sample[min_index]);
		}
	}

	Sorting<T> *clone() const override
	{
		return new SelectionSort<T>(this);
	}

	~SelectionSort() override {}
};

template <typename T>
class InsertionSort : public Sorting<T>
{
public:
	InsertionSort() {}

	InsertionSort(const InsertionSort<T> *source) : Sorting<T>(source) {}

	void sort(vector<T> &sample, size_t first, size_t last) const override
	{
		for (size_t i = first; i < last; i++)
		{
			for (size_t j = i + 1; j > first; j--)
			{
				if (!Sorting<T>::comparator(sample[j - 1], sample[j]))
				{
					std::swap(sample[j - 1], sample[j]);
				}
				else
					break;
			}
		}
	}

	Sorting<T> *clone() const override
	{
		return new InsertionSort<T>(this);
	}

	~InsertionSort() override {}
};

template <typename T>
class QuickSort : public Sorting<T>
{
private:
	PivotingStrategy<T> *pivoting_strategy;

	void partition(vector<T> &sample, int &left, int &right) const
	{
		T pivot = pivoting_strategy->getPivot(sample, left, right);

		while (left <= right)
		{
			while (Sorting<T>::comparator(sample[left], pivot))
				left++;
			while (Sorting<T>::comparator(pivot, sample[right]))
				right--;
			if (left <= right)
			{
				std::swap(sample[left], sample[right]);
				left++;
				right--;
			}
		}
	}

	void sortRecursive(vector<T> &sample, int left, int right) const
	{
		int i = left, j = right;
		partition(sample, i, j);

		if (j > left)
			sortRecursive(sample, left, j);
		if (i < right)
			sortRecursive(sample, i, right);
	}

public:
	QuickSort(PivotingStrategy<T> *_pivoting_strategy)
		: pivoting_strategy(_pivoting_strategy)
	{
		if (!_pivoting_strategy)
		{
			pivoting_strategy = new MiddlePivot<T>();
		}
	}

	QuickSort(const QuickSort<T> *source) : Sorting<T>(source)
	{
		pivoting_strategy = source->pivoting_strategy;
	}

	void sort(vector<T> &sample, size_t first, size_t last) const override
	{
		if (sample.empty())
			return;
		sortRecursive(sample, first, last);
	}

	Sorting<T> *clone() const override
	{
		return new QuickSort<T>(this);
	}

	~QuickSort() override {
		delete pivoting_strategy;
	}
};

template <typename T>
class MergeSort : public Sorting<T>
{
private:
	void merge(vector<T> &sample, size_t left, size_t mid, size_t right) const
	{
		size_t size = right - left + 1, i = 0, a = left, b = mid;
		vector<T> temp(size);

		while (a < mid && b <= right)
		{
			if (Sorting<T>::comparator(sample[a], sample[b]))
			{
				temp[i] = sample[a];
				a++;
			}
			else
			{
				temp[i] = sample[b];
				b++;
			}
			i++;
		}

		while (a < mid)
		{
			temp[i] = sample[a];
			i++;
			a++;
		}

		while (b <= right)
		{
			temp[i] = sample[b];
			i++;
			b++;
		}

		i = 0;
		for (; i < size; i++)
		{
			sample[left + i] = temp[i];
		}
	}

	void sortRecursive(vector<T> &sample, size_t left, size_t right) const
	{
		size_t mid = (left + right) / 2;
		if (mid > left)
			sortRecursive(sample, left, mid);
		if (right > mid + 1)
			sortRecursive(sample, mid + 1, right);
		merge(sample, left, mid + 1, right);
	}

public:
	MergeSort() {}

	MergeSort(const MergeSort<T> *source) : Sorting<T>(source) {}

	void sort(vector<T> &sample, size_t first, size_t last) const override
	{
		sortRecursive(sample, first, last);
	}

	Sorting<T> *clone() const override
	{
		return new MergeSort(this);
	}

	~MergeSort() override {}
};

template <typename T>
class TreeSort : public Sorting<T>
{
public:
	TreeSort() {}

	TreeSort(const TreeSort<T>* source) : Sorting<T>(source) {}

	void sort(vector<T>& sample, size_t first, size_t last) const override
	{
		std::function<bool(T const&, T const&)> comparator_bind = [=](T const& a, T const& b) {
			return Sorting<T>::comparator(a, b);
		};
		lists::AVLTree<T, T>* treePtr = new lists::AVLTree<T, T>(lists::detail::getValueAsKey<T>, comparator_bind);
		std::unique_ptr<lists::SearchTree<T, T>> tree(treePtr);
		for (size_t i = first; i <= last; i++) {
			tree.get()->add(sample[i]);
		}
		std::vector<T> result;
		tree->forEach([&result](const T& elem) {
			result.push_back(elem);
			});
		for (size_t i = first; i <= last; i++) {
			sample[i] = result[i - first];
		}
	}

	Sorting<T>* clone() const override
	{
		return new TreeSort<T>(this);
	}

	~TreeSort() override {}
};

template <typename T>
class HeapSort : public Sorting<T>
{
private:
	void heapify(std::vector<T>& sample, size_t size, size_t first, size_t index) const
	{
		size_t largest = index;
		size_t left = 2 * largest + 1, right = 2 * largest + 2;

		if (left < size && !Sorting<T>::comparator(sample[first + left], sample[first + largest])) {
			largest = left;
		}

		if (right < size && !Sorting<T>::comparator(sample[first + right], sample[first + largest])) {
			largest = right;
		}

		if (largest != index)
		{
			std::swap(sample[first + index], sample[first + largest]);
			heapify(sample, size, first, largest);
		}
	}

public:
	HeapSort() {}

	HeapSort(const HeapSort<T>* source) : Sorting<T>(source) {}

	void sort(vector<T>& sample, size_t first, size_t last) const override
	{
		size_t size = last - first + 1;
		for (size_t i = size / 2 - 1; i != SIZE_MAX; i--) {
			heapify(sample, size, first, i);
		}

		for (size_t i = size - 1; i > 0; i--)
		{
			std::swap(sample[first], sample[first + i]);
			heapify(sample, i, first, 0);
		}
	}

	Sorting<T>* clone() const override
	{
		return new HeapSort<T>(this);
	}

	~HeapSort() override {}
};