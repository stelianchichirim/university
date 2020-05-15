#include <bits/stdc++.h>

using namespace std;

const char LAMBDA = '#';
const int A_SIZE = 26;

struct edge {
    int vec;
    char ch;
};

class FiniteAutomaton {
private:
    vector< vector<edge> > g;
    vector<bool> finalState;
    int initialState, N;
    vector<char> vaz;
    void lambdaDfs(int nod) {
        vaz[nod] = 1;
        for (auto x : g[nod])
            if (!vaz[x.vec] && x.ch == LAMBDA) lambdaDfs(x.vec);
    }
    void dfs(int nod, vector<char> &marked, vector< vector<edge> > &graph) {
        marked[nod] = 1;
        for (auto x : graph[nod])
            if (!marked[x.vec]) dfs(x.vec, marked, graph);
    }
public:
    FiniteAutomaton(int n, int start) {
        g.resize(n + 1);
        finalState.resize(n + 1, false);
        vaz.resize(n + 1, 0);
        initialState = start;
        N = n;
    }
    void addEdge(int x, int y, char ch) {
        g[x].push_back({y, ch});
    }
    void addFinalState(int x) {
        finalState[x] = true;
    }
    void PrintAutomaton() {
        printf("Number of states = %d\n", N);
        printf("Initial state is = %d\n", initialState);
        printf("Final states are: ");
        for (int i = 1; i <= N; ++i)
            if(finalState[i]) printf("%d ",i);
        printf("\nThe transitions are:\n");
        for (int i = 1; i <= N; ++i)
            for (auto x : g[i])
                printf("%d %d %c\n", i, x.vec, x.ch);
        printf("\n");
    }
    FiniteAutomaton lambdaNfaToNfa() {
        FiniteAutomaton auxFa(N, initialState);
        for (int i = 1; i <= N; ++i) {
            fill(vaz.begin(), vaz.end(), 0);
            lambdaDfs(i);
            map<pair<int, char>, char> mp;
            bool isFinal = false;
            for (int j = 1; j <= N; ++j)
                if (vaz[j]) {
                    if (finalState[j]) isFinal = true;
                    for (auto x : g[j])
                        if (x.ch != LAMBDA && mp[{x.vec, x.ch}] == 0) {
                                auxFa.addEdge(i, x.vec, x.ch);
                                mp[{x.vec, x.ch}] = 1;
                        }
                }
            mp.clear();
            if(isFinal) auxFa.addFinalState(i);
        }
        return auxFa;
    }
    FiniteAutomaton nfaToDfa() {
        FiniteAutomaton auxFa((1<<N), (1<<(initialState-1)));
        vector<int> vecmask;
        vecmask.resize(A_SIZE, 0);
        for (int mask = 1; mask < (1<<N); ++mask) {
            fill(vecmask.begin(), vecmask.end(), 0);
            bool isFinal = false;
            for (int i = 0; i < N; ++i)
                if(mask & (1<<i)) {
                    if (finalState[i + 1]) isFinal = true;
                    for (auto x : g[i + 1])
                        vecmask[x.ch - 'a'] |= (1<<(x.vec - 1));
                }
            if(isFinal) auxFa.addFinalState(mask);
            for (int i = 0; i < A_SIZE; ++i)
                if(vecmask[i] > 0) auxFa.addEdge(mask, vecmask[i], 'a' + i);
        }
        vecmask.clear();
        return auxFa;
    }
    FiniteAutomaton dfaToMinimalDfa() {
        vector<char> reachFromInitial(N + 1, 0);
        vector<char> reachFromFinal(N + 1, 0);
        dfs(initialState, reachFromInitial, g);
        vector< vector<edge> > gt;
        vector<int> dfaVec[A_SIZE];
        for (int i = 0; i < A_SIZE; ++i) dfaVec[i].resize(N + 1, 0);
        gt.resize(N + 1);
        for (int i = 1; i <= N; ++i)
            for (auto x : g[i]) {
                gt[x.vec].push_back({i,x.ch});
                dfaVec[x.ch - 'a'][i] = x.vec;
            }
        for (int i = 1; i <= N; ++i)
            if (finalState[i]) dfs(i, reachFromFinal, gt);
        fill(vaz.begin(), vaz.end(), 0);
        for (int i = 1; i <= N; ++i)
            if (reachFromFinal[i] && reachFromInitial[i]) vaz[i] = 1;
        vector<int> eqClass(N + 1, 0);
        vector<int> neweqClass(N + 1, 0);
        vector< vector<int> > q(1);
        vector< vector<int> > newq;
        for (int i = 1; i <= N; ++i)
            if (vaz[i] && finalState[i]) q.back().push_back(i), eqClass[i] = q.size();
        if(q.back().size() > 0) q.push_back({});
        for (int i = 1; i <= N; ++i)
            if (vaz[i] && !finalState[i]) q.back().push_back(i), eqClass[i] = q.size();
        if(q.back().size() == 0) q.pop_back();
        vector<int> difeq;
        while(newq.size() != q.size()) {
            for (int i = 0; i < q.size(); ++i) {
                newq.push_back({q[i][0]});
                neweqClass[q[i][0]] = newq.size();
                difeq.clear();
                for (int j = 1; j < q[i].size(); ++j) {
                    bool ok = true;
                    for (int k = 0; k < A_SIZE; ++k)
                        if (eqClass[dfaVec[k][q[i][0]]] != eqClass[dfaVec[k][q[i][j]]]) ok = false;
                    if (ok) {
                        newq.back().push_back(q[i][j]);
                        neweqClass[q[i][j]] = newq.size();
                    }
                    else {
                        difeq.push_back(q[i][j]);
                        neweqClass[q[i][j]] = newq.size() + 1;
                    }
                }
                if (difeq.size() > 0) newq.push_back(difeq);
            }
            if (q.size() == newq.size()) break;
            q = newq;
            newq.clear();
            eqClass = neweqClass;
        }
        FiniteAutomaton auxFa(q.size(), eqClass[initialState]);
        map<pair<int, char>, char> mp;
        for (int i = 0; i < q.size(); ++i) {
            for (int nod : q[i]) {
                if (finalState[nod]) auxFa.addFinalState(i + 1);
                for (auto x : g[nod])
                    if (eqClass[x.vec] > 0 && mp[{eqClass[x.vec], x.ch}] == 0) {
                        auxFa.addEdge(i + 1, eqClass[x.vec], x.ch);
                        mp[{eqClass[x.vec], x.ch}] = 1;
                    }
            }
            mp.clear();
        }
        return auxFa;
    }
};

