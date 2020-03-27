#include <bits/stdc++.h>

using namespace std;

int l,ok,n,m,x,y,q,t,nr;
char s[155],sf[310],vaz[310][155],c;
vector<pair<int,char> > v[310];
queue<pair<string,int> > g;
vector<string> ans;
map<pair<int,string>, char> mp;

void go(int nod,int pos)
{
    if(pos==l+1)
    {
        if(sf[nod]) ok=1;
        return;
    }
    vaz[nod][pos]=1;
    for(int i=0;i<v[nod].size();i++)
        if(v[nod][i].second==s[pos])
        {
            int vec=v[nod][i].first;
            if(!vaz[vec][pos+1]) go(vec,pos+1);
        }
}

void solve1()
{
    while(nr--)
    {
        scanf("\n%s",s+1);
        l=strlen(s+1);
        for(int i=0;i<=n;i++)
            for(int j=1;j<=l+1;j++) vaz[i][j]=0;
        ok=0;
        go(q,1);
        if(ok) printf("1\n");
        else printf("0\n");
    }
}

void solve2()
{
    printf("Primele 100 cele mai scurte:\n");
    g.push({"",q});
    while(!g.empty())
    {
        int nod=g.front().second;
        string s1=g.front().first;
        if(sf[nod])
        {
            ans.push_back(s1);
            if(ans.size()==100) break;
        }
        g.pop();
        for(int i=0;i<v[nod].size();i++)
        {
            string aux=s1+v[nod][i].second;
            int vec=v[nod][i].first;
            if(mp[{vec,aux}]==0)
            {
                mp[{vec,aux}]=1;
                g.push({aux,vec});
            }
        }
    }
    for(int i=0;i<ans.size();i++)
    {
        for(int j=0;j<ans[i].size();j++) printf("%c",ans[i][j]);
        printf("\n");
    }
}

void citire()
{
    scanf("%d%d%d",&n,&m,&t);
    scanf("%d",&q);
    for(int i=1;i<=t;i++)
    {
        scanf("%d",&x);
        sf[x]=1;
    }
    for(int i=1;i<=m;i++)
    {
        scanf("%d%d %c\n",&x,&y,&c);
        v[x].push_back({y,c});
    }
    scanf("%d",&nr);
    solve1();  // cerinta 1
    // cerinta 2
    solve2();
}

int main()
{
    freopen("file.in","r",stdin);
    freopen("file.out","w",stdout);
    citire();
    return 0;
}
