#pragma once

#include "HashTable.h"
#include <functional>
#include <vector>

//Some code is commented due to the inefficiency of 2-choice-hashing (it needs a lot more fields to keep info), but it works (except for remove())

template <typename KeyT, typename ValueT>
class CoalescedHashTable : public HashTable<KeyT, ValueT> {
private:
	struct Cell {
		std::pair<KeyT, ValueT> item;
		size_t index;
		//size_t depth;
		Cell* prev, *next;
		//size_t bucket_num;

		Cell(std::pair<KeyT, ValueT> const &item, size_t index, Cell* prev, Cell* next) : 
			item(item), index(index), prev(prev), next(next) {}

		/*void increaseDepthDown() {
			Cell* current = this;
			while (current) {
				current->depth++;
				current = current->next;
			}
		}

		void decreaseDepthDown() {
			Cell* current = this;
			while (current) {
				current->depth--;
				current = current->next;
			}
		}*/

		void unlink() {
			if (prev) {
				prev->next = next;
			}
			if (next) {
				next->prev = prev;
			}
			//next->decreaseDepthDown();
		}

		/*void link_next_to(Cell* current) {
			this->prev = current;
			this->next = current->next;
			if (current->next) {
				current->next->prev = this;
			}
			current->next = this;
			this->depth = prev->depth;
			if (next) next->increaseDepthDown();
		}*/
	};

	std::vector<Cell*> table;
	size_t _size;
	//size_t buckets_count;
	std::function<size_t(KeyT const&, size_t)> hash, second_hash;
	bool two_choice_enabled;

	size_t getIndex(KeyT const& key) {
		size_t index = hash(key, table.size());
		/*if (two_choice_enabled) {
			size_t secondary_index = second_hash(key, table.size());
			if (table[secondary_index]) {
				if (!table[index] || table[index]->depth > table[secondary_index]->depth) {
					index = secondary_index;
				}
			}
		}*/
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

public:
	CoalescedHashTable(std::function<size_t(KeyT const&, size_t)> _hash, size_t capacity = 1) : 
		hash(_hash), _size(0), two_choice_enabled(false) {
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
		//buckets_count = 0;
	}

	bool insert(KeyT const& key, ValueT const& value) override {
		if (this->contains(key, value)) return false;
		size_t index = this->getIndex(key);
		Cell* current = table[index];

		if (current) {
			index = this->findEmptyCell();
			if (index == SIZE_MAX) return false;
			while (current->next) current = current->next;
			auto new_cell = new Cell({ key, value }, index, current, nullptr);
			current->next = new_cell;
			table[index] = new_cell;
		}
		else {
			table[index] = new Cell({ key, value }, index, nullptr, nullptr);
		}

		_size++;
		return true;
	}

	bool remove(KeyT const& key, ValueT const& value) override {
		if (!this->contains(key, value)) return false;
		size_t index = this->findIndex(key, value);
		Cell* to_remove = table[index];
		table[index] = nullptr;
		to_remove->unlink();
		delete to_remove;
		_size--;
		this->rehash(0);
	}

	bool remove(KeyT const& key) override {
		auto values_to_remove = this->find(key);
		if (values_to_remove.empty()) return false;
		for (ValueT const& value : values_to_remove) {
			this->remove(key, value);
		}
		return true;
	}

	bool contains(KeyT const& key, ValueT const& value) const override {
		auto values = this->find(key);
		for (ValueT const& found_value : values) {
			if (found_value == value) return true;
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

		/*if (two_choice_enabled) {
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
		}*/

		return result;
	}

	std::vector<std::pair<KeyT, ValueT>> getAllItems() const override {
		std::vector<std::pair<KeyT, ValueT>> result;
		for (Cell* cell : table) {
			if (cell) {
				result.push_back(cell->item);
			}
		}
		return result;
	}

	void enableTwoChoiceHashing(std::function<size_t(KeyT const&, size_t)> _second_hash) override {
		second_hash = _second_hash;
		two_choice_enabled = true;
	}

	void disableTwoChoiceHashing() override {
		two_choice_enabled = false;
	}

	void rehash(size_t add_count) override {
		size_t new_capacity = table.size() + add_count;
		auto items = this->getAllItems();
		this->clear();
		table.resize(new_capacity);
		for (auto const& item : items) {
			this->insert(item.first, item.second);
		}
	}

	~CoalescedHashTable() {
		this->clear();
	}
};