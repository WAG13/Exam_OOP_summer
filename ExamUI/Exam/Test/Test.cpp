#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "../Data/DateTime.h"
#include "../Data/DataGenerator.h"
#include "../Lists/DoublyCircularLinkedList.h"
#include "../Lists/DoublyLinkedList.h"
#include "../HashTables/BucketHashTable.h"
#include "../HashTables/CoalescedHashTable.h"
#include "../Trees/AVLTree.h"
#include "../Trees/BPlusTree.h"
#include "../Maps/TreeMap.h"
#include "../Maps/StandardMap.h"
#include "../Maps/ListMap.h"
#include "../Sets/HashSet.h"
#include "../Sets/ListSet.h"
#include "../Sets/TreeSet.h"
#include "../Sets/VectorSet.h"
#include "../SortingAlgorithms/SortingAlgorithms.h"
#include "../Iterator.h"
#include <memory>
#include <vector>

TEST_CASE("Data")
{
    SUBCASE("Creating data")
    {
        DateTime date1(1800, 12, 1, 0, 15, 0);
        REQUIRE(date1.toString() == "01.12.1800 00:15:00");
        DateTime date2(2015, 14, 18, 12, 65, 8);
        REQUIRE(date2.toString() == "18.02.2016 13:05:08");
        DateTime date3(2015, 12, 31, 25, 65, 8);
        REQUIRE(date3.toString() == "01.01.2016 02:05:08");

    }
    SUBCASE("Creating random data")
    {
        DateTime min_date(1800, 1, 1, 0, 0, 0);
        REQUIRE(min_date.toString() == "01.01.1800 00:00:00");
        DateTime max_date(2500, 1, 1, 0, 0, 0);
        REQUIRE(max_date.toString() == "01.01.2500 00:00:00");
        DataGenerator<DateTime>* dataGen = new RandomDataGenerator<DateTime>();
        vector<DateTime> random = dataGen->generateVector(min_date, max_date, 150);
        REQUIRE(random.size() == 150);
        for (auto& r : random) {
            REQUIRE(r <= max_date);
            REQUIRE(r >= min_date);
        }
    }
}

int foo(int element_value) { return (element_value + 10); };

TEST_CASE("List")
{
    SUBCASE("Creating Doubly Linked List")
    {
        int size = 150;
        DoublyLinkedList<int, int> linked_list(&foo);
        for (int i = 0; i < size; i++)
            linked_list.insertNewNode(i);

        auto vector_linked_list = linked_list.toVector();
        for (int i = 0; i < size; i++) {
            REQUIRE(vector_linked_list[i].second == i);
            REQUIRE(vector_linked_list[i].second + 10 == vector_linked_list[i].first);
        }

        REQUIRE(!linked_list.searchByKey(5));
        linked_list.deleteByKey(15);
        REQUIRE(!linked_list.searchByValue(5));

        REQUIRE(linked_list.searchByValue(15));
        linked_list.deleteByValue(15);
        REQUIRE(!linked_list.searchByValue(15));
        linked_list.deleteByValue(-15);
        REQUIRE(!linked_list.searchByValue(-15));
    }

    SUBCASE("Creating Doubly Circular Linked List")
    {
        int size = 150;
        DoublyCircularLinkedList<int, int> linked_list(&foo);
        for (int i = 0; i < size; i++)
            linked_list.insertNewNode(i);

        auto vector_linked_list = linked_list.toVector();
        for (int i = 0; i < size; i++) {
			REQUIRE(vector_linked_list[i].second == i);
			REQUIRE(vector_linked_list[i].second + 10 == vector_linked_list[i].first);
        }

        REQUIRE(!linked_list.searchByKey(5));
        linked_list.deleteByKey(15);
        REQUIRE(!linked_list.searchByValue(5));
        linked_list.deleteByKey(-5);
        REQUIRE(!linked_list.searchByKey(-5));

        REQUIRE(linked_list.searchByValue(15));
        linked_list.deleteByValue(15);
        REQUIRE(!linked_list.searchByValue(15));
        linked_list.deleteByValue(-15);
        REQUIRE(!linked_list.searchByValue(-15));
    }
}

