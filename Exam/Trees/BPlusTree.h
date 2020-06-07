#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP

#include "SearchTree.h"
#include <vector>
#include <iostream>

namespace lists
{
	template<typename T, typename Key>
	struct BPlusNode
	{
		bool isLeaf = true;
		BPlusNode<T, Key>* next = nullptr; //якщо isLeaf, вказує на наступний leaf

		std::vector<T> values; //n ключів, t-1 <= n <=2t-1
		std::vector<BPlusNode<T, Key>*> children;
		//кількість дітей = n + 1;
		//де вузол номер N містить значення між values[N-1] та values[N]

		//пошук елемента
		bool contains(const Key& key, GetKeyFuncType<T, Key> getKeyFunc) const;

		//розбиття вузла
		void splitChild(size_t index, size_t t);

		//вставка елемента (передумова: values.size() < 2*t - 1)
		void insertNonFull(const T& element, GetKeyFuncType<T, Key> getKeyFunc, size_t t);

		//видалення елемента (передумова: values.size() > t - 1, якщо це не корінь)
		void remove(const Key& key, GetKeyFuncType<T, Key> getKeyFunc, size_t t);

		//для деструктора дерева
		void deleteChildren();

		//вивід на екран
		void printStructure(std::ostream& os, int depth) const;
	};

	//Лінійний пошук серед ключів, якщо потрібно, спускаємось
	template<typename T, typename Key>
	bool BPlusNode<T, Key>::contains(const Key& key, GetKeyFuncType<T, Key> getKeyFunc) const
	{
		size_t i = 0;
		//TODO: Можна замінити на binary search для більшої ефективності при великих values.size()
		for (; i < values.size(); i++)
		{
			if (values[i] >= element)
				break;
		}
		if (isLeaf && i < values.size() && getKeyFunc(values[i]) == getKeyFunc(element))
			return true;

		if (!isLeaf)
		{
			//DISK_READ
			return children[i]->contains(element);
		}
		return false;
	}

	template<typename T, typename Key>
	void BPlusNode<T, Key>::splitChild(size_t index, size_t t)
	{
		BPlusNode<T, Key>* child = children[index];

		//Створення вузла і перенесення туди (t–1) більших
		//ключів та відповідно t правих дочірніх вузлів.

		BPlusNode<T, Key>* newNode = new BPlusNode<T, Key>();
		newNode->isLeaf = child->isLeaf;

		newNode->values.reserve(t - 1);
		for (size_t i = 0; i < t - 1; i++)
			newNode->values.push_back(child->values[t + i]);

		if (!child->isLeaf)
		{
			newNode->children.reserve(t);
			for (size_t i = 0; i < t; i++)
				newNode->children.push_back(child->children[t + i]);
		}

		//Оновлення кільності ключів в child
		if (child->isLeaf)
		{
			child->values.resize(t);
		}
		if (!child->isLeaf)
		{
			child->values.resize(t - 1);
			child->children.resize(t);
		}

		//Вставка newNode як дочірнього вузла this з переносом медіани
		//з child в this для розділення child та newNode

		children.insert(children.begin() + index + 1, newNode); //операція O(n)
		values.insert(values.begin() + index, child->values[t - 1]); //операція O(n)

		//Оновлюємо next
		newNode->next = child->next;
		child->next = newNode;

		//DISK_WRITE(this)
		//DISK_WRITE(newNode)
		//DISK_WRITE(child)
	}

	template<typename T, typename Key>
	void BPlusNode<T, Key>::insertNonFull(const T& element, GetKeyFuncType<T, Key> getKeyFunc, size_t t)
	{
		if (isLeaf)
		{
			//проста вставка в лист

			if (values.empty() || getKeyFunc(element) > getKeyFunc(values.back()))
			{
				values.push_back(element);
			}
			else
			{
				//шукаємо потрібний індекс і зміщуюємо елементи
				size_t i = values.size() - 1;

				values.push_back(values.back());
				while (i > 0 && getKeyFunc(element) < getKeyFunc(values[i - 1]))
				{
					values[i] = values[i - 1];
					i--;
				}
				values[i] = element;
				//DISK_WRITE(this)
			}
		}
		else
		{
			//Не лист, шукаємо піддерево для спуску
			size_t i = values.size();

			while (i > 0 && getKeyFunc(element) < getKeyFunc(values[i - 1]))
				i--;

			//DISK_READ(children[i])
			if (children[i]->values.size() == 2 * t - 1)
			{
				//вузол внизу повний - ділимо
				splitChild(i, t);

				//куди спускатися після поділу
				if (element > values[i])
					i++;
			}
			children[i]->insertNonFull(element, t);
		}
	}

