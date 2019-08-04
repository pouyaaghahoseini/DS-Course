#include <iostream>
using namespace std;
template<class T>
class Array {
public:
    T *a;
    int length;
    Array(int len);
    virtual ~Array();
    Array<T>& operator=(Array<T> &b) {
        if (a != NULL) delete[] a;
        a = b.a;
        b.a = NULL;
        length = b.length;
        return *this;
    }
    T& operator[](int i) {
//        assert(i >= 0 && i < length);
        return a[i];
    }
    void swap(int i, int j) {
        T x = a[i];
        a[i] = a[j];
        a[j] = x;
    }
};

template<class T>
Array<T>::Array(int len) {
    length = len;
    a = new T[length];
}

template<class T>
Array<T>::~Array() {
//    if (a != NULL) delete[] a;
}

template<class T>
class ArrayQueue {
public:
    Array<T> a;
    int j;
    int n;
    void resize();
    ArrayQueue();
    virtual ~ArrayQueue();
    virtual bool add(T x);
    virtual T remove();
    int size();
    T get(int i);
    T set(int i, T x);
    void print(){
        printX();
        printY();
    }
    void printX(){
        for(int i=0; i<n ; i++)
            cout<<a[(j+i)%a.length]->x <<" ";
        cout<<endl;
    }
    void printY(){
        for(int i=0; i<n ; i++)
            cout<<a[(j+i)%a.length]->y <<" ";
        cout<<endl;
    }
    void SortByX(){
        for(int i=0 ; i<(n-1) ; i++)
            for(int j=0 ; j<(n-i-1) ; j++)
                if (get(j)->x > get(j+1)->x)
                    a.swap(j,j+1);
    }
    void SortByY(){
        for(int i=0 ; i<(n-1) ; i++)
            for(int j=0 ; j<(n-i-1) ; j++)
                if (get(j)->y > get(j+1)->y)
                    a.swap(j,j+1);
    }
    void clear(){
        while (n>0)
            remove();
    }
};

template<class T>
ArrayQueue<T>::ArrayQueue() : a(1) {
    n = 0;
    j = 0;
}

template<class T>
ArrayQueue<T>::~ArrayQueue() {
//    delete a;
}

template<class T> inline
T ArrayQueue<T>::get(int i) {
    return a[(j + i) % a.length];
}

template<class T> inline
T ArrayQueue<T>::set(int i, T x) {
    T y = a[(j + i) % a.length];
    a[(j + i) % a.length] = x;
    return y;
}

template<class T>
void ArrayQueue<T>::resize() {
    Array<T> b(max(1, 2*n));
    for (int k = 0; k < n; k++)
        b[k] = a[(j+k)%a.length];
    a = b;
    j = 0;
}

template<class T>
bool ArrayQueue<T>::add(T x) {
    if (n + 1 > a.length) resize();
    a[(j+n) % a.length] = x;
    n++;
    return true;
}

template<class T>
T ArrayQueue<T>::remove() {
    T x = a[j];
    j = (j + 1) % a.length;
    n--;
    if (a.length >= 3*n) resize();
    return x;
}

template<class T>
int ArrayQueue<T>::size() {
    return n;
}


