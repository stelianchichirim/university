#include <bits/stdc++.h>

using namespace std;

struct node {
    int val, B;
    node *vec, *tata, *dr;
    node(int x) {
        val = x;
        B = 0;
        vec = nullptr;
        tata = nullptr;
        dr = nullptr;
    }
};

unordered_map<int, node*> mp; ///for deletion

///merge two binomial trees and return the resulting tree
node* mergeTrees(node* t1, node*t2) {
    if (t1->val > t2->val) swap(t1, t2);
    t2->tata = t1;
    t2->dr = t1->vec;
    t1->vec = t2;
    t1->B++;
    return t1;
}

///merge two binomial heaps and return the resulting heap
vector<node*> mergeHeaps(vector<node*> &h1, vector<node*> &h2) {
    vector<node*> newH;
    int pos1 = 0, pos2 = 0;
    while (pos1 < h1.size() or pos2 < h2.size()) {
        if (pos1 == h1.size()) {
            newH.push_back(h2[pos2]);
            pos2++;
        }
        else if (pos2 == h2.size()) {
            newH.push_back(h1[pos1]);
            pos1++;
        }
        else if (h1[pos1]->val <= h2[pos2]->val) {
            newH.push_back(h1[pos1]);
            pos1++;
        }
        else {
            newH.push_back(h2[pos2]);
            pos2++;
        }
    }
    /// adjust the new heap so that trees are in increasing order of degree and have different degree.
    if (newH.size() <= 1) return newH;
    vector<node*> aux;
    aux.push_back(newH[0]);
    for (int i = 1; i < newH.size(); ++i)
        if (aux.back()->B < newH[i]->B) aux.push_back(newH[i]);
        else if (aux.back()->B == newH[i]->B) aux.back() = mergeTrees(aux.back(), newH[i]);
        else {
            swap(aux.back(),newH[i]);
            aux.push_back(newH[i]);
        }
    return aux;
}


///insert a binomial tree in a binomial heap
void insertTreeInHeap(vector<node*> &h, node* t) {
    vector<node*> aux;
    aux.push_back(t);
    h = mergeHeaps(h, aux);
}

///return the root with minimum value
node* getMin(vector<node*> &h) {
    if (h.size()==0) return nullptr;
    node* ans = h[0];
    for (int i = 1; i < h.size(); ++i)
        if (ans->val > h[i]->val) ans = h[i];
    return ans;
}

///remove the root of a binomial tree and return a binomial heap
vector<node*> removeRootFromTree(node* root) {
    vector<node*> h;
    node *curr = root->vec;
    delete root;
    node *aux;
    while (curr) {
        aux = curr;
        curr = curr->dr;
        aux->dr = nullptr;
        aux->tata = nullptr;
        h.push_back(aux);
    }
    return h;
}

///delete the minimum value from a heap
void deleteMin(vector<node*> &h) {
    node* minRoot = getMin(h);
    if (minRoot == nullptr) return;
    mp[minRoot->val] = nullptr; ///for deletion
    vector<node*> aux;
    for (int i = 0; i < h.size(); ++i)
        if (h[i] != minRoot) aux.push_back(h[i]);
    h = removeRootFromTree(minRoot);
    h = mergeHeaps(aux, h);
}

///delete a node from a heap
void removeNodeFromHeap(vector<node*> &h, node* nod) {
    if(nod == nullptr) return;
    while(nod->tata) {
        swap(nod->val, nod->tata->val);
        mp[nod->val] = nod; ///for deletion
        nod = nod->tata;
    }
    vector<node*> aux;
    for (int i = 0; i < h.size(); ++i)
        if (h[i] != nod) aux.push_back(h[i]);
    h = removeRootFromTree(nod);
    h = mergeHeaps(aux, h);
}

///insert a new value in a heap
void insertVal(vector<node*> &h, int val) {
    node* aux = new node(val);
    mp[val] = aux; ///for deletion
    insertTreeInHeap(h, aux);
}

///build a heap with elements from an array
vector<node*> buildHeap(vector<int> &v) {
    vector<node*> newheap;
    for(int x : v)
        insertVal(newheap, x);
    return newheap;
}

///print the minimum value from a heap
void printMinVal(vector<node*> &h) {
    node* ans = getMin(h);
    if (ans == nullptr) printf("Heap is empty\n");
    else printf("%d\n", ans->val);
}

int main()
{
    freopen("file.in", "r", stdin);
    freopen("file.out", "w", stdout);
    int Q, x, tip;
    scanf("%d", &Q);
    vector<node*> heap;
    for (int i = 1; i <= Q; ++i) {
        scanf("%d", &tip);
        if (tip == 1) {
            scanf("%d", &x);
            insertVal(heap, x);
        }
        else if (tip == 2) {
            scanf("%d", &x);
            if(mp[x] != nullptr) removeNodeFromHeap(heap, mp[x]);
            mp[x] = nullptr;
        }
        else if (tip == 3) printMinVal(heap);
        else deleteMin(heap);
    }
    ///testing the merge and build functions
    /*scanf("%d", &Q);
    vector<int> v;
    for(int i=1;i<=Q;i++)
    {
        scanf("%d",&x);
        v.push_back(x);
    }
    vector<node*> heap1=buildHeap(v);
    v.clear();
    for(int i=1;i<=Q;i++)
    {
        scanf("%d",&x);
        v.push_back(x);
    }
    vector<node*> heap2=buildHeap(v);
    heap1 = mergeHeaps(heap1, heap2);
    for(int i=1;i<=2*Q;i++)
    {
        printMinVal(heap1);
        deleteMin(heap1);
    }*/
    return 0;
}