TEST_CASE("Hash")
{
    SUBCASE("Separate hashing") {
        auto hash = new BucketHashTable<int, int, std::vector<std::pair<int, int>>> { [](int const& v, size_t index, size_t bucket_count) { return size_t(v) % bucket_count; }, 7 };
        REQUIRE(hash->capacity() == 7);

        hash->insert(2, 13);
        hash->insert(2, 15);
        hash->insert(1, 2);
        hash->insert(14, 5);
        REQUIRE(hash->find(14) == std::vector{ 5 });
        REQUIRE(hash->find(1) == std::vector{ 2 });
        REQUIRE(hash->find(2) == std::vector{13, 15});
        REQUIRE(hash->find(3) == std::vector<int>{});
        REQUIRE(hash->find(6) == std::vector<int>{});

        hash->remove(2, 13);
        REQUIRE(hash->find(2) == std::vector{ 15 });
        hash->remove(2);
        REQUIRE(hash->find(2) == std::vector<int>{});
        REQUIRE(hash->size() == 2);

        hash->enableTwoChoiceHashing([](int const& v, size_t index, size_t bucket_count) { return (size_t(v) * size_t(v)) % bucket_count; });
        hash->insert(2, 8);
        hash->insert(2, 9);
        hash->insert(2, 10);
        //first algorithm searches in the 2-nd bucket, then in the 4-th bucket
        REQUIRE(hash->find(2) == std::vector{ 8, 10, 9 });

        REQUIRE(hash->getAllItems() == std::vector<std::pair<int, int>>{ { 14, 5 }, { 1, 2 }, { 2, 8 }, { 2, 10 }, { 2, 9 } });
        //implicitly rehashes hash table
        hash->disableTwoChoiceHashing();
        REQUIRE(hash->getAllItems() == std::vector<std::pair<int, int>>{ { 14, 5 }, { 1, 2 }, { 2, 8 }, { 2, 10 }, { 2, 9 } });
        //now there are 11 buckets
        hash->rehash(4);
        //key 14 moves to the 3-rd bucket
        REQUIRE(hash->getAllItems() == std::vector<std::pair<int, int>>{ { 1, 2 }, { 2, 8 }, { 2, 10 }, { 2, 9 }, { 14, 5 } });

		delete hash;
    }

    SUBCASE("Coalesced hashing")
    {
		auto hash = new CoalescedHashTable<int, int>{ [](int const& v, size_t index, size_t bucket_count) { return size_t(v) % bucket_count; }, 7 };
		REQUIRE(hash->capacity() == 7);

		hash->insert(2, 13);
		hash->insert(2, 15);
		hash->insert(1, 2);
		hash->insert(14, 5);

		REQUIRE(hash->find(14) == std::vector{ 5 });
		REQUIRE(hash->find(1) == std::vector{ 2 });
		REQUIRE(hash->find(2) == std::vector{ 13, 15 });
		REQUIRE(hash->find(3) == std::vector<int>{});
		REQUIRE(hash->find(6) == std::vector<int>{});

		//items that are placed to a full cell then go to the first free cell and are placed there (and are added to the corresponding list)
		REQUIRE(hash->getAllItems() == std::vector<std::pair<int, int>>{ { 2, 15 }, { 1, 2 }, { 2, 13 }, { 14, 5 } });

		//the table rehashes
		hash->remove(2, 13);
		REQUIRE(hash->getAllItems() == std::vector<std::pair<int, int>>{ { 14, 5 }, { 1, 2 }, { 2, 15 } });

		delete hash;
    }

    SUBCASE("Hopscotch hashing")
    {
    }
}

template<typename T>
void testTree(lists::SearchTreeSimple<T>* tree)
{
	tree->add(3);
	tree->add(11);
	tree->add(6);
	tree->add(8);
	REQUIRE(tree->contains(11));
	REQUIRE(tree->contains(8));
	REQUIRE(!tree->contains(12));

	REQUIRE(tree->remove(6));
	REQUIRE(!tree->contains(6));
	REQUIRE(!tree->remove(12));
}

