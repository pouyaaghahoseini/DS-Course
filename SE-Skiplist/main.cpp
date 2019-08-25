#include <assert.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>

#define HEIGHT 100
int RandomHeight() { return rand() % HEIGHT; }

using namespace std;
class Element {
public:
    string Key;
    double Value;
    Element(string key, double value) : Key(key), Value(value) {}
    Element() {}
    ~Element() {}
    Element &operator=(Element &b){
        Key=b.Key;
        Value=b.Value;
        return *this;
    }
    friend bool operator<(const Element &a, const Element &b) {
        return a.Key < b.Key;
    }

    friend bool operator>(const Element &a, const Element &b) {
        return a.Key > b.Key;
    }

    friend bool operator==(const Element &a, const Element &b) {
        return a.Key == b.Key;
    }

    friend bool operator>=(const Element &a, const Element &b) {
        return a.Key >= b.Key;
    }
};
template <class T> class Array {
protected:
    T *a;

public:
    int length;
    Array(int len);
    Array(int len, T init);
    virtual ~Array();
    Array<T> &operator=(Array<T> &b) {
//        if (a != NULL)
//           delete[] a;
        a = b.a;
        b.a = NULL;
        length = b.length;
        return *this;
    }
    T &operator[](int i) {
        assert(i >= 0 && i < length);
        return a[i];
    }
};
template <class T> Array<T>::Array(int len) {
    length = len;
    a = new T[length];
}
template <class T> Array<T>::Array(int len, T init) {
    length = len;
    a = new T[length];
    for (int i = 0; i < length; i++)
        a[i] = init;
}
template <class T> Array<T>::~Array() {
//    if (a != NULL)
//        delete[] a;
}
///////////////////////////////////////    ArrayDeque //////////////////////////

