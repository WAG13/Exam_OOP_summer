#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include"../Data/DateTime.h"
#include"../Data/DataGenerator.h"
#include "../HashTables/BucketHashTable.h"
#include "../Trees/AVLTree.h"
#include "../Trees/BPlusTree.h"
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
    /*SUBCASE("Separate hashing") {
        BucketHashTable<int, std::string, std::function<size_t(int)>, std::vector<std::pair<int, std::string>>> 
            hash{ {1, "one"}, {2, "two"}, {3, "three"} };
    }*/

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

TEST_CASE("Key-value")
{

    SUBCASE("get Value by Key")
    {
    }

    SUBCASE("change Value by Key")
    {
    }

    SUBCASE("add Value by Key")
    {
    }

    SUBCASE("delete Value by Key")
    {
    }

    SUBCASE("get all Keys")
    {
    }

    SUBCASE("get all Values")
    {
    }

    SUBCASE("get all Key-value")
    {
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
