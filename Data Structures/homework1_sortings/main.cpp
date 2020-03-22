#include <bits/stdc++.h>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

const int MaxNforBubble = 1e4, MaxValforCountSort = 1e7;

// BubbleSort

bool BubbleSort(vector<int>& v) {
    printf("BubbleSort -> ");
    if(v.size() > MaxNforBubble) {
        printf("Nu putem sta pana maine sa ruleze => Numarul de elemente este prea mare\n");
        return false;
    }
    for(int i = 0; i < v.size(); i++)
        for(int j = i + 1; j < v.size(); j++)
            if(v[i] > v[j]) swap(v[i], v[j]);
    return true;
}

// QuickSort

void DoQuickSort(int l, int r, vector<int>& v) {
    int i = l, j = r, pivot = v[l + rng() % (r - l + 1)];
    do {
        while(v[i] < pivot) i++;
        while(v[j] > pivot) j--;
        if(i <= j) {
            swap(v[i], v[j]);
            i++;
            j--;
        }
    } while(i <= j);
    if(l < j) DoQuickSort(l, j, v);
    if(r > i) DoQuickSort(i, r, v);
}

bool QuickSort(vector<int>& v) {
    printf("QuickSort -> ");
    DoQuickSort(0, v.size() - 1, v);
    return true;
}

// MergeSort

vector<int> aux;

void DoMergeSort(int l, int r, vector<int>& v) {
    if(l >= r) return;
    int mid = (l + r) / 2;
    DoMergeSort(l, mid, v);
    DoMergeSort(mid + 1, r, v);
    merge(v.begin() + l, v.begin() + mid + 1, v.begin() + mid + 1, v.begin() + r + 1, aux.begin());
    for(int i = l; i <= r; i++)
        v[i] = aux[i - l];
}

bool MergeSort(vector<int>& v) {
    aux.resize(v.size());
    printf("MergeSort -> ");
    DoMergeSort(0, v.size() - 1, v);
    return true;
}

// CountSort

bool CountSort(vector<int>& v) {
    printf("CountSort -> ");
    vector<int> cnt;
    int Vmax = 0, poz = 0;
    for(int i = 0; i < v.size(); i++)
        if(v[i] > Vmax) Vmax = v[i];
    if(Vmax > MaxValforCountSort) {
        printf("Numerele din sir sunt prea mari => Prea multa memorie\n");
        return false;
    }
    cnt.resize(Vmax+1, 0);
    for(int i = 0; i < v.size(); i++)
        cnt[v[i]]++;
    for(int i = 0; i <= Vmax; i++)
        while(cnt[i] > 0) {
            cnt[i]--;
            v[poz++] = i;
        }
    return true;
}

// RadixSort

const int bucket = 8;
const int lim = (1 << bucket) - 1;

void DoRadixSort(int ind, vector<int>& v) {
    int vaz[lim+1];
    int n = v.size();
    memset(vaz, 0, sizeof(vaz));
    for(int i = 0; i < n; i++)
        vaz[(v[i] >> (ind * bucket)) & lim]++;
    for(int i = 1; i <= lim; i++)
        vaz[i] += vaz[i - 1];
    vector<int> v1;
    v1.resize(v.size());
    for(int i = n - 1; i >= 0; i--)
        v1[--vaz[(v[i] >> (ind * bucket)) & lim]] = v[i];
    for(int i = 0; i < n; i++)
        v[i] = v1[i];
}

bool RadixSort(vector<int>& v) {
    printf("RadixSort -> ");
    for(int i = 0; i < 32 / bucket; i++)
        DoRadixSort(i, v);
    return true;
}

// HeapSort

void heapify(int nod, int n, vector<int>& v) {
    int l = nod * 2 + 1;
    int r = nod * 2 + 2;
    if(l >= n) return;
    if(v[nod] < v[l]) {
        if(r < n && v[r] > v[l]) {
            swap(v[nod], v[r]);
            heapify(r, n, v);
        } else {
            swap(v[nod], v[l]);
            heapify(l, n, v);
        }
    } else if(r < n && v[nod] < v[r]) {
        swap(v[nod], v[r]);
        heapify(r, n, v);
    }
}

bool HeapSort(vector<int>& v) {
    printf("HeapSort -> ");
    int n = v.size();
    for(int i = n / 2 - 1; i >= 0; i--)
        heapify(i, n, v);
    for(int i = n - 1; i >= 0; i--) {
        swap(v[0], v[i]);
        heapify(0, i, v);
    }
    return true;
}

// SortStl

bool SortStl(vector<int>& v) {
    printf("SortStl -> ");
    sort(v.begin(), v.end());
    return true;
}

bool IsSorted(vector<int>& v) {
    for(int i = 0; i < v.size() - 1; i++)
        if(v[i] > v[i + 1]) return false;
    return true;
}

const int val1 = 1e6, val2 = 1e7, val3 = 1e9;
const int NforTests[11] = {10, 1000, 10000, 100000, 100000, 500000, 500000, 1000000, 5000000, 10000000, 50000000};
const int ValforTests[11] = {val1, val1, val1, val1, val3, val1, val2, val1, val1, val2, val2};

vector<int> GenerateTest(int nrTest) {
    int N = NforTests[nrTest];
    int Maxval = ValforTests[nrTest];
    vector<int> v;
    for(int i = 0; i < N; i++)
        v.push_back(rng() % Maxval);
    int Vmax = 0;
    for(int i = 0; i < v.size(); i++)
        Vmax = max(Vmax, v[i]);
    printf("Numere: %d\n", N);
    printf("Valoare maxima: %d\n", Vmax);
    return v;

}

auto sortFunctions = {BubbleSort, QuickSort, MergeSort, CountSort, RadixSort, HeapSort, SortStl};

int main()
{
    freopen("results.txt", "w", stdout);
    int T = 11;
    for(int i = 0; i < T; i++) {
        vector<int> v = GenerateTest(i);
        for(auto sortF : sortFunctions) {
            vector<int> aux = v;
            auto start = chrono::high_resolution_clock::now();
            if(sortF(aux) == true) {
                auto stop = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
                printf("a durat %d milisecunde si ", duration.count());
                if(IsSorted(aux)) printf("a fost sortat crescator\n");
                else printf("nu a fost sortat crescator\n");
            }
        }
        printf("\n");
    }
    return 0;
}
