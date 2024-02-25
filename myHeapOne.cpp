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
public:
    int nodeNum;
    Node *rootList;

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
    // prints a single tree as required in the question

    void DisplaySingleTree(Node *singleTree)
    {
        cout << "[Parent: " << (singleTree->parent == nullptr ? -1 : singleTree->parent->key) << " , ";

        cout << "(" << singleTree->key << "," << singleTree->value << ")";

        cout << "]";
        Node *p = singleTree;
        if (p == nullptr)
        {
            return;
        }

        Node *ch = p->child;
        if (ch != nullptr)
        {
            cout << "->";

            queue<Node *> q;
            Node *temp = ch;
            do
            {
                cout << "[";
                cout << "Parent: " << (temp->parent == nullptr ? -1 : temp->parent->key) << " , ";
                cout << "(" << temp->key << "," << temp->value << ")";
                cout << "]";
                q.push(temp);
                temp = temp->right;
            } while (temp != ch);
            cout << endl;
            while (!q.empty())
            {
                if (q.front()->child != nullptr)
                {
                    DisplaySingleTree(q.front());
                }
                q.pop();
            }
        }
    }
    // prints the entire heap tree by tree as required in the question
    void DisplayHeap()
    {
        Node *p = rootList;
        int i = 1;
        if (p == nullptr)
        {
            return;
        }
        do
        {
            cout << "Tree " << i++ << ": ";
            DisplaySingleTree(p);
            cout << endl;
            p = p->right;
        } while (p != rootList);
        cout << endl;
    }

    bool Increase_key(int x, int k)
    {
        if (rootList == nullptr)
            return false;

        Node *ptr = Find(x);

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

    Node *FindHelp(Node *H, int k)
    {
        Node *x = H;
        x->c = 'y';
        Node *p = NULL;
        if (x->key == k)
        {
            p = x;
            x->c = 'n';
            return p;
        }

        if (p == NULL)
        {
            if (x->child != nullptr)
            {
                p = FindHelp(x->child, k);
                if (p != nullptr)
                    return p;
            }
            if (x->right != nullptr && (x->right)->c != 'y')
            {
                p = FindHelp(x->right, k);
                if (p != nullptr)
                    return p;
            }
        }

        x->c = 'n';
        return p;
    }
    Node *Find(int k)
    {
        return FindHelp(this->rootList, k);
    }
    Node *extractMax()
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
            // if there is a tree in A[d], link it with x and remove it from A[d]
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
            // if there is a tree in A[i],add it to the root list

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
                    // updating the root list if necessary

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
        // calling Increase_key to make the key maximum
        bool done = Increase_key(k, INT_MAX);
        if (done)
        {
            // calling extractMax to remove the maximum key
            Node *np = extractMax();
        }
        return;
    }

    int maxKey()
    {
        return rootList->key;
    }
    // getter for rootlist
    Node *getRootList()
    {
        return rootList;
    }
    Node *childListOfMax()
    {
        return rootList->child;
    }
    int getNodeNum()
    {
        return nodeNum;
    }
};

//***************
class OfflineFunction
{
public:
    FibonacciHeap make_heap()
    {
        FibonacciHeap fh;
        return fh;
    }

    bool is_empty(FibonacciHeap fh)
    {
        return fh.getNodeNum() == 0;
    }

    void insert(FibonacciHeap &fh, int key, int value)
    {
        fh.insert(key, value);
    }

    int extract_max(FibonacciHeap &fh)
    {
        Node *maxNode = fh.extractMax();
        if (maxNode != nullptr)
        {
            return maxNode->key;
        }
        else
        {
            return -1;
        }
    }

    // returns whether the increase key operation was successful or not

    bool increase_key(FibonacciHeap &fh, int index, int newKey)
    {
        return fh.Increase_key(index, newKey);
    }

    void Delete_(FibonacciHeap &fh, int value)
    {
        fh.Delete(value);
    }

    FibonacciHeap meld(FibonacciHeap &fh1, FibonacciHeap &fh2)
    {
        FibonacciHeap fh;
        fh.rootList = fh.Union(fh1.rootList, fh2.rootList);
        return fh;
    }

    int find_max(FibonacciHeap &fh)
    {
        if (fh.rootList == nullptr)
        {
            return -1;
        }
        return fh.rootList->key;
    }

    void print(FibonacciHeap &fh)
    {        
        fh.DisplayHeap();
    }

    void test()
    {
        FibonacciHeap fh = make_heap();
        cout << (is_empty(fh) ? "Empty" : "Not Empty") << endl;

        // inserting thousand random values from 0 to 2999

        for (int i = 0; i < 1000; i++)
        {
            insert(fh, rand() % 2999, rand());

            // extracted max after every 100 insertions
            if (i && i % 100 == 0)
            {

                int extractedMax = extract_max(fh);
                cout << "Extracted Max: " << extractedMax << endl;
                this->print(fh);
            }
        }

        //attempting to delete hundred random values from 0 to 2999
        for(int i=0;i<100;i++){
            Delete_(fh,rand()%2999);
            cout << "After deleting: " << endl;
            this->print(fh);
        }

        // increasing the key of 100 random values from 0 to 2999
        for(int i=0;i<100;i++){
            increase_key(fh,rand()%2999,rand());
            cout << "After increasing key: " << endl;
            this->print(fh);
            
        }
    }
};

// Main function to test the implementation

int main()
{
    freopen("output.txt", "w", stdout);
    OfflineFunction of;
    srand(time(nullptr));
    FibonacciHeap fh;
    of.test();
}