TEST_CASE("Tree")
{
	SUBCASE("AVL tree")
	{
		lists::AVLTreeSimple<int>* treePtr = new lists::AVLTreeSimple<int>();
		std::unique_ptr<lists::SearchTreeSimple<int>> tree(treePtr);
		testTree(tree.get());

		std::vector<int> result = treePtr->find_all(7, 14);
		REQUIRE(result == std::vector<int> { 8, 11 });
	}

	SUBCASE("B+ tree")
	{
		std::unique_ptr<lists::SearchTreeSimple<int>> tree(new lists::BPlusTreeSimple<int>(3));
		testTree(tree.get());
	}
}

TEST_CASE("Sorting")
{

    SUBCASE("Selection Sort")
    {
		std::unique_ptr<Sorting<int>> sorting = std::make_unique<SelectionSort<int>>();
		sorting->setComparator([](int const& left, int const& right) { return left > right; });
		std::vector<int> v{ 45, -5, 4, -7, 7, 7, 9 };
		sorting.get()->sort(v, 0, 6);
		REQUIRE(v == std::vector{ 45, 9, 7, 7, 4, -5, -7 });
    }

    SUBCASE("Heap Sort")
    {
		std::unique_ptr<Sorting<int>> sorting = std::make_unique<HeapSort<int>>();
		sorting->setComparator([](int const& left, int const& right) { return left > right; });
		std::vector<int> v{ 45, -5, 4, -7, 7, 7, 9 };
		sorting.get()->sort(v, 0, 6);
		REQUIRE(v == std::vector{ 45, 9, 7, 7, 4, -5, -7 });
    }

    SUBCASE("Merge Sort")
    {
		std::unique_ptr<Sorting<int>> sorting = std::make_unique<MergeSort<int>>();
		std::vector<int> v{ 45, -5, 4, -7, 7, 7, 9 };
		sorting.get()->sort(v, 0, 6);
		REQUIRE(v == std::vector{ -7, -5, 4, 7, 7, 9, 45 });
    }

    SUBCASE("Bucket Sort")
    {
    }

    SUBCASE("Insertion Sort")
    {
		std::unique_ptr<Sorting<int>> sorting = std::make_unique<InsertionSort<int>>();
		std::vector<int> v{ 45, -5, 4, -7, 7, 7, 9 };
		sorting.get()->sort(v, 0, 6);
		REQUIRE(v == std::vector{ -7, -5, 4, 7, 7, 9, 45 });
    }

    SUBCASE("Quicksort")
    {
		std::unique_ptr<Sorting<int>> sorting = std::make_unique<QuickSort<int>>(new MedianOfThreePivot<int>());
		std::vector<int> v{ 45, -5, 4, -7, 7, 7, 9 };
		sorting.get()->sort(v, 0, 6);
		REQUIRE(v == std::vector{ -7, -5, 4, 7, 7, 9, 45 });
    }

    SUBCASE("Radix Sort")
    {
    }

	SUBCASE("Tree Sort")
	{
		Sorting<int>* sorting = new TreeSort<int>();
		sorting->setComparator([](int const& left, int const& right) { return left > right; });
		std::vector<int> v{ 5, 7, -3, 4, 0, 4 };
		sorting->sort(v, 1, 5);
		REQUIRE(v == std::vector<int>{ 5, 7, 4, 4, 0, -3 });
		delete sorting;
	}
}

template<typename ValueT, typename KeyT>
void testMap(Map<ValueT, KeyT>* map)
{
	map->set(5, "Five");
	REQUIRE(map->get(5) == "Five");

	map->set(5, "New Five");
	REQUIRE(map->get(5) == "New Five");

	map->set(4, "Four");
	REQUIRE(map->contains(4));
	REQUIRE(map->get(4) == "Four");

	REQUIRE(map->getValues().size() == 2);

	map->remove(4);
	REQUIRE(!map->contains(4));

	REQUIRE(map->getValues().size() == 1);

	for (int i = 10; i < 100; i++)
		map->set(i, "Item " + i);
	REQUIRE(map->getValues().size() == 91);

	for (int i = 10; i < 90; i += 2)
		map->remove(i);
	REQUIRE(map->getKVPs().size() == 51);
}