template <class T> class ArrayDeque {
public:
    Array<T> a;
    int j;
    int n;
    void resize();
    ArrayDeque();
    virtual ~ArrayDeque();
    int size();
    const T &get(int i);
    T set(int i, T x);
    virtual void add(int i, T x);
    virtual T remove(int i);
    virtual void clear();
};
template <class T> inline const T &ArrayDeque<T>::get(int i) {
    return a[(j + i) % a.length];
}
template <class T> inline T ArrayDeque<T>::set(int i, T x) {
    T y = a[(j + i) % a.length];
    a[(j + i) % a.length] = x;
    return y;
}
template <class T> void ArrayDeque<T>::clear() {
    n = 0;
    j = 0;
    Array<T> b(1);
    a = b;
}
template <class T> ArrayDeque<T>::ArrayDeque() : a(1) {
    n = 0;
    j = 0;
}
template <class T> ArrayDeque<T>::~ArrayDeque() {}
template <class T> void ArrayDeque<T>::resize() {
    Array<T> b(max(1, 2 * n));
    for (int k = 0; k < n; k++)
        b[k] = a[(j + k) % a.length];
    a = b;
    j = 0;
}
template <class T> int ArrayDeque<T>::size() { return n; }
template <class T> void ArrayDeque<T>::add(int i, T x) {
    if (n + 1 > a.length)
        resize();
    if (i < n / 2) { // shift a[0],..,a[i-1] left one position
        j = (j == 0) ? a.length - 1 : j - 1;
        for (int k = 0; k <= i - 1; k++)
            a[(j + k) % a.length] = a[(j + k + 1) % a.length];
    } else { // shift a[i],..,a[n-1] right one position
        for (int k = n; k > i; k--)
            a[(j + k) % a.length] = a[(j + k - 1) % a.length];
    }
    a[(j + i) % a.length] = x;
    n++;
}
template <class T> T ArrayDeque<T>::remove(int i) {
    T x = a[(j + i) % a.length];
    if (i < n / 2) { // shift a[0],..,[i-1] right one position
        for (int k = i; k > 0; k--)
            a[(j + k) % a.length] = a[(j + k - 1) % a.length];
        j = (j + 1) % a.length;
    } else { // shift a[i+1],..,a[n-1] left one position
        for (int k = i; k < n - 1; k++)
            a[(j + k) % a.length] = a[(j + k + 1) % a.length];
    }
    n--;
    if (3 * n < a.length)
        resize();
    return x;
}
////////////////////////////////////////////////////////////////////////
template <class T> class BDeque : public ArrayDeque<T> {
    using ArrayDeque<T>::a;
    using ArrayDeque<T>::j;
    using ArrayDeque<T>::n;
public:
    BDeque(int b) {
        n = 0;
        j = 0;
        Array<T> z(b + 1);
        a = z;
    }
    virtual ~BDeque() {}
    // C++ Question: Why is this necessary?
    virtual void add(int i, T x) { ArrayDeque<T>::add(i, x); }
    virtual bool add(T x) {
        ArrayDeque<T>::add(ArrayDeque<T>::size(), x);
        return true;
    }
    //   void resize() {}
};
template <class T> class Node {
public:
    int h;
    BDeque<T> d;
    Node *prev;
    Node **next;
    T Last() { return d.get(d.size()-1); }
    Node(int height, int b);
    ~Node() {}
};
template <class T>
Node<T>::Node(int height, int blocksize) : h(height), d(blocksize) {
    prev = nullptr;
    next = new Node *[h + 1];
    for (int i = 0; i < height; i++)
        next[i] = nullptr;
}
////////////////////////////////////////////////////////////////
template <class T> class SESkiplist {
public:
    Node<T> *sentinel;
    const int maxHeight, b;
    int height;
    SESkiplist(int maxHeight, int blockSize);
    ~SESkiplist() {}
    bool add(T &value);
    bool remove(T &value);
    const T *find(T &value);
    void gather(Node<T> *node);
    bool AddElement(T &value, Node<T> *node);
    bool RemoveElement(T &value, Node<T> *node);
    void RemoveNode(Node<T> *node);
    Node<T> *AddBefore(Node<T> *node, const T &initValue);
    Node<T> *FindPredNode(T &value);
};
template <class T>
SESkiplist<T>::SESkiplist(int maxHeight, int blockSize): maxHeight(maxHeight), b(blockSize), height(0) {
    sentinel = new Node<T>(HEIGHT, blockSize + 1);
    sentinel->prev = sentinel;
    for (int i = HEIGHT; i >= 0; i--) {
        sentinel->next[i] = sentinel;
    }
}
template <class T> void SESkiplist<T>::RemoveNode(Node<T> *w) {
    Node<T> *u = sentinel;
    int r = height;
    int comp = 0;
    while (r >= 0) {
        while (u->next[r] != sentinel && (u->next[r]->Last() < w->Last())) {
            u = u->next[r];
        }
        if (u->next[r] == w) {
            u->next[r] = u->next[r]->next[r];
            if (u == sentinel && u->next[r] == sentinel)
                height--; // skiplist height has gone down
        }
        r--;
    }
}
template <class T> void SESkiplist<T>::gather(Node<T> *w) {
    Node<T> *u = w;
    for (int j = 0; j < b - 1; j++) {
        while (u->d.size() < b)
            u->d.add(u->next[0]->d.remove(0));
        u = u->next[0];
    }
    u->prev->next[0] = u->next[0];
    u->next[0]->prev = u->prev;
    delete u;
}
template <class T> bool SESkiplist<T>::RemoveElement(T &input, Node<T> *w) {
    int j = -1;
    for (int i = 0; i < w->d.size(); i++) {
        if (w->d.get(i) == input) {
            j = i;
            break;
        }
    }
    if (j == -1) {
        return false;
    }
    int r = 0;
    Node<T> *u = w;
    while (r < b && u->next[0] != sentinel && u->d.size() == b - 1) {
        u = u->next[0];
        r++;
    }
    if (r == b) {
        RemoveNode(u->prev);
        gather(w);
    }
    if (u->d.size() == 1) {
        RemoveNode(u);
    }
    u = w;
    u->d.remove(j);
    while (u->d.size() < b - 1 && u->next[0] != sentinel) {
        u->d.add(u->next[0]->d.remove(0));
        u = u->next[0];
    }
    if (u->d.size() == 0) {
        u->prev->next[0] = u->next[0];
        u->next[0]->prev = u->prev;
        delete u;
    }
    return true;
}
template <class T> bool SESkiplist<T>::AddElement(T &input, Node<T> *w) {
    int j = w->d.size();
    for (int i = 0; i < w->d.size(); i++) {
        if (w->d.get(i) == input) {
            return false;
        }
        if (w->d.get(i) > input) {
            j = i;
            break;
        }
    }
    Node<T> *u = w;
    int r = 0;
    for (; r < b; r++) {
        if (b + 1 > u->d.size()) {
            break;
        }
        if (u->next[0] != sentinel) {
            u = u->next[0];
        } else {
            break;
        }
    }

    if (b + 1 == u->d.size()) {
        if (r == b) {
            Node<T> *block = AddBefore(u->next[0], u->d.remove(u->d.size() - 1));
            while (block != w) {
                while (block->d.size() < b) {
                    block->d.add(0, block->prev->d.remove(block->prev->d.size() - 1));
                }
                block = block->prev;
            }
        } else {
            AddBefore(sentinel, u->d.remove(u->d.size() - 1));
        }
    }
    while (u != w) {
        u->d.add(0, u->prev->d.remove(u->prev->d.size() - 1));
        u = u->prev;
    }
    w->d.add(j, input);
    return true;
  }