class Node{
public:
    float x;
    float y;
    Node *auxilary;
    Node *left;
    Node *right;
    Node *parent;
    Node() {
        left = right = parent = auxilary = NULL;
    }
};
Node* MakeTree(Node * v){
    ArrayQueue< Node* > listy;
    Node *u = v, *prev = NULL, *next;
    while (u != NULL) {
        if(u->left==NULL & u->right==NULL){
            Node* temporary = new Node();
            temporary->x=u->x;
            temporary->y=u->y;
            listy.add(temporary);
        }

        if (prev == u->parent) {
            if (u->left != NULL) next = u->left;
            else if (u->right != NULL) next = u->right;
            else next = u->parent;
        } else if (prev == u->left) {
            if (u->right != NULL) next = u->right;
            else next = u->parent;
        } else {
            next = u->parent;
        }
        prev = u;
        u = next;
    }

    listy.SortByY();

    while (listy.size()>0){
        int count=listy.size();
        for (int i=0; i<count && i+1<count; i+=2){
            Node *predecessor = listy.get(i);
            while (predecessor->right!=NULL){
                predecessor=predecessor->right;
            }
            Node *temp = new Node ();
            listy.get(i)->parent = temp;
            listy.get(i+1)->parent = temp;
            temp->x=predecessor->x;
            temp->y=predecessor->y;
            temp->left=listy.get(i);
            temp->right=listy.get(i+1);
            listy.add(temp);
        }
        if(count%2==0){
            for (int i=0; i<count; i++)
                listy.remove();
        }
        else if(count%2==1){
            for (int i=0; i<count-1; i++) {
                listy.remove();
            }
            listy.add(listy.get(0));
            listy.remove();
        }
        if(count==1){
            return listy.get(0);
        }
    }

}
Node *preProcess(ArrayQueue <Node*> listx ) {
    while (listx.size() > 0) {
        int count = listx.size();
        for (int i = 0; i < count && i + 1 < count; i += 2) {
            Node *predecessor = listx.get(i);
            while (predecessor->right != NULL) {
                predecessor = predecessor->right;
            }
            Node *temp = new Node();
            listx.get(i)->parent = temp;
            listx.get(i + 1)->parent = temp;
            temp->x = predecessor->x;
            temp->y = predecessor->y;
            temp->left = listx.get(i);
            temp->right = listx.get(i + 1);
            temp->auxilary = MakeTree(temp);
            listx.add(temp);
        }
        if (count % 2 == 0) {
            for (int i = 0; i < count; i++)
                listx.remove();
        } else if (count % 2 == 1) {
            for (int i = 0; i < count - 1; i++) {
                listx.remove();
            }
            listx.add(listx.get(0));
            listx.remove();
        }
        if (count == 1) {
            return listx.get(0);
        }
    }
}

