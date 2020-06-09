#include <iostream>
#include<string>
#include<vector>
#include "Lists/LinkedList.h"
#include "Lists/DoublyLinkedList.h"
#include "Data/DataGenerator.h"

int foo(int a) { return (a+5); }
int main()
{
    LinkedList<int,int> list(foo);
    for (int i = 0; i < 15;i++)
        list.insertNewNode(i);
        list.insertNewNode(55);
        list.insertNewNode(5);
        list.deleteByKey(5);
        list.deleteByValue(5);
        list.deleteByValue(55);
       
    DoublyLinkedList<int, int> list2(foo);
    for (int i = 0; i < 15; i++)
        list2.insertNewNode(i);
    list2.insertNewNode(55);
    list2.insertNewNode(5);
    list2.deleteByKey(5);
    list2.deleteByValue(5);
    list2.deleteByValue(55);

    list.show();
    list2.show();
    return 1;
}
