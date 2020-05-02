#pragma once
#pragma once

#include <new>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

using namespace std;

//struct _time
//{
//	int day = 0;
//	int month = 0;
//	int year = 0;
//};

struct contract
{
	int num;
	tm begin;
	tm end;
	int job;
	int payment;
};

constexpr int BLACK(0);
constexpr int RED(1);

template<typename K, typename V>

class Red_Black
{
private:
	struct Node
	{
		K key;
		V value;
		int color;
		Node* parent;
		Node* left;
		Node* right;
		Node() : key(), value(), color{ RED }, left{ nullptr },
			right{ nullptr }, parent{ nullptr } {};
		Node(K k, V v) : key{ k }, value{ v }, color{ RED },
			left{ nullptr }, right{ nullptr }, parent{ nullptr } {};
	};

	Node* root;

	//дедушка
	Node* grandparent(Node* node)
	{
		if ((node != nullptr) && (node->parent != nullptr))
			return node->parent->parent;
		else
			return nullptr;
	}

	//дядя
	Node* uncle(Node* n)
	{
		Node* grp = grandparent(n);
		if (grp == nullptr)
			return nullptr; // No grandparent means no uncle
		if (n->parent == grp->left)
			return grp->right;
		else
			return grp->left;
	}


	//брат
	Node* sibling(Node* n)
	{
		if (n == n->parent->left)
			return n->parent->right;
		else
			return n->parent->left;
	}

	//Левый поворот
	Node* rotate_left(Node* root, Node* n)
	{
		Node* temp = n->right;
		n->right = temp->left;

		if (temp->left != nullptr)
			temp->left->parent = n;

		temp->parent = n->parent; /* при этом, возможно, становится корнем дерева */

		if (n->parent == nullptr)
			root = temp;
		else if (n == n->parent->left)
			n->parent->left = temp;
		else
			n->parent->right = temp;

		n->parent = temp;
		temp->left = n;

		return root;
	}

	//Правый поворот
	Node* rotate_right(Node* root, Node* n)
	{
		Node* temp = n->left;
		n->left = temp->right;

		if (temp->right != nullptr)
			temp->right->parent = n;

		temp->parent = n->parent; /* при этом, возможно, становится корнем дерева */

		if (n->parent == nullptr)
			root = temp;
		else if (n == n->parent->right)
			n->parent->right = temp;
		else
			n->parent->left = temp;

		n->parent = temp;
		temp->right = n;

		return root;
	}

	//Вставка нового
	Node* Insert(Node* root, K k, V v)
	{
		Node* node = root, * temp = nullptr, * newnode = new Node(k, v);

		if (!root)
			return newnode;

		while (node != nullptr)
		{
			temp = node;
			if (newnode->key < node->key)
				node = node->left;
			else
				node = node->right;
		}

		newnode->parent = temp;

		if (newnode->key < temp->key)
			temp->left = newnode;
		else
			temp->right = newnode;

		root = Insert_fixup(root, newnode);
		root->color = BLACK;

		return root;
	}


	//Проверка цветов и балансировка после вставки
	Node* Insert_fixup(Node* root, Node* newnode)
	{
		Node* temp;

		if (!newnode->parent->parent)
			return root;

		while (newnode->parent && newnode->parent->color == RED)
		{
			if (newnode->parent == newnode->parent->parent->left)
			{
				temp = newnode->parent->parent->right;
				if (temp && temp->color == RED)
				{
					newnode->parent->color = BLACK;
					temp->color = BLACK;
					newnode->parent->parent->color = RED;
					newnode = newnode->parent->parent;
				}
				else
				{
					if (newnode == newnode->parent->right)
					{
						newnode = newnode->parent;
						root = rotate_left(root, newnode);
					}
					newnode->parent->color = BLACK;
					newnode->parent->parent->color = RED;
					root = rotate_right(root, newnode->parent->parent);
				}
			}
			else
			{
				temp = newnode->parent->parent->left;
				if (temp && temp->color == RED)
				{
					newnode->parent->color = BLACK;
					temp->color = BLACK;
					newnode->parent->parent->color = RED;
					newnode = newnode->parent->parent;
				}
				else
				{
					if (newnode == newnode->parent->left)
					{
						newnode = newnode->parent;
						root = rotate_right(root, newnode);
					}
					newnode->parent->color = BLACK;
					newnode->parent->parent->color = RED;
					root = rotate_left(root, newnode->parent->parent);
				}
			}
		}

		return root;
	}

