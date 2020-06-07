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
		BPlusNode<T, Key>* next = nullptr; //���� isLeaf, ����� �� ��������� leaf

		std::vector<T> values; //n ������, t-1 <= n <=2t-1
		std::vector<BPlusNode<T, Key>*> children;
		//������� ���� = n + 1;
		//�� ����� ����� N ������ �������� �� values[N-1] �� values[N]

		//����� ��������
		bool contains(const Key& key, GetKeyFuncType<T, Key> getKeyFunc) const;

		//�������� �����
		void splitChild(size_t index, size_t t);

		//������� �������� (����������: values.size() < 2*t - 1)
		void insertNonFull(const T& element, GetKeyFuncType<T, Key> getKeyFunc, size_t t);

		//��������� �������� (����������: values.size() > t - 1, ���� �� �� �����)
		void remove(const Key& key, GetKeyFuncType<T, Key> getKeyFunc, size_t t);

		//��� ����������� ������
		void deleteChildren();

		//���� �� �����
		void printStructure(std::ostream& os, int depth) const;
	};

	//˳����� ����� ����� ������, ���� �������, ����������
	template<typename T, typename Key>
	bool BPlusNode<T, Key>::contains(const Key& key, GetKeyFuncType<T, Key> getKeyFunc) const
	{
		size_t i = 0;
		//TODO: ����� ������� �� binary search ��� ����� ����������� ��� ������� values.size()
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

		//��������� ����� � ����������� ���� (t�1) ������
		//������ �� �������� t ������ ������� �����.

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

		//��������� ������� ������ � child
		if (child->isLeaf)
		{
			child->values.resize(t);
		}
		if (!child->isLeaf)
		{
			child->values.resize(t - 1);
			child->children.resize(t);
		}

		//������� newNode �� ���������� ����� this � ��������� ������
		//� child � this ��� ��������� child �� newNode

		children.insert(children.begin() + index + 1, newNode); //�������� O(n)
		values.insert(values.begin() + index, child->values[t - 1]); //�������� O(n)

		//��������� next
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
			//������ ������� � ����

			if (values.empty() || getKeyFunc(element) > getKeyFunc(values.back()))
			{
				values.push_back(element);
			}
			else
			{
				//������ �������� ������ � �������� ��������
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
			//�� ����, ������ �������� ��� ������
			size_t i = values.size();

			while (i > 0 && getKeyFunc(element) < getKeyFunc(values[i - 1]))
				i--;

			//DISK_READ(children[i])
			if (children[i]->values.size() == 2 * t - 1)
			{
				//����� ����� ������ - �����
				splitChild(i, t);

				//���� ���������� ���� �����
				if (element > values[i])
					i++;
			}
			children[i]->insertNonFull(element, t);
		}
	}

	template<typename T, typename Key>
	void BPlusNode<T, Key>::remove(const Key& key, GetKeyFuncType<T, Key> getKeyFunc, size_t t)
	{
		//����������: values.size() > t - 1, ���� �� �� �����
		if (isLeaf)
		{
			//���� ���� k ����������� � ���� x �� ������� �
			//������ � ��������� k � x.

			for (size_t i = 0; i < values.size(); i++)
			{
				if (getKeyFunc(values[i]) == key)
				{
					values.erase(values.begin() + i); //�������� O(n)
					return; //������� � ��������
				}
			}
			return; //�� ������� �������
		}

		//���� �� �������� �����:
		//��������� ��������, ��� ���� ������ element
		size_t i = 0;
		while (i < values.size() && key > getKeyFunc(values[i]))
			i++;

		BPlusNode<T, Key>* node = children[i];
		if (node->values.size() > t - 1)
		{
			//����� ����������
			node->remove(element, t);
		}
		else
		{
			//���� � ����� ���� t-1 ��������:
			//���������� �����.

			//���� � ����� ���� ��������� ��������, �������� � �����
			if (i > 0 && children[i - 1]->values.size() > t - 1)
			{
				BPlusNode<T, Key>* leftBro = children[i - 1];
				if (leftBro->isLeaf)
				{
					//��� �����:

					//���������� ��������� �������
					node->values.insert(node->values.begin(), leftBro->values.back());
					leftBro->values.pop_back();

					//������� ����-���������
					values[i - 1] = leftBro->values.back();
				}
				else
				{
					//��� ����������� �����:

					//��������� ���������� � ��������
					node->values.insert(node->values.begin(), values[i - 1]);
					//�� ���� ���� ������� ������ ��������� �����
					values[i - 1] = leftBro->values.back();
					leftBro->values.pop_back();

					//� ���������� ��������� ��������
					node->children.insert(node->children.begin(), leftBro->children.back());
					leftBro->children.pop_back();
				}

				//����� ����� ����������
				node->remove(key, getKeyFunc, t);
				return;
			}
			//��������� ������
			if (i < children.size() - 1 && children[i + 1]->values.size() > t - 1)
			{
				BPlusNode<T, Key>* rightBro = children[i + 1];
				if (rightBro->isLeaf)
				{
					//��� �����:

					//���������� ��������� �������
					node->values.push_back(rightBro->values.front());
					rightBro->values.erase(rightBro->values.begin());

					//������� ����-���������
					values[i] = rightBro->values.front();
				}
				else
				{
					//��� ����������� �����:

					//��������� ���������� � ��������
					node->values.push_back(values[i]);
					//�� ���� ���� ������� ������ ��������� �����
					values[i] = rightBro->values.front();
					rightBro->values.erase(rightBro->values.begin());

					//� ���������� ��������� ��������
					node->children.push_back(rightBro->children.front());
					rightBro->children.erase(rightBro->children.begin());
				}

				//����� ����� ����������
				node->remove(key, getKeyFunc, t);
				return;
			}
			//� ���� ����� �� (t-1) �����, ��`������
			if (i < children.size() - 1)
			{
				//��`������� ������

				BPlusNode<T, Key>* rightBro = children[i + 1];

				if (node->isLeaf)
				{
					//��������� next
					node->next = rightBro->next;
					//����-�������� ���������
					values.erase(values.begin() + i);
				}
				else
				{
					//����-��������� ��� �������
					node->values.push_back(values[i]);
					values.erase(values.begin() + i);

				}

				//��'������ ����� (O(n))
				node->values.reserve(node->values.size() + rightBro->values.size());
				node->values.insert(node->values.end(), rightBro->values.begin(), rightBro->values.end());

				if (!node->isLeaf)
				{
					//��'������ ��������� (O(n))
					node->children.reserve(node->children.size() + rightBro->children.size());
					node->children.insert(node->children.end(), rightBro->children.begin(), rightBro->children.end());
				}
				//��������� �����
				delete rightBro;
				children.erase(children.begin() + i + 1);

				//����� ����� ����������
				node->remove(key, getKeyFunc, t);
				return;
			}
			if (i > 0)
			{
				//��`������� ����

				BPlusNode<T, Key>* leftBro = children[i - 1];

				if (node->isLeaf)
				{
					//��������� next
					leftBro->next = node->next;
					//����-�������� ���������
					values.erase(values.begin() + i - 1);
				}
				else
				{
					//����-��������� ��� �������
					leftBro->values.push_back(values[i - 1]);
					values.erase(values.begin() + i - 1);
				}

				//��'������ ����� (O(n))
				leftBro->values.reserve(leftBro->values.size() + node->values.size());
				leftBro->values.insert(leftBro->values.end(), node->values.begin(), node->values.end());

				if (!node->isLeaf)
				{
					//��'������ ��������� (O(n))
					leftBro->children.reserve(leftBro->children.size() + node->children.size());
					leftBro->children.insert(leftBro->children.end(), node->children.begin(), node->children.end());
				}
				//��������� �����
				delete node;
				children.erase(children.begin() + i);

				//����� ����� ����������
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
			//������� ����������� ������

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
		//��������� ������ ������
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
		//���� �� ����� �� ��������� next

		//��������� ��������� ����
		BPlusNode<T, Key>* node = root;
		while (!node->children.empty())
			node = node->children[0];

		//���� �� �����
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