void readRegularGrammar() {
    int Q, N = 1;  ///1 is final state
    vector<pair<pair<int, int>, int> > edges;
    map<char, int> mp;
    char s[10], c;
    scanf("%d", &Q);
    while (Q--) {
        scanf("\n%c %s", &c, s);
        int l = strlen(s), nod, vec = 1;
        if (mp[c] == 0) N++, mp[c] = N;
        nod = mp[c];
        if (l == 2) {
            if (mp[s[1]] == 0) N++, mp[s[1]] = N;
            vec = mp[s[1]];
        }
        edges.push_back({{nod, vec}, s[0]});
    }
    FiniteAutomaton fa(N, mp['S']);
    fa.addFinalState(1);
    for (int i = 0; i < edges.size(); ++i)
        fa.addEdge(edges[i].first.first, edges[i].first.second, edges[i].second);
    //fa.PrintAutomaton();
    fa = fa.lambdaNfaToNfa();
    //fa.PrintAutomaton();
    fa = fa.nfaToDfa();
    fa = fa.dfaToMinimalDfa();
    fa.PrintAutomaton();
}

void dfaToRegex() {
    int N, M, K, S, x, y;
    char c;
    string dp[50][50][50];
    scanf("%d%d%d", &N, &M, &K);
    scanf("%d", &S);
    vector<bool> finalState(N + 1);
    for (int i = 0; i < K; ++i) {
        scanf("%d", &x);
        finalState[x] = 1;
    }
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= N; ++j)
            if (i == j) dp[i][j][0] = "#";
            else dp[i][j][0] = "@";
    for (int i = 0; i< M; ++i) {
        scanf("\n%d %d %c", &x, &y, &c);
        dp[x][y][0] = dp[x][y][0] + " + " + c;
    }
    for (int k = 1; k <= N; ++k)
        for (int i = 1; i <= N; ++i)
            for (int j = 1; j <= N; ++j)
                dp[i][j][k] = "(" + dp[i][j][k-1] + ") + (" + dp[i][k][k-1] + "(" + dp[k][k][k-1] + ")*" + dp[k][j][k-1] + ")";
    string ans = "@";
    for (int i = 1; i <= N; ++i)
        if (finalState[i]) ans = ans + " + (" + dp[S][i][N] + ")";
    cout << ans << "\n";
}

int main()
{
    freopen("file.in", "r", stdin);
    freopen("file.out", "w", stdout);
    readRegularGrammar();
    dfaToRegex();
    return 0;
}
