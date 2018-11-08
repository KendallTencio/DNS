//Splay Tree

#include<cstdio>
#include<cstdlib>
struct Node{
	Node *l;
	Node *r;
	Node *p;
	int v;
};

class SplayTree{
private:
	Node *root;

public:
	void rightRotate(Node *P);
	void leftRotate(Node *P);
	void Splay(Node *T);
	void Insert(int v);
	void Inorder(Node *R);
	Node* Find(int v);
	bool Erase(int v);
	Node* getRoot();
};