	template<typename T, typename Key>
	void BPlusNode<T, Key>::remove(const Key& key, GetKeyFuncType<T, Key> getKeyFunc, size_t t)
	{
		//передумова: values.size() > t - 1, якщо це не корінь
		if (isLeaf)
		{
			//Якщо ключ k знаходиться у вузлі x та останній є
			//листом – видаляємо k з x.

			for (size_t i = 0; i < values.size(); i++)
			{
				if (getKeyFunc(values[i]) == key)
				{
					values.erase(values.begin() + i); //операція O(n)
					return; //знайшли і видалили
				}
			}
			return; //не знайшли елемент
		}

		//якщо це внутрішній вузол:
		//знаходимо піддерево, яке може містити element
		size_t i = 0;
		while (i < values.size() && key > getKeyFunc(values[i]))
			i++;

		BPlusNode<T, Key>* node = children[i];
		if (node->values.size() > t - 1)
		{
			//умова виконується
			node->remove(element, t);
		}
		else
		{
			//якщо в ньому лише t-1 елементів:
			//розглядаємо сусідів.

			//якщо у сусіда зліва достатньо елементів, заберемо у нього
			if (i > 0 && children[i - 1]->values.size() > t - 1)
			{
				BPlusNode<T, Key>* leftBro = children[i - 1];
				if (leftBro->isLeaf)
				{
					//Для листа:

					//переносимо найбільший елемент
					node->values.insert(node->values.begin(), leftBro->values.back());
					leftBro->values.pop_back();

					//Замінимо ключ-роздільник
					values[i - 1] = leftBro->values.back();
				}
				else
				{
					//Для внутрішнього вузла:

					//Роздільник переносимо в піддерево
					node->values.insert(node->values.begin(), values[i - 1]);
					//На його місце ставимо крайній роздільник сусіда
					values[i - 1] = leftBro->values.back();
					leftBro->values.pop_back();

					//І переносимо відповідний вказівник
					node->children.insert(node->children.begin(), leftBro->children.back());
					leftBro->children.pop_back();
				}

				//тепер умова виконується
				node->remove(key, getKeyFunc, t);
				return;
			}
			//аналогічно справа
			if (i < children.size() - 1 && children[i + 1]->values.size() > t - 1)
			{
				BPlusNode<T, Key>* rightBro = children[i + 1];
				if (rightBro->isLeaf)
				{
					//Для листа:

					//переносимо найменший елемент
					node->values.push_back(rightBro->values.front());
					rightBro->values.erase(rightBro->values.begin());

					//Замінимо ключ-роздільник
					values[i] = rightBro->values.front();
				}
				else
				{
					//Для внутрішнього вузла:

					//Роздільник переносимо в піддерево
					node->values.push_back(values[i]);
					//На його місце ставимо крайній роздільник сусіда
					values[i] = rightBro->values.front();
					rightBro->values.erase(rightBro->values.begin());

					//І переносимо відповідний вказівник
					node->children.push_back(rightBro->children.front());
					rightBro->children.erase(rightBro->children.begin());
				}

				//тепер умова виконується
				node->remove(key, getKeyFunc, t);
				return;
			}
			//У обох сусідів по (t-1) ключу, об`єднаємо
			if (i < children.size() - 1)
			{
				//Об`єднання справа

				BPlusNode<T, Key>* rightBro = children[i + 1];

				if (node->isLeaf)
				{
					//Оновлюємо next
					node->next = rightBro->next;
					//Ключ-вказівник видаляємо
					values.erase(values.begin() + i);
				}
				else
				{
					//Ключ-роздільник стає медіаною
					node->values.push_back(values[i]);
					values.erase(values.begin() + i);

				}

				//Об'єднуємо ключі (O(n))
				node->values.reserve(node->values.size() + rightBro->values.size());
				node->values.insert(node->values.end(), rightBro->values.begin(), rightBro->values.end());

				if (!node->isLeaf)
				{
					//Об'єднуємо вказівники (O(n))
					node->children.reserve(node->children.size() + rightBro->children.size());
					node->children.insert(node->children.end(), rightBro->children.begin(), rightBro->children.end());
				}
				//видаляємо брата
				delete rightBro;
				children.erase(children.begin() + i + 1);

				//тепер умова виконується
				node->remove(key, getKeyFunc, t);
				return;
			}
			if (i > 0)
			{
				//Об`єднання зліва

				BPlusNode<T, Key>* leftBro = children[i - 1];

				if (node->isLeaf)
				{
					//Оновлюємо next
					leftBro->next = node->next;
					//Ключ-вказівник видаляємо
					values.erase(values.begin() + i - 1);
				}
				else
				{
					//Ключ-роздільник стає медіаною
					leftBro->values.push_back(values[i - 1]);
					values.erase(values.begin() + i - 1);
				}

				//Об'єднуємо ключі (O(n))
				leftBro->values.reserve(leftBro->values.size() + node->values.size());
				leftBro->values.insert(leftBro->values.end(), node->values.begin(), node->values.end());

				if (!node->isLeaf)
				{
					//Об'єднуємо вказівники (O(n))
					leftBro->children.reserve(leftBro->children.size() + node->children.size());
					leftBro->children.insert(leftBro->children.end(), node->children.begin(), node->children.end());
				}
				//видаляємо брата
				delete node;
				children.erase(children.begin() + i);

				//тепер умова виконується
				leftBro->remove(key, getKeyFunc, t);
				return;
			}
		}
	}

