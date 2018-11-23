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

/*
int main()
{
	SplayTree arbolito;
	while(true)
	{
		int t;
		scanf("%d",&t);
		if(t!=0 && t!=-1) arbolito.Insert(t);
		else if(t==0)
		{
			scanf("%d",&t);
			if(!arbolito.Find(t))
				printf("Couldn't Find %d!\n",t);
			else
				printf("Found %d!\n",t);
		}
		else
		{
			scanf("%d",&t);
			if(arbolito.Erase(t))
				printf("Deleted %d!\n",t);
			else
				printf("Couldn't Find %d!\n",t);
		}
		if(arbolito.getRoot()) printf("root: %d\n",arbolito.getRoot()->v);
		arbolito.Inorder(arbolito.getRoot());
	}
}
*/
