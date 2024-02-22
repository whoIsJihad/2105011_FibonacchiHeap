#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int key;
    int value;
    int degree;
    bool marked;
    Node *parent;
    Node *child;
    Node *left;
    Node *right;

    char c;

    Node(int k, int v)
    {
        key = k;
        value = v;
        degree = 0;
        marked = false;
        parent = nullptr;
        child = nullptr;
        left = this;
        right = this;
        c = 'n';
    }
    Node *createNode(int key, int value)
    {
        return new Node(key, value);
    }

    void showNode()
    {
        cout << "\nkey : " << key;
        cout << "\nvalue : " << value;
    }
};

class FibonacciHeap
{
    int nodeNum;
    Node *rootList;

public:
    Node *initialise()
    {
        return nullptr;
    }

    Node *createNode(int key, int value)
    {
        return new Node(key, value);
    }

    Node *insert(Node *rootList, Node *x)
    {
        if (rootList != nullptr)
        {
            rootList->left->right = x;
            x->right = rootList;
            x->left = rootList->left;
            rootList->left = x;

            if (rootList->key > x->key)
                rootList = x;
        }

        else
            rootList = x;
        nodeNum++;
        return rootList;
    }

    void linkerChild(Node *rootList, Node *y, Node *z)
    {
        y->left->right = y->right;
        y->right->left = y->left;

        if (rootList->right == rootList)
            rootList = z;

        y->left = y;
        y->right = y;
        y->parent = z;

        if (z->child == nullptr)
        {
            z->child = y;
        }

        y->right = z->child;
        y->left = z->child->left;

        z->child->left->right = y;
        z->child->left = y;

        if (y->key < z->child->key)
        {
            z->child = y;
        }
        z->degree++;
    }

    Node *Union(Node *one, Node *two)
    {
        Node *np;
        Node *H = one;
        H->left->right = two;
        two->left->right = H;
        np = H->left;
        H->left = two->left;
        two->left = np;
        return H;
    }

    int Display(Node *H)
    {
        Node *p = H;
        if (p == NULL)
        {
            cout << "The Heap is Empty" << endl;
            return 0;
        }
        cout << "The root nodes of Heap are: " << endl;

        do
        {
            cout << p->key;
            p = p->right;
            if (p != H)
            {
                cout << "-->";
            }
        } while (p != H && p->right != NULL);
        cout << endl;
    }

    Node *extractMin(Node *rootList)
    {
        Node *p;
        Node *ptr;

        Node* z=rootList;

        p=z;
        ptr=z;

        if(z==nullptr) {
            return z;
        }

        Node* x;
        Node* np;

        x=nullptr;

        if(z->child!=nullptr){
            x=z->child;
        }

        if(x!=nullptr){
            ptr=x;

            do{
                np=x->right;
                rootList->left->right=x;
                x->right=rootList;
                x->left=rootList->left;
                rootList->left=x;

                if(x->key<rootList->key){
                    rootList=x;
                }

                x->parent=nullptr;
                x=np;
            }
            while(np!=ptr);
        }
        z->left->right=z->right;
        z->right->left=z->left;
        rootList=z->right;

        if(z==z->right && z->child==nullptr){
            rootList=nullptr;
        }
        else {
            rootList=z->right;
            Consolidate(rootList);
        }
        nodeNum--;
        return p;

    }  
};

int main()
{
    int n, m, l;

    FibonacciHeap fh;
    Node *p;
    Node *H;

    H = fh.initialise();

    p = fh.createNode(2, 3);

    H = fh.insert(H, p);

    p = fh.createNode(3, 4);

    H = fh.insert(H, p);

    int x = fh.Display(H);
}