	//Перемещение
	Node* Transplant(Node* root, Node* u, Node* v)
	{
		if (u->parent == nullptr)
			root = v;
		else if (u == u->parent->left)
			u->parent->left = v;
		else
			u->parent->right = v;

		if (v)
			v->parent = u->parent;

		return root;
	}

	//Поиск наименьшего
	Node* minimal(Node* x)
	{
		while (x->left != nullptr)
			x = x->left;

		return x;
	}

	//Удаление из дерева
	Node* Delete_(Node* root, Node* z)
	{
		Node* y = z, * x;
		int8_t orig_col = y->color;

		if (z->left == nullptr)
		{
			x = z->right;
			root = Transplant(root, z, z->right);
		}
		else if (z->right == nullptr)
		{
			x = z->left;
			root = Transplant(root, z, z->left);
		}
		else
		{
			y = minimal(z->right);
			orig_col = y->color;
			x = y->right;
			if (x && y->parent == z)
			{
				x->parent = y;
			}
			else
			{
				root = Transplant(root, y, y->right);
				y->right = z->right;
				if (y->right)
					y->right->parent = y;
			}
			root = Transplant(root, z, y);
			y->left = z->left;
			if (y->left)
				y->left->parent = y;
			y->color = z->color;
		}

		if (orig_col == BLACK)
			root = Delete__Fixup(root, x);

		return root;
	}

	//Проверка цветов и балансировка после удаления
	Node* Delete__Fixup(Node* root, Node* x)
	{
		Node* w;
		while (x && x != root && x->color == BLACK)
		{
			if (x == x->parent->left)
			{
				w = x->parent->right;

				if (w->color == RED)
				{
					w->color = BLACK;
					x->parent->color = RED;
					root = rotate_left(root, x->parent);
					w = x->parent->right;
				}

				if (w->left->color == BLACK && w->right->color == BLACK)
				{
					w->color = RED;
					x = x->parent;
				}
				else
				{
					if (w->right->color == BLACK)
					{
						w->left->color = BLACK;
						w->color = RED;
						root = rotate_right(root, w);
						w = x->parent->right;
					}

					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					root = rotate_left(root, x->parent);
					x = root;
				}
			}
			else
			{
				w = x->parent->left;

				if (w->color == RED)
				{
					w->color = BLACK;
					x->parent->color = RED;
					root = rotate_right(root, x->parent);
					w = x->parent->left;
				}

				if (w->right->color == BLACK && w->left->color == BLACK)
				{
					w->color = RED;
					x = x->parent;
				}
				else
				{
					if (w->left->color == BLACK)
					{
						w->right->color = BLACK;
						w->color = RED;
						root = rotate_left(root, w);
						w = x->parent->left;
					}

					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					root = rotate_right(root, x->parent);
					x = root;
				}
			}
		}

		if (x)
			x->color = BLACK;
		return root;
	}

	//Поиск по имени 
	Node* Search_(Node* node, K k) {
		if (node == nullptr)
		{
			return nullptr;
		}

		for (Node* iter = node; iter != nullptr; )
		{
			if (k < iter->key) {
				iter = iter->left;
			}
			else if (k > iter->key) {
				iter = iter->right;
			}
			else
			{
				return iter;
			}
		}
		return nullptr;
	}

	//Сумма всех договоров работника
	void sum_(Node* node)
	{
		vector<contract> concs = node->value;
		int sum = 0;

		for (int i = 0; i < concs.size(); i++)
			sum += concs[i].payment;

		cout << endl << "Стоимость всех договоров работника '" << node->key << "': " << sum << endl << endl;
	}

