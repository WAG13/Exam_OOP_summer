#pragma once

#include "HashTable.h"
#include <functional>
#include <initializer_list>
#include <vector>

template <typename KeyT, typename ValueT, typename CollisionContainer>
class BucketHashTable : public HashTable<KeyT, ValueT> {
private:
	std::vector<CollisionContainer> buckets;
	size_t _size;
	std::function<size_t(KeyT const&, size_t)> hash, second_hash;
	bool two_choice_enabled;

	size_t getIndex(KeyT const& key) {
		size_t index = hash(key, buckets.size());
		if (two_choice_enabled) {
			size_t secondary_index = second_hash(key, buckets.size());
			if (buckets[index].size() > buckets[secondary_index].size()) {
				index = secondary_index;
			}
		}
		return index;
	}

public:
	BucketHashTable(std::function<size_t(KeyT const&, size_t, size_t)> _hash, size_t capacity = 1) : 
		hash(std::bind(_hash, std::placeholders::_1, 0, std::placeholders::_2)), _size(0), two_choice_enabled(false) {
		buckets.resize(capacity);
	}

	/*BucketHashTable(size_t buckets_count, std::initializer_list<std::pair<KeyT, ValueT>> init) {
		buckets.resize(buckets_count);
		for (auto const& item : init) {
			this->insert(item.first, item.second);
		}
		_size = init.size();
	}*/

	bool empty() const override {
		return _size == 0;
	}

	size_t size() const override {
		return _size;
	}

	size_t capacity() const override {
		return buckets.size();
	}

	void clear() override {
		buckets.clear();
		_size = 0;
	}

	bool insert(KeyT const& key, ValueT const& value) override {
		if (this->contains(key, value)) return false;
		size_t index = this->getIndex(key);
		buckets[index].push_back({ key, value });
		_size++;
		return true;
	}

	bool remove(KeyT const& key, ValueT const& value) override {
		if (!this->contains(key, value)) return false;

		size_t index = hash(key, buckets.size());
		for (auto iter = buckets[index].begin(); iter != buckets[index].end(); iter++) {
			if (iter->second == value) {
				buckets[index].erase(iter);
				_size--;
				return true;
			}
		}

		if (two_choice_enabled) {
			size_t secondary_index = second_hash(key, buckets.size());
			for (auto iter = buckets[secondary_index].begin(); iter != buckets[secondary_index].end(); iter++) {
				if (iter->second == value) {
					buckets[secondary_index].erase(iter);
					_size--;
					return true;
				}
			}
		}

		return false;
	}

	bool remove(KeyT const& key) {
		size_t index = hash(key, buckets.size());
		size_t num_of_removed;

		num_of_removed = buckets[index].size();
		buckets[index].clear();

		if (two_choice_enabled) {
			size_t secondary_index = second_hash(key, buckets.size());
			num_of_removed += buckets[secondary_index].size();
			buckets[secondary_index].clear();
		}

		if (num_of_removed == 0) return false;
		_size -= num_of_removed;
		return true;
	}

	bool contains(KeyT const& key, ValueT const& value) const {
		auto result = this->find(key);
		for (ValueT const& item : result) {
			if (item == value) return true;
		}
		return false;
	}

	std::vector<ValueT> find(KeyT const& key) const {
		std::vector<ValueT> result;
		size_t index = hash(key, buckets.size());
		for (auto const& item : buckets[index]) {
			if (item.first == key) {
				result.push_back(item.second);
			}
		}

		if (two_choice_enabled) {
			size_t secondary_index = second_hash(key, buckets.size());
			if (index != secondary_index) {
				for (auto const& item : buckets[secondary_index]) {
					if (item.first == key) {
						result.push_back(item.second);
					}
				}
			}
		}

		return result;
	}

	std::vector<std::pair<KeyT, ValueT>> getAllItems() const {
		std::vector<std::pair<KeyT, ValueT>> result;
		for (auto const& bucket : buckets) {
			for (auto const& item : bucket) {
				result.push_back(item);
			}
		}
		return result;
	}

	void enableTwoChoiceHashing(std::function<size_t(KeyT const&, size_t, size_t)> _second_hash) {
		second_hash = std::bind(_second_hash, std::placeholders::_1, 0, std::placeholders::_2);
		two_choice_enabled = true;
	}

	void disableTwoChoiceHashing() {
		two_choice_enabled = false;
		this->rehash(0);
	}

	void rehash(size_t add_count) override {
		size_t new_size = buckets.size() + add_count;
		auto items = this->getAllItems();
		buckets.clear();
		buckets.resize(new_size);
		for (auto const& item : items) {
			this->insert(item.first, item.second);
		}
	}

	~BucketHashTable() {}
};