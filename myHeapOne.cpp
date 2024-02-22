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
    FibonacciHeap()
    {
        nodeNum = 0;
        rootList = nullptr;
    }
    // inserting key value pair
    void insert(int key, int value)
    {
        Node *x = new Node(key, value);
        if (rootList != nullptr)
        {
            rootList->left->right = x;
            x->right = rootList;
            x->left = rootList->left;
            rootList->left = x;

            if (rootList->key < x->key)
                rootList = x;
        }

        else
            rootList = x;
        nodeNum++;
    }
    // adds y to the child list of z
    void linkerChild(Node *y, Node *z)
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

        if (y->key > z->child->key)
        {
            z->child = y;
        }
        z->degree++;
    }
    // joins two circular doubly linked lists
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
    // prints the value of all child of a particular node
    void DisplayChild(Node *rr)
    {
        Node *p = rr;
        if (p == nullptr)
            return;
        do
        {
            cout << "(" << p->key << ", " << p->value << ")";
            p = p->right;
            if (p != rr)
            {
                cout << " , ";
            }
        } while (p != rr && p->right != nullptr);
        cout << endl;
    }
    //prints a single tree as required in the question

    void DisplaySingleTree(Node *singleTree)
    {

        Node *p = singleTree;
        if (p == nullptr)
        {
            return;
        }

        Node *ch = p->child;
        cout << "(" << p->key << ", " << p->value << ")";
        if (ch != nullptr)
        {
            cout << "->";
            DisplayChild(ch);
        }
        //for each child of p call DisplaySingleTree

        Node * ch1=ch;
        //do while loop to traverse through all the children of p by ch1 until all child is traversed   
        do
        {
            DisplaySingleTree(ch1);
            ch1=ch1->right;
        } while (ch1 != ch);
    }
    //prints the entire heap tree by tree as required in the question
    void DisplayHeap(){
        Node *p = rootList;
        if (p == nullptr)
        {
            return;
        }
        do
        {
            DisplaySingleTree(p);
            p = p->right;
        } while (p != rootList);
    }

    bool Increase_key(int x, int k)
    {
        if (rootList == nullptr)
            return false;

        Node *ptr = Find(rootList, x);

        if (ptr == nullptr || ptr->key > k)
            return false;

        ptr->key = k;

        Node *y = ptr->parent;

        if (y && y->key < ptr->key)
        {
            Cut(ptr, y);
            Cascade(y);
        }

        if (ptr->key > rootList->key)
            rootList = ptr;

        return true;
    }

    // cuts the link between x and x's parent (y), and adds x to the root list
    void Cut(Node *x, Node *y)
    {
        if (x == x->right)
        {
            y->child = nullptr;
        }

        (x->left)->right = x->right;
        (x->right)->left = x->left;

        if (x == y->child)
        {
            y->child = x->right;
        }
        y->degree--;
        x->right = x->left = x;

        rootList->left->right = x;
        x->right = rootList;
        x->left = rootList->left;
        rootList->left = x;
        x->parent = nullptr;
        x->marked = false;
    }

    void Cascade(Node *y)
    {
        Node *z = y->parent;
        if (z)
        {
            if (!y->marked)
            {
                y->marked = true;
            }
            else
            {
                Cut(y, z);
                Cascade(z);
            }
        }
    }

    Node *Find(Node *H, int k)
    {
        Node *x = H;
        x->c = 'y';
        Node *p = nullptr;

        if (x->key == k)
        {
            p = x;
            x->c = 'n';
            return p;
        }
        if (p == nullptr)
        {
            if (x->child)
            {
                p = Find(x->child, k);
            }
            if (x->right->c != 'y')
            {
                p = Find(x->right, k);
            }
        }
        x->c = 'n';
        return p;
    }

    Node *extractMin()
    {
        Node *p, *ptr, *z = rootList;
        p = ptr = z;

        if (z == nullptr)
        {
            return z;
        }

        Node *x;
        Node *np;

        x = nullptr;

        if (z->child != nullptr)
        {
            x = z->child;
        }

        if (x != nullptr)
        {
            // setting ptr equals to child of z
            ptr = x;

            // adding all child of z to rootlist
            do
            {
                np = x->right;
                rootList->left->right = x;
                x->right = rootList;
                x->left = rootList->left;
                rootList->left = x;
                // updating the key if necessary
                if (x->key > rootList->key)
                {
                    rootList = x;
                }

                x->parent = nullptr;
                // updating x
                x = np;
            } while (np != ptr);
        }

        z->left->right = z->right;
        z->right->left = z->left;
        rootList = z->right;

        if (z == z->right && z->child == nullptr)
        {
            rootList = nullptr;
        }
        else
        {
            rootList = z->right;
            Consolidate();
        }
        nodeNum--;
        return p;
    }

    void Consolidate()
    {
        int D = log2(nodeNum) + 1;
        Node *A[D];

        for (int i = 0; i < D; i++)
        {
            A[i] = nullptr;
        }

        Node *x = rootList;
        Node *start = rootList;

        do
        {
            Node *next = x->right;
            int d = x->degree;

            while (A[d] != nullptr)
            {
                Node *y = A[d];

                if (x->key < y->key)
                {
                    swap(x, y);
                }

                linkerChild(y, x);

                A[d] = nullptr;
                d++;
            }

            A[d] = x;
            x = next;

        } while (x != start);

        rootList = nullptr;

        for (int i = 0; i < D; i++)
        {
            if (A[i] != nullptr)
            {
                if (rootList == nullptr)
                {
                    rootList = A[i];
                    rootList->left = rootList->right = rootList;
                }
                else
                {
                    A[i]->left = rootList->left;
                    rootList->left->right = A[i];
                    A[i]->right = rootList;
                    rootList->left = A[i];
                    if (A[i]->key > rootList->key)
                    {
                        rootList = A[i];
                    }
                }
            }
        }
    }

    void Delete(int k)
    {
        bool done = Increase_key(k, INT_MIN);
        if (done)
        {
            Node *np = extractMin();
        }
        return;
    }
};

int main()
{
    Node *np = new Node(0, 0);
    cout << np->key << " " << np->value;
    cout << endl;

    cout << np << endl;
    cout << np->left << " " << np->right << endl;
}