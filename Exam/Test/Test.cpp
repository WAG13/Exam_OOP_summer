#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "Trees/AVLTree.h"
#include "Trees/BPlusTree.h"
#include <vector>
#include <memory>

TEST_CASE("Data")
{
    SUBCASE("Creating data")
    {
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

    SUBCASE("Coalesced hashing")
    {
    }

    SUBCASE("Hopscotch hashing")
    {
    }
}

TEST_CASE("Tree")
{
	SUBCASE("AVL tree")
	{
		std::unique_ptr<lists::AVLTreeSimple<int>> tree(new lists::AVLTreeSimple<int>());
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
		std::vector<int> result = tree->find_all(7, 14);
		REQUIRE(result == std::vector<int> { 8, 11 });
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
