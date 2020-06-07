#ifndef AVLTREE
#define AVLTREE

#include "SearchTree.h"

#include <compare>
#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <stack>

namespace lists
{
	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	struct AVLTree : public SearchTree<T, Key, GetKeyFunc>
	{
		AVLTree()
		{}
		~AVLTree() override;

		void add(const T& element) override;
		void printAll(std::ostream& os) const override;
		bool remove(const Key& key) override;
		bool contains(const Key& key) const override;
		std::vector<T> find_all(const Key& min, const Key& max) const /*override*/;
		void forEach(std::function<void(const T&)> func) const override;

		void print_as_tree(std::ostream& os) const;

	private:
		static const signed char LEFT_HEAVY = -1;
		static const signed char BALANCED = 0;
		static const signed char RIGHT_HEAVY = 1;

		struct Node
		{
			char balance = BALANCED;
			Node* left = nullptr;
			Node* right = nullptr;
			T value;

			Node(const T& value)
				: value(value)
			{}

			void print(std::ostream& os, bool& first_element) const;
			void print_as_tree(std::ostream& os, int levels) const;
			void forEach(std::function<void(const T&)> func) const;
		};

		Node* root = nullptr;

		//returns true if height has changed
		static bool rotate_left(Node*& node);
		static bool rotate_right(Node*& node);
		static void rotate_right_left(Node*& node);
		static void rotate_left_right(Node*& node);
		static void add(Node*& current_node, Node* new_node, bool& recalculate_balance);

		bool remove_standard_bst(const Key& key, std::stack<Node**>& path);
		void remove_node_standard_bst(Node** node, std::stack<Node**>& path);
	};