template <class T>
Node<T> *SESkiplist<T>::AddBefore(Node<T> *w, const T &input) {
    Node<T> *u = new Node<T>(RandomHeight(), b + 1);
    for (int i = 0; i <= u->h; i++) {
        u->next[i] = sentinel;
    }
    u->d.add(input);

    Node<T> *temp = sentinel;
    for (int r = height; r >= 0; r--) {
        while (temp->next[r] != sentinel && temp->next[r]->d.get(0) < u->d.get(0)) {
            temp = temp->next[r];
        }
        if (r <= u->h) {
            temp->next[r] = u;
        }
    }
    for (int r = height + 1; r <= u->h; r++) {
        sentinel->next[r] = u;
        height++;
    }
    u->next[0]= w;
    u->prev = w->prev;
    w->prev->next[0] = u;
    w->prev = u;
    return u;
}
template <class T> bool SESkiplist<T>::add(T &input) {
    Node<T> *node = sentinel;
    for (int r = height; r >= 0; r--) {
        while (node->next[r] != sentinel && node->next[r]->d.size() > b - 1 &&
               node->next[r]->Last() < input) {
            node = node->next[r];
        }
    }
    if (node->next[0] == sentinel) {
        AddBefore(sentinel, input);
        return true;
    }
    else {
        return AddElement(input, node->next[0]);
    }
}
template <class T> bool SESkiplist<T>::remove(T &input) {
    Node<T> *u = sentinel;
    for (int r = height; r >= 0; r--) {
        while (u->next[r] != sentinel && u->next[r]->Last() < input) {
            u = u->next[r];
        }
    }
    if (u->next[0] == sentinel) {
        return false;
    }
    return RemoveElement(input, u->next[0]);
}
template <class T> Node<T> *SESkiplist<T>::FindPredNode(T &input) {
    Node<T> *u = sentinel;
    for (int r = height; r >= 0; r--) {
        while (u->next[r] != sentinel && u->next[r]->Last() < input) {
            u = u->next[r];
        }
    }
    return u;
}
template <class T> const T *SESkiplist<T>::find(T &input) {
    Node<T> *u = FindPredNode(input)->next[0];
    for (int i = 0; i < u->d.size(); i++) {
        if (u->d.get(i) >= input) {
            return &(u->d.get(i));
        }
    }
    return nullptr;
}
//////////////////////////////////////////////////////////////////////
int main() {
    SESkiplist<Element> S(HEIGHT, 100);
    int requests;
    string cmd,key;
    double value;
    Element E;
    string line;
    cin>>requests;
    getline(cin, line);
    for (int i=0 ; i<requests ; i++){
        getline(cin, line);
        istringstream Stream (line);
        Stream >> cmd ;
        Stream >> key;
        if (cmd == "add") {
            Stream >> value;
            Element E(key, value);
            if (S.add(E)) {
                cout << "added" << endl;
            }
            else {
                cout << "already in there" << endl;
            }
        }
        else if (cmd == "remove") {
            Element E(key, 0);
            if (S.remove(E)) {
                cout << "removed" << endl;
            } else {
                cout << "does not exist" << endl;
            }
        } else if (cmd == "find") {
            Element E(key, 0);
            const Element *SearchResult = S.find(E);
            if (SearchResult == nullptr) {
                cout << "not found" << endl;
            } else {
                if (SearchResult->Value == (int)SearchResult->Value) {
                    cout << fixed << setprecision(1) << SearchResult->Value << endl;
                } else {
                   cout << std::defaultfloat << SearchResult->Value << endl;
                }
            }
        }
    }
    return 0;
}
