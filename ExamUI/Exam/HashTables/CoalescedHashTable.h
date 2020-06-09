#pragma once

#include "HashTable.h"
#include <functional>
#include <vector>

template <typename KeyT, typename ValueT>
class CoalescedHashTable : public HashTable<KeyT, ValueT> {
private:
	struct Cell {
		std::pair<KeyT, ValueT> item;
		size_t index, depth;
		Cell* prev, next;
		size_t bucket_num;

		Cell(std::pair<KeyT, ValueT> const &item, size_t index, size_t depth, Cell* prev, Cell* next, size_t bucket_num) : 
			item(item), index(index), depth(depth), prev(prev), next(next), bucket_num(bucket_num) {}
	};

	std::vector<Cell*> table;
	size_t _size, buckets_count;
	std::function<size_t(KeyT const&, size_t)> hash, second_hash;
	bool two_choice_enabled;

	size_t getIndex(KeyT const& key) {
		size_t index = hash(key, table.size());
		if (two_choice_enabled) {
			size_t secondary_index = second_hash(key, table.size());
			if (table[secondary_index]) {
				if (!table[index] || table[index]->depth > table[secondary_index]->depth)) {
					index = secondary_index;
				}
			}
		}
		return index;
	}

	size_t findEmptyCell() const {
		for (size_t i = 0; i < table.size(); i++) {
			if (!table[i]) return i;
		}
		return SIZE_MAX;
	}

	size_t findIndex(KeyT const& key, ValueT const& value) {
		size_t index = hash(key, table.size());
		while (table[index]->item.second != value) {
			index = table[index]->next->index;
		}
		return index;
	}

	void decreaseDepthDown(Cell* current) {
		while (current) {
			current->depth--;
			current = current->next;
		}
	}

public:
	CoalescedHashTable(std::function<size_t(KeyT const&, size_t)> _hash, size_t capacity = 1) : hash(_hash), _size(0), two_choice_enabled(false), buckets_count(0) {
		table.resize(capacity, nullptr);
	}

	bool empty() const override {
		return _size == 0;
	}

	size_t size() const override {
		return _size;
	}

	size_t capacity() const override {
		return table.size();
	}

	bool full() const {
		return _size == table.size();
	}

	void clear() override {
		for (Cell* item : table) {
			if (item) delete item;
		}
		table.clear();
		_size = 0;
		buckets_count = 0;
	}

	bool insert(KeyT const& key, ValueT const& value) override {
		if (this->contains(key, value)) return false;
		size_t index = this->getIndex(key);
		Cell* current = table[index];
		if (current) {
			index = this->findEmptyCell();
			if (index == SIZE_MAX) return false;
			while (current->next) current = current->next;
			auto new_cell = new Cell({ key, value }, index, current->depth + 1, current, nullptr, current->bucket_num);
			current->next = new_cell;
		}
		else {
			buckets_count++;
			table[index] = new Cell({ key, value }, index, 0, nullptr, nullptr, buckets_count);
		}
		_size++;
		return true;
	}

	bool remove(KeyT const& key, ValueT const& value) override {
		if (!this->contains(key, value)) return false;
		size_t index = this->findIndex(key, value);
		Cell* to_remove = table[index];

		if (!to_remove->next) {
			if (to_remove->prev) to_remove->prev->next = nullptr;
			table[index] = nullptr;
			buckets_count--;
		}
		else {
			to_remove->next->prev = to_remove->prev;
			if (to_remove->prev) to_remove->prev->next = to_remove->next;
			else {
				table[to_remove->next->index] = nullptr;
				table[index] = to_remove->next;
				to_remove->next->index = index;
			}
			this->decreaseDepthDown(to_remove);
		}
		delete to_remove;
		_size--;
	}

	bool contains(KeyT const& key, ValueT const& value) const override {
		auto items = this->find(key);
		for (Cell* cell : items) {
			if (cell->item.second == value) return true;
		}
		return false;
	}

	std::vector<ValueT> find(KeyT const& key) const override {
		std::vector<ValueT> result;
		size_t index = hash(key, table.size());
		Cell* current = table[index];
		while (current) {
			if (current->item.first == key) {
				result.push_back(current->item.second);
			}
			current = current->next;
		}

		if (two_choice_enabled) {
			size_t second_index = second_hash(key, table.size());
			if (table[index]->bucket_num == table[second_index]->bucket_num) {
				size_t first_depth = table[index]->depth, second_depth = table[second_index]->depth;
				if (first_depth > second_depth) {
					Cell* current = table[second_index];
					while (second_depth != first_depth) {
						if (current->item.first == key) {
							result.push_back(current->item.second);
						}
						current = current->next;
						second_depth++;
					}
				}
			}
			else {
				Cell* current = table[second_index];
				while (current) {
					if (current->item.first == key) {
						result.push_back(current->item.second);
					}
					current = current->next;
				}
			}
		}

		return result;
	}

	std::vector<std::pair<KeyT, ValueT>> getAllItems() const override {
		std::vector<std::pair<KeyT, ValueT>> result;
		//TODO
		return result;
	}
};