	template<typename T, typename Key>
	void BPlusNode<T, Key>::deleteChildren()
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i])
			{
				children[i]->deleteChildren();
				delete children[i];
			}
		}
	}

	template<typename T, typename Key>
	void BPlusNode<T, Key>::printStructure(std::ostream& os, int depth) const
	{
		//    for(int i = 0; i < depth; i++)
		//        os << "   ";
		//    os << "leaf? " << isLeaf << " children: " << children.size() << " values: " << values.size() << '\n';
		for (size_t i = 0; i < values.size(); i++)
		{
			if (!isLeaf)
				children[i]->printStructure(os, depth + 1);
			for (int i = 0; i < depth; i++)
				os << "   ";
			os << values[i] << std::endl;
		}
		if (!children.empty())
			children.back()->printStructure(os, depth + 1);
	}





	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	class BPlusTree : public SearchTree<T, Key, GetKeyFunc>
	{
	public:
		BPlusTree(size_t t);
		virtual ~BPlusTree();

		void insert(const T& element);
		void printStructure(std::ostream& os) const;
		void printInOrder(std::ostream& os) const;
		bool contains(const Key& key) const;
		void remove(const Key& key);
	private:
		size_t t;
		BPlusNode<T, Key>* root;

		friend BPlusNode<T, Key>;
	};

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	BPlusTree<T, Key, GetKeyFunc>::BPlusTree(size_t t)
		: t(t), root(new BPlusNode<T, Key>())
	{}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	BPlusTree<T, Key, GetKeyFunc>::~BPlusTree()
	{
		root->deleteChildren();
		delete root;
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void BPlusTree<T, Key, GetKeyFunc>::insert(const T& element)
	{
		BPlusNode<T, Key>* currentRoot = root;

		if (currentRoot->values.size() == 2 * t - 1)
		{
			//випадок заповненого кореня

			root = new BPlusNode<T, Key>();
			root->isLeaf = false;
			root->children.push_back(currentRoot);
			root->splitChild(0, t);
			root->insertNonFull(element, t);
		}
		else
		{
			currentRoot->insertNonFull(element, t);
		}
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void BPlusTree<T, Key, GetKeyFunc>::remove(const Key& key)
	{
		root->remove(key, GetKeyFunc, t);
		//Зменшення висоти дерева
		if (root->children.size() == 1)
		{
			BPlusNode<T, Key>* child = root->children[0];
			delete root;
			root = child;
		}
	}


	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void BPlusTree<T, Key, GetKeyFunc>::printStructure(std::ostream& os) const
	{
		root->printStructure(os, 0);
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	bool BPlusTree<T, Key, GetKeyFunc>::contains(const Key& key) const
	{
		return root->contains(key, GetKeyFunc);
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void BPlusTree<T, Key, GetKeyFunc>::printInOrder(std::ostream& os) const
	{
		//Вивід на екран за допомогою next

		//Знаходимо мінімальний лист
		BPlusNode<T, Key>* node = root;
		while (!node->children.empty())
			node = node->children[0];

		//Вивід на екран
		bool first = true;
		os << "[ ";
		while (node)
		{
			for (size_t i = 0; i < node->values.size(); i++)
			{
				if (!first)
					os << ", ";
				os << node->values[i];
				first = false;
			}
			node = node->next;
		}
		os << " ]";
	}


#endif // BPLUSTREE_HPP

}