TEST_CASE("Map")
{
	SUBCASE("AVL Tree Map")
	{
		std::unique_ptr<MapTreeType<std::string, int>> treeType(new MapTreeTypeAVL<std::string, int>());
		std::unique_ptr<Map<std::string, int>> treeMap(new TreeMap(treeType.get()));
		testMap(treeMap.get());
	}

	SUBCASE("B+ Tree Map")
	{
		std::unique_ptr<MapTreeType<std::string, int>> treeType(new MapTreeTypeBPlus<std::string, int>(500));
		std::unique_ptr<Map<std::string, int>> treeMap(new TreeMap(treeType.get()));
		testMap(treeMap.get());
	}

	SUBCASE("std::map")
	{
		std::unique_ptr<Map<std::string, int>> treeMap(new StandardMap<std::string, int>());
		testMap(treeMap.get());
	}

	SUBCASE("Doubly Linked List Map")
	{
		std::unique_ptr<MapListType<std::string, int>> listType(new MapListTypeDouble<std::string, int>());
		std::unique_ptr<Map<std::string, int>> listMap(new ListMap(listType.get()));
		testMap(listMap.get());
	}

	SUBCASE("Doubly Linked Circular List Map")
	{
		std::unique_ptr<MapListType<std::string, int>> listType(new MapListTypeDoubleCircular<std::string, int>());
		std::unique_ptr<Map<std::string, int>> listMap(new ListMap(listType.get()));
		testMap(listMap.get());
	}
}


template<typename T>
void testSet(Set<T>* set)
{
	set->insert(5);
	set->insert(10);
	auto it = set->begin();
	bool firstIsFive = (*it == 5);
	//REQUIRE(firstIsFive || *it == 10);
	it++;
	//REQUIRE((firstIsFive && *it == 5) || (!firstIsFive && *it == 10));
	it++;
	REQUIRE(it.isEnd());

	set->insert(16);
	REQUIRE(set->contains(16));
	set->remove(16);
	REQUIRE(!set->contains(16));
}