class Range{
public:
    Node * root;
    ArrayQueue< Node* > final;
    Range(Node * x){
        root=x;
    }
    void searchArea( float x1 , float y1 , float x2 , float y2);
    void findy(Node * root , float y1 , float y2 );
};
void Range::findy(Node *root, float y1, float y2) {
    Node  * greater;
    Node  * lower;
    Node  * split;
    split=root;
    while (split!=NULL){
        if(split->y<y1)
            split=split->right;
        if(split->y>y2)
            split=split->left;
        else
            break;
    }
    if(split==NULL)
        return;
    if (split->right==NULL && split->left==NULL){
        if(split->y>=y1 && split->y<=y2)
            final.add(split);
        return;
    }
    greater=split->right;
    lower=split->left;
    while (lower->left!=NULL){
        if(lower->y<y1)
            lower=lower->right;
        else{
            if(lower->right->right!=NULL){
                Node *u = lower->right , *prev = lower, *next;
                while (u != lower) {

                    if(u->left==NULL && u->right==NULL && u->y>=y1 && u->y<=y2 ){
                        final.add(u);
                    }
                    if (prev == u->parent) {
                        if (u->left != NULL) next = u->left;
                        else if (u->right != NULL) next = u->right;
                        else next = u->parent;
                    } else if (prev == u->left) {
                        if (u->right != NULL) next = u->right;
                        else next = u->parent;
                    } else {
                        next = u->parent;
                    }
                    prev = u;
                    u = next;
                }
            }
            else {
                if(lower->right->y<=y2 && y1<=lower->right->y)
                    final.add(lower->right);
            }
            if(y1==lower->y)
                break;
            lower=lower->left;
        }
    }
    if(lower->y>=y1 && lower->y<=y2)
        final.add(lower);

    if(greater->right==NULL && greater->left==NULL) {
        if (greater->y >= y1 && greater->y <= y2 )
            final.add(greater);
    }
    while (greater->right!=NULL){
        if(greater->y>y2)
            greater=greater->left;
        else{
            if(greater->left->left!=NULL && greater->right->right!=NULL){
                Node *u = greater->left , *prev = greater, *next;
                while (u != greater) {

                    if(u->left==NULL && u->right==NULL && u->y>=y1 && u->y<=y2 ){
                        final.add(u);
                    }
                    if (prev == u->parent) {
                        if (u->left != NULL) next = u->left;
                        else if (u->right != NULL) next = u->right;
                        else next = u->parent;
                    } else if (prev == u->left) {
                        if (u->right != NULL) next = u->right;
                        else next = u->parent;
                    } else {
                        next = u->parent;
                    }
                    prev = u;
                    u = next;
                }
            }
            else if(greater->left->left!=NULL && greater->right->right==NULL){
                Node *u = greater->left , *prev = greater, *next;
                while (u != greater) {

                    if(u->left==NULL && u->right==NULL && u->y>=y1 && u->y<=y2 ){
                        final.add(u);
                    }
                    if (prev == u->parent) {
                        if (u->left != NULL) next = u->left;
                        else if (u->right != NULL) next = u->right;
                        else next = u->parent;
                    } else if (prev == u->left) {
                        if (u->right != NULL) next = u->right;
                        else next = u->parent;
                    } else {
                        next = u->parent;
                    }
                    prev = u;
                    u = next;
                }
                if(greater->right->y<=y2 && y1<=greater->right->y)
                    final.add(greater->right);
            }
            else {
                if(greater->right->y<=y2 && y1<=greater->right->y)
                    final.add(greater->right);
                if(greater->left->y<=y2 && y1<=greater->left->y)
                    final.add(greater->left);
            }
            greater=greater->right;
        }
    }
}
void Range::searchArea(float x1, float y1, float x2, float y2) {
    Node  * greater;
    Node  * lower;
    Node  * split;
    if (x1==x2 && y1==y2)
        return;
    split=root;
    while (split!=NULL){
        if(split->x<x1)
            split=split->right;
        if(split->x>x2)
            split=split->left;
        else
            break;
    }
    greater=split->right;
    lower=split->left;
    while (lower->left!=NULL){
        if(lower->x<x1)
            lower=lower->right;
        else{
            if(lower->right->right!=NULL){
                findy(lower->right->auxilary, y1 , y2);
            }
            else {
                if(lower->right->x<=x2 && x1<=lower->right->x && lower->right->y >= y1 && lower->right->y <=y2)
                    final.add(lower->right);
            }
            if(x1==lower->x)
                break;
            lower=lower->left;
        }
    }
    if(lower->x>=x1 && lower->x<=x2 && lower->y >=y1 && lower->y <=y2)
        final.add(lower);

    if(greater->right==NULL && greater->left==NULL){
        if(greater->x >=x1 && greater->x <=x2 && greater->y >=y1 && greater->y <=y2)
            final.add(greater);
    }
    while (greater->right!=NULL){
        if(greater->x>x2)
            greater=greater->left;
        else{
            if(greater->left->left!=NULL && greater->right->right!=NULL){
                findy(greater->left->auxilary,y1,y2);
            }
            else if(greater->left->left!=NULL && greater->right->right==NULL){
                if(greater->right->x >=x1 && greater->right->x <= x2 && greater->right->y >=y1 && greater->right->y <=y2)
                    final.add(greater->right);
                findy(greater->left->auxilary,y1,y2);
            }
            else {
                if(greater->right->x<=x2 && x1<=greater->right->x && greater->right->y >=y1 && greater->right->y <=y2)
                    final.add(greater->right);
                if(greater->left->x<=x2 && x1<=greater->left->x && greater->left->y<=y2 && y1<=greater->left->y)
                    final.add(greater->left);
            }
            greater=greater->right;
        }
    }
}
int main() {
    int points;
    float a, b;
    cin >> points;
    ArrayQueue<float> x;
    ArrayQueue<float> y;
    for (int i = 0; i < points; i++) {
        cin >> a;
        x.add(a);
    }
    for (int i = 0; i < points; i++) {
        cin >> b;
        y.add(b);
    }
    ArrayQueue<Node *> listx;
    for (int i = 0; i < points; i++) {
        Node *temp = new Node();
        temp->x = x.a[i];
        temp->y = y.a[i];
        listx.add(temp);
    }
    listx.SortByX();
    Node *root = new Node();
    root=preProcess(listx);
    Range R(root);
    float x1,x2,y1,y2;
    int counter;
    cin>>counter;
    for(int i=0 ; i<counter ; i++) {
        cin >> x1 >> y1 >> x2 >> y2;
        R.searchArea(x1, y1, x2, y2);
        R.final.SortByY();
        if(R.final.size()==0)
            cout<<"None"<<endl;
        else
            R.final.print();
        R.final.clear();
    }
    return 0;
}

