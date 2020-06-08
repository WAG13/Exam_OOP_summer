#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include"../Data/DateTime.h"
#include"../Data/DataGenerator.h"
#include "../HashTables/BucketHashTable.h"
#include "../Trees/AVLTree.h"
#include "../Trees/BPlusTree.h"
#include "../Maps/TreeMap.h"
#include <memory>
#include <vector>

TEST_CASE("Data")
{
    SUBCASE("Creating data")
    {
        DateTime date1(1800, 12, 1, 0, 15, 0);
        REQUIRE(date1.getDateTime() == "01.12.1800 00:15:00");
        DateTime date2(2015, 14, 18, 12, 65, 8);
        REQUIRE(date2.getDateTime() == "18.02.2016 13:05:08");
        DateTime date3(2015, 12, 31, 25, 65, 8);
        REQUIRE(date3.getDateTime() == "01.01.2016 02:05:08");
        
    }
    SUBCASE("Creating random data")
    {
        DateTime min_date(1800, 1, 1, 0, 0, 0);
        REQUIRE(min_date.getDateTime() == "01.01.1800 00:00:00");
        DateTime max_date(2500, 1, 1, 0, 0, 0);
        REQUIRE(max_date.getDateTime() == "01.01.2500 00:00:00");
        DataGenerator<DateTime>* dataGen = new RandomDataGenerator<DateTime>();
        vector<DateTime> random = dataGen->generateVector(min_date, max_date, 15);
        REQUIRE(random.size() == 15);
        for (auto& r : random) {
            REQUIRE(r <= max_date);
            REQUIRE(r >= min_date);
        }
    }
}

TEST_CASE("List")
{
    SUBCASE("Creating DList")
    {
    }

    SUBCASE("Creating СDList")
    {
    }
}

TEST_CASE("Hash")
{
    //SUBCASE("Separate hashing") {
    //    auto hash = new BucketHashTable<int, int, [](int const& v, size_t bucket_count) { return size_t(v) % bucket_count; }, std::vector<std::pair<int, int>>> {7};
    //    REQUIRE(hash->buckets_count() == 7);

    //    hash->insert(2, 13);
    //    hash->insert(2, 15);
    //    hash->insert(1, 2);
    //    hash->insert(14, 5);
    //    REQUIRE(hash->find(14) == std::vector{ 5 });
    //    REQUIRE(hash->find(1) == std::vector{ 2 });
    //    REQUIRE(hash->find(2) == std::vector{13, 15});
    //    REQUIRE(hash->find(3) == std::vector<int>{});
    //    REQUIRE(hash->find(6) == std::vector<int>{});

    //    hash->remove(2, 13);
    //    REQUIRE(hash->find(2) == std::vector{ 15 });
    //    hash->remove(2);
    //    REQUIRE(hash->find(2) == std::vector<int>{});
    //    REQUIRE(hash->size() == 2);

    //    hash->enableTwoChoiceHashing([](int const& v, size_t bucket_count) { return (size_t(v) * size_t(v)) % bucket_count; });
    //    hash->insert(2, 8);
    //    hash->insert(2, 9);
    //    hash->insert(2, 10);
    //    //first algorithm searches in the 2-nd bucket, then in the 4-th bucket
    //    REQUIRE(hash->find(2) == std::vector{ 8, 10, 9 });

    //    REQUIRE(hash->getAllItems() == std::vector<std::pair<int, int>>{ { 14, 5 }, { 1, 2 }, { 2, 8 }, { 2, 10 }, { 2, 9 } });
    //    //implicitly rehashes hash table
    //    hash->disableTwoChoiceHashing();
    //    REQUIRE(hash->getAllItems() == std::vector<std::pair<int, int>>{ { 14, 5 }, { 1, 2 }, { 2, 8 }, { 2, 10 }, { 2, 9 } });
    //    //now there are 11 buckets
    //    hash->rehash(4);
    //    //key 14 moves to the 3-rd bucket
    //    REQUIRE(hash->getAllItems() == std::vector<std::pair<int, int>>{ { 1, 2 }, { 2, 8 }, { 2, 10 }, { 2, 9 }, { 14, 5 } });
    //}

    SUBCASE("Coalesced hashing")
    {
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
    }

    SUBCASE("Heap Sort")
    {
    }

    SUBCASE("Merge Sort")
    {
    }

    SUBCASE("Bucket Sort")
    {
    }

    SUBCASE("Insertion Sort")
    {
    }

    SUBCASE("Quicksort")
    {
    }

    SUBCASE("Radix Sort")
    {
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
		std::unique_ptr<MapTreeType<std::string, int>> treeType(new MapTreeTypeBPlus<std::string, int>(2));
		std::unique_ptr<Map<std::string, int>> treeMap(new TreeMap(treeType.get()));
		testMap(treeMap.get());
	}
}

TEST_CASE("Set")
{

    SUBCASE("find element")
    {
    }

    SUBCASE("add element")
    {
    }

    SUBCASE("delete element")
    {
    }
}

TEST_CASE("Set operations")
{

    SUBCASE("union")
    {
    }

    SUBCASE("intersection")
    {
    }

    SUBCASE("difference")
    {
    }

    SUBCASE("symmetric_difference")
    {
    }
}