	//Вывод всех контрактов данного пользователя
	void contract_(Node* node)
	{
		vector<contract> concs = node->value;

		cout << node->key << "\n" << "{" << "\n";
		for (int i = 0; i < concs.size(); i++)
			print_contract(concs[i]);
		cout << "}\n";
	}

	//Поиск самого дорогого контракта
	void expst_(Node* node)
	{
		vector<contract> concs = node->value;
		contract expst;
		expst.payment = -1;

		for (int i = 0; i < concs.size(); i++)
			if (concs[i].payment > expst.payment)
				expst = concs[i];

		cout << "Самый дорогой договор '" << node->key << "':\n";
		print_contract(expst);
		cout << endl;
	}

	//Поиск самого продолжительного контракта
	void longest_(Node* node)
	{
		vector<contract> concs = node->value;
		contract longest = concs[0];

		for (int i = 1; i < concs.size(); i++)
			if ((for_comparison(concs[i].end) - for_comparison(concs[i].begin)) > (for_comparison(longest.end) - for_comparison(longest.begin)))
				longest = concs[i];

		cout << "Самый продолжительный договор '" << node->key << "':\n";
		print_contract(longest);
		cout << endl;
	}

	//Форматированный вывод контракта
	void print_contract(contract conc)
	{
		cout << "\t" << "Договор №" << conc.num << " нач. " << conc.begin.tm_mday << ".";
		cout << conc.begin.tm_mon << "." << conc.begin.tm_year << " кон. " << conc.end.tm_mday << ".";
		cout << conc.end.tm_mon << "." << conc.end.tm_year << " Работа ";
		cout << conc.job << " Стоимость " << conc.payment << ";" << "\n";
	}

	//Печатаем дерево
	void PrintTree(Node* node)
	{
		static int cnt = 0;
		if (node != nullptr) {
			cnt++;
			PrintTree(node->right);
			cnt--;
			for (int i = 0; i < cnt; i++)
				std::cout << "\t";
			cout << node->key << "\n";
			cnt++;
			PrintTree(node->left);
			cnt--;
		}
	}

public:

	Red_Black() : root(nullptr) {};

	void Add(K k, V v)
	{
		root = Insert(root, k, v);
	}

	//парсер из файла
	void file_employee(ifstream& file)
	{
		string name, str;
		char c;
		contract emp;
		vector<contract> employees;
		while (file)
		{

			getline(file, name);
			file >> c;
			//пустой договор пофиксить
			//проверить на пустое дерево или одна попытка удаления
			while (file)
			{
				file >> str;
				file >> c;
				file >> emp.num;
				file >> str;
				file >> emp.begin.tm_mday;
				file >> c;
				file >> emp.begin.tm_mon;
				file >> c;
				file >> emp.begin.tm_year;
				file >> str;
				file >> emp.end.tm_mday;
				file >> c;
				file >> emp.end.tm_mon;
				file >> c;
				file >> emp.end.tm_year;
				file >> str;
				file >> emp.job;
				file >> str;
				file >> emp.payment;
				file >> c;
				employees.push_back(emp);
				file >> c;
				if (c == '}')
					break;
			}
			this->Add(name, employees);
			getline(file, name);
		}
	}

	Node* Find(K k)
	{
		return Search_(root, k);
	}

	void sum(K k)
	{
		sum_(Search_(root, k));
	}

	void list(K k)
	{
		contract_(Search_(root, k));
	}

	void expensivest(K k)
	{
		expst_(Search_(root, k));
	}

	void longest(K k)
	{
		longest_(Search_(root, k));
	}

	void Delete(K k)
	{
		root = Delete_(root, Search_(root, k));
	}

	void Print()
	{
		PrintTree(root);
	}

};

//енто, чтобы даты сравнивать -- перевод в дни
int for_comparison(tm date)
{
	int a = (14 - date.tm_mon) / 12;
	int y = date.tm_year + 4800 - a;
	int m = date.tm_mon + 12 * a - 3;

	return (date.tm_mday + (153 * m + 2) / 5 + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045);
}