TEST_CASE("Set")
{
	SUBCASE("Doubly Linked List Set")
	{
		std::unique_ptr<SetListType<int>> listType(new SetListTypeDouble<int>());
		std::unique_ptr<Set<int>> listMap(new ListSet(listType.get()));
		testSet(listMap.get());
	}

	SUBCASE("Doubly Linked Circular List Set")
	{
		std::unique_ptr<SetListType<int>> listType(new SetListTypeDoubleCircular<int>());
		std::unique_ptr<Set<int>> listMap(new ListSet(listType.get()));
		testSet(listMap.get());
	}

	SUBCASE("AVL Tree Set")
	{
		std::unique_ptr<SetTreeType<int>> treeType(new SetTreeTypeAVL<int>());
		std::unique_ptr<Set<int>> treeSet(new TreeSet(treeType.get()));
		testSet(treeSet.get());
	}

	SUBCASE("B+ Tree Set")
	{
		std::unique_ptr<SetTreeType<int>> treeType(new SetTreeTypeBPlus<int>(2));
		std::unique_ptr<Set<int>> treeSet(new TreeSet(treeType.get()));
		testSet(treeSet.get());
	}

	SUBCASE("Vector Set")
	{
		std::unique_ptr<Set<int>> vectorSet(new VectorSet<int>());
		testSet(vectorSet.get());
	}

	SUBCASE("Set Operations")
	{
		std::unique_ptr<SetTreeType<int>> treeType(new SetTreeTypeAVL<int>());
		std::unique_ptr<Set<int>> treeSet1(new TreeSet(treeType.get()));
		treeSet1->insert(1);
		treeSet1->insert(2);
		treeSet1->insert(3);
		treeSet1->insert(4);
		std::unique_ptr<Set<int>> treeSet2(new TreeSet(treeType.get()));
		treeSet2->insert(3);
		treeSet2->insert(4);
		treeSet2->insert(5);
		treeSet2->insert(6);

		std::unique_ptr<Set<int>> treeSetUnion(new TreeSet(treeType.get()));
		treeSetUnion->insertUnion(treeSet1.get(), treeSet2.get());
		for (int i = 1; i <= 6; i++)
			REQUIRE(treeSetUnion->contains(i));

		std::unique_ptr<Set<int>> treeSetIntersection(new TreeSet(treeType.get()));
		treeSetIntersection->insertIntersection(treeSet1.get(), treeSet2.get());
		REQUIRE(!treeSetIntersection->contains(1));
		REQUIRE(!treeSetIntersection->contains(2));
		REQUIRE(treeSetIntersection->contains(3));
		REQUIRE(treeSetIntersection->contains(4));
		REQUIRE(!treeSetIntersection->contains(5));
		REQUIRE(!treeSetIntersection->contains(6));

		std::unique_ptr<Set<int>> treeSetComplement(new TreeSet(treeType.get()));
		treeSetComplement->insertComplement(treeSet1.get(), treeSet2.get());
		REQUIRE(treeSetComplement->contains(1));
		REQUIRE(treeSetComplement->contains(2));
		REQUIRE(!treeSetComplement->contains(3));
		REQUIRE(!treeSetComplement->contains(4));
		REQUIRE(!treeSetComplement->contains(5));
		REQUIRE(!treeSetComplement->contains(6));

		std::unique_ptr<Set<int>> treeSetSymmDiff(new TreeSet(treeType.get()));
		treeSetSymmDiff->insertSymmetricDiff(treeSet1.get(), treeSet2.get());
		REQUIRE(treeSetSymmDiff->contains(1));
		REQUIRE(treeSetSymmDiff->contains(2));
		REQUIRE(!treeSetSymmDiff->contains(3));
		REQUIRE(!treeSetSymmDiff->contains(4));
		REQUIRE(treeSetSymmDiff->contains(5));
		REQUIRE(treeSetSymmDiff->contains(6));
	}

	SUBCASE("Hash Set") {
		HashType<std::string, std::string>* hash1 = 
			new SeparateHash<std::string, std::string, std::vector<std::pair<std::string, std::string>>>{};
		auto set1 = new HashSet<std::string>{ hash1 };

		set1->insert("one");
		set1->insert("two");
		set1->insert("three");
		REQUIRE(set1->contains("one"));
		REQUIRE(set1->contains("two"));
		REQUIRE(set1->contains("three"));
		set1->insert("five");
		set1->remove("one");
		REQUIRE(!set1->contains("one"));

		HashType<std::string, std::string>* hash2 =
			new CoalescedHash<std::string, std::string>{};
		auto set2 = new HashSet<std::string>{ hash2 };

		set2->insert("one");
		set2->insert("four");
		set2->insert("three");

		HashSet<std::string> set3{ hash2 };
		set3.insertUnion(set1, set2);
		REQUIRE(set3.contains("one"));
		REQUIRE(set3.contains("two"));
		REQUIRE(set3.contains("three"));
		REQUIRE(set3.contains("four"));
		REQUIRE(set3.contains("five"));

		HashSet<std::string> set4{ hash1 };
		set4.insertIntersection(set1, set2);
		REQUIRE(set4.contains("three"));
		REQUIRE(!set4.contains("one"));
		REQUIRE(!set4.contains("two"));
		REQUIRE(!set4.contains("four"));
		REQUIRE(!set4.contains("five"));

		HashSet<std::string> set5{ hash2 };
		set5.insertSymmetricDiff(set1, set2);
		REQUIRE(!set5.contains("three"));
		REQUIRE(set5.contains("one"));
		REQUIRE(set5.contains("two"));
		REQUIRE(set5.contains("four"));
		REQUIRE(set5.contains("five"));

		delete hash1;
		delete hash2;
		delete set1;
		delete set2;
	}
}