	template<typename T>
	using AVLTreeSimple = AVLTree<T, T, detail::getValueAsKey<T>>;



	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	AVLTree<T, Key, GetKeyFunc>::~AVLTree()
	{
		std::queue<Node*> queue;
		if (root)
			queue.push(root);
		while (!queue.empty())
		{
			Node* root = queue.front();
			queue.pop();
			if (root->left)
				queue.push(root->left);
			if (root->right)
				queue.push(root->right);
			delete root;
		}
	}



	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	bool AVLTree<T, Key, GetKeyFunc>::rotate_left(Node*& node)
	{
#if DEBUG
		std::cout << "rotate left ";
		std::cout << node->value << std::endl;
#endif // DEBUG

		Node* right_child = node->right;

		bool height_changed = true;
		if (right_child->balance == BALANCED) //only happens with deletion
		{
			right_child->balance = LEFT_HEAVY;
			node->balance = RIGHT_HEAVY;
			height_changed = false;
		}
		else
		{
			right_child->balance = BALANCED;
			node->balance = BALANCED;
		}

		node->right = right_child->left;
		right_child->left = node;
		node = right_child;
		return height_changed;
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	bool AVLTree<T, Key, GetKeyFunc>::rotate_right(Node*& node)
	{
#if DEBUG
		std::cout << "rotate right ";
		std::cout << node->value << std::endl;
#endif // DEBUG

		Node* left_child = node->left;

		bool height_changed = true;
		if (left_child->balance == BALANCED) //only happens with deletion
		{
			left_child->balance = RIGHT_HEAVY;
			node->balance = LEFT_HEAVY;
			height_changed = false;
		}
		else
		{
			left_child->balance = BALANCED;
			node->balance = BALANCED;
		}
		node->left = left_child->right;
		left_child->right = node;
		node = left_child;
		return height_changed;
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::rotate_left_right(Node*& node)
	{
#if DEBUG
		std::cout << "rotate left right ";
		std::cout << node->value << std::endl;
#endif // DEBUG

		Node* left_child = node->left;
		Node* new_parent = left_child->right;

		if (new_parent->balance == RIGHT_HEAVY)
		{
			node->balance = BALANCED;
			left_child->balance = LEFT_HEAVY;
		}
		else if (new_parent->balance == BALANCED)
		{
			node->balance = BALANCED;
			left_child->balance = BALANCED;
		}
		else
		{
			node->balance = RIGHT_HEAVY;
			left_child->balance = BALANCED;
		}
		new_parent->balance = BALANCED;

		left_child->right = new_parent->left;
		new_parent->left = left_child;
		node->left = new_parent->right;
		new_parent->right = node;
		node = new_parent;
	}



	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::rotate_right_left(Node*& node)
	{
#if DEBUG
		std::cout << "rotate right left ";
		std::cout << node->value << std::endl;
#endif // DEBUG

		Node* right_child = node->right;
		Node* new_parent = right_child->left;

		if (new_parent->balance == LEFT_HEAVY)
		{
			node->balance = BALANCED;
			right_child->balance = RIGHT_HEAVY;
		}
		else if (new_parent->balance == BALANCED)
		{
			node->balance = BALANCED;
			right_child->balance = BALANCED;
		}
		else
		{
			node->balance = LEFT_HEAVY;
			right_child->balance = BALANCED;
		}
		new_parent->balance = BALANCED;

		right_child->left = new_parent->right;
		new_parent->right = right_child;
		node->right = new_parent->left;
		new_parent->left = node;
		node = new_parent;
	}


	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::add(const T& element)
	{
		if (!root)
		{
			root = new Node(element);
			return;
		}
		bool temp = false;
		//        std::cout << "ADDING " << element << '\n';
		add(root, new Node(element), temp);
		//        printAll(std::cout);
		//        std::cout << "\n\n\n";
	}

	//Entirely recursive algorithm https://cis.stvincent.edu/html/tutorials/swd/avltrees/avltrees.html
	//The advantage is that we don't have to calculate the height when rebalancing

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::add(Node*& current_node, Node* new_node, bool& recalculate_balance)
	{
		if (!current_node)
		{
			current_node = new_node;
			recalculate_balance = true;
			return;
		}

		bool should_rebalance_current = false;
		if (GetKeyFunc(new_node->value) <=> GetKeyFunc(current_node->value) < 0)
		{
			add(current_node->left, new_node, should_rebalance_current);
			if (should_rebalance_current)
			{
				if (current_node->balance == BALANCED)
				{
					current_node->balance = LEFT_HEAVY;
					recalculate_balance = true;
				}
				else if (current_node->balance == RIGHT_HEAVY)
				{
					current_node->balance = BALANCED;
					recalculate_balance = false;
				}
				else
				{
					//Rebalance
//                    std::cout << "\nBEFORE:\n\n";
//                    print_as_tree(std::cout);
//                    std::cout << "current: " << current_node->value << ", inserting: " << new_node->value << std::endl;
					if (current_node->left->balance == LEFT_HEAVY)
					{
						rotate_right(current_node);
						recalculate_balance = false;
					}
					else
					{
						rotate_left_right(current_node);
						//                        recalculate_balance = true;
						recalculate_balance = false;
					}
					//                    std::cout << "\n\nAFTER:\n\n";
					//                    print_as_tree(std::cout);
				}
			}
			else
				recalculate_balance = false;
		}
		else
		{
			add(current_node->right, new_node, should_rebalance_current);
			if (should_rebalance_current)
			{
				if (current_node->balance == BALANCED)
				{
					current_node->balance = RIGHT_HEAVY;
					recalculate_balance = true;
				}
				else if (current_node->balance == LEFT_HEAVY)
				{
					current_node->balance = BALANCED;
					recalculate_balance = false;
				}
				else
				{
					//Rebalance
//                    std::cout << "\nBEFORE:\n\n";
//                    print_as_tree(std::cout);
//                    std::cout << "current: " << current_node->value << ", inserting: " << new_node->value << std::endl;
					if (current_node->right->balance == RIGHT_HEAVY)
					{
						rotate_left(current_node);
						recalculate_balance = false;
					}
					else
					{
						rotate_right_left(current_node);
						//                        recalculate_balance = true;
						recalculate_balance = false;
					}
					//                    std::cout << "\n\nAFTER:\n\n";
					//                    print_as_tree(std::cout);
				}
			}
			else
				recalculate_balance = false;
		}
	}


	//iterative approach
	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	bool AVLTree<T, Key, GetKeyFunc>::remove(const Key& key)
	{
#if DEBUG
		std::cout << "REMOVING " << element << '\n';
		print_all(std::cout);
		std::cout << "\n\n";
#endif // DEBUG

		std::stack<Node**> path;

		//step 1: standard BST remove

		if (!remove_standard_bst(key, path))
			return false;

#if DEBUG
		std::cout << "Path length: " << path.size() << std::endl;
		print_all(std::cout);
		std::cout << "\n\n";
#endif

		//step 2: rebalance, starting from removed node's parent
		//note that at this point, balance values haven't been updated

		while (path.size() > 1)
		{
			Node** current_node = path.top();
#if DEBUG
			std::cout << "current " << *current_node << "\n";
#endif

			path.pop();

			Node** parent = path.top();
			if (current_node == &(*parent)->left) //parent's left child height decreased by 1
			{
#if DEBUG
				std::cout << "decreased height on the left, parent " << (*parent)->value << std::endl;
#endif // DEBUG

				if ((*parent)->balance == RIGHT_HEAVY)
				{
#if DEBUG
					std::cout << "BEFORE\n";
					print_all(std::cout);
					std::cout << "\n\n\n";
#endif // DEBUG

					bool height_changed = true;
					Node* right_child = (*parent)->right;
					if (right_child->balance != LEFT_HEAVY)
						height_changed = rotate_left(*parent);
					else
						rotate_right_left(*parent);
					if (!height_changed)
						break;
				}
				else if ((*parent)->balance == BALANCED)
				{
					(*parent)->balance = RIGHT_HEAVY;
					break; //parent absorbs the height decrease
				}
				else
				{
					(*parent)->balance = BALANCED;
				}
			}
			else //parent's right child height decreased by 1
			{
#if DEBUG
				std::cout << "decreased height on the right, parent " << (*parent)->value << "\n";
				std::cout << "current " << *current_node << "\n";
#endif

				if ((*parent)->balance == LEFT_HEAVY)
				{

#if DEBUG
					std::cout << "BEFORE\n";
					print_all(std::cout);
					std::cout << "\n\n\n";
#endif // DEBUG

					bool height_changed = true;
					Node* left_child = (*parent)->left;
					if (left_child->balance != RIGHT_HEAVY)
						height_changed = rotate_right(*parent);
					else
						rotate_left_right(*parent);

					if (!height_changed)
						break;
				}
				else if ((*parent)->balance == BALANCED)
				{
					(*parent)->balance = LEFT_HEAVY;
					break; //parent absorbs the height decrease
				}
				else
				{
					(*parent)->balance = BALANCED;
				}
			}

			//parent might've changed if we rotated
			path.pop();
			path.push(parent);
		}

#if DEBUG
		std::cout << "REMOVED " << element << "\n";
		print_all(std::cout);
		std::cout << "\n\n\n";
#endif // DEBUG

		return true;
	}


	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	bool AVLTree<T, Key, GetKeyFunc>::remove_standard_bst(const Key& key, std::stack<Node**>& path)
	{
		if (!root)
		{
			return false;
		}
		Node** current_node = &root;
		while (current_node)
		{
			auto compare_result = key <=> GetKeyFunc((*current_node)->value);
			if (compare_result < 0 && (*current_node)->left)
			{
				path.push(current_node);
				current_node = &(*current_node)->left;
			}
			else if (compare_result > 0 && (*current_node)->right)
			{
				path.push(current_node);
				current_node = &(*current_node)->right;
			}
			else if (compare_result == 0)
			{
				remove_node_standard_bst(current_node, path);
				return true;
			}
			else
			{
#if DEBUG
				std::cout << "NOT FOUND\n\n";
#endif // DEBUG
				return false;
			}
		}
		return false;
	}

	//Returns true if deleted right child, false is left
	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::remove_node_standard_bst(Node** node, std::stack<Node**>& path)
	{
		if ((*node)->left)
		{
			if (!(*node)->right) //only left child
			{
				Node* temp = *node;
				*node = (*node)->left;
				delete temp;
				path.push(node);
			}
			else //both children
			{
				path.push(node);
				Node** min_node = &(*node)->right;
				while ((*min_node)->left)
				{
					path.push(min_node);
					min_node = &(*min_node)->left;
				}
				(*node)->value = (*min_node)->value;
				remove_node_standard_bst(min_node, path);
			}
		}
		else
		{
			if ((*node)->right)
			{
				Node* temp = *node;
				*node = (*node)->right;
				path.push(node);
				//                path.push(&(*node)->right);
				delete temp;
			}
			else
			{
				Node* temp = *node;
				*node = nullptr;
				path.push(node);
				delete temp;
			}
		}
	}


	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::Node::print(std::ostream& os, bool& first_element) const
	{
		if (left)
			left->print(os, first_element);

		if (first_element)
			first_element = false;
		else
			os << ", ";
		os << value;

		if (right)
			right->print(os, first_element);
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::printAll(std::ostream& os) const
	{
		os << "[ ";
		if (root)
		{
			bool first_element = true;
			root->print(os, first_element);
		}
		os << " ]" << std::endl;
		//        #if DEBUG
		print_as_tree(os);
		//        #endif // DEBUG
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::Node::print_as_tree(std::ostream& os, int levels) const
	{
		for (int i = 0; i < levels; i++)
			os << " - ";
		os << value << "   balance: " << (int)balance;
		os << "   address: " << this << '\n';
		if (left)
		{
			for (int i = 0; i < levels; i++)
				os << " - ";
			os << "Left:\n";
			left->print_as_tree(os, levels + 1);
		}
		if (right)
		{
			for (int i = 0; i < levels; i++)
				os << " - ";
			os << "Right:\n";
			right->print_as_tree(os, levels + 1);
		}
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::print_as_tree(std::ostream& os) const
	{
		std::queue<Node*> queue;
		if (root)
			root->print_as_tree(os, 0);
		else
			os << "Empty tree!\n";
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::Node::forEach(std::function<void(const T&)> func) const
	{
		if (left)
			left->forEach(func);
		func(value);
		if (right)
			right->forEach(func);
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	void AVLTree<T, Key, GetKeyFunc>::forEach(std::function<void(const T&)> func) const
	{
		if (root)
			root->forEach(func);
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	bool AVLTree<T, Key, GetKeyFunc>::contains(const Key& key) const
	{
		//        int searches = 0;
		std::vector<T> result;
		std::queue<Node*> bfs_queue;
		if (root)
			bfs_queue.push(root);
		while (!bfs_queue.empty())
		{
			//            searches++;
			Node* current_node = bfs_queue.front();
			Key current_key = GetKeyFunc(current_node->value);
			bfs_queue.pop();
			if (key == current_key)
			{
				//                std::cout << "(searches: " << searches << ")\n";
				return true;
			}
			else if (key < current_key)
			{
				if (current_node->left)
					bfs_queue.push(current_node->left);
			}
			else if (current_node->right)
				bfs_queue.push(current_node->right);
		}
		return false;
	}

	template<typename T, typename Key, GetKeyFuncType<T, Key> GetKeyFunc>
	std::vector<T> AVLTree<T, Key, GetKeyFunc>::find_all(const Key& min, const Key& max) const
	{
		std::vector<T> result;
		std::queue<Node*> bfs_queue;
		if (root)
			bfs_queue.push(root);
		while (!bfs_queue.empty())
		{
			Node* current_node = bfs_queue.front();
			bfs_queue.pop();
			if (GetKeyFunc(current_node->value) >= min && current_node->left)
				bfs_queue.push(current_node->left);
			if (GetKeyFunc(current_node->value) <= max)
			{
				if (current_node->right)
					bfs_queue.push(current_node->right);
				if (GetKeyFunc(current_node->value) >= min)
					result.push_back(current_node->value);
			}
		}
		return result;
	}
}
#endif // AVLTREE
