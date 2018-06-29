#include <bits/stdc++.h>

using namespace std;

int n, m, k, s, t;
vector<int> dij;

struct edg {
    int to, cp, pr, f, bc, num;
};

vector<vector<edg> > gr;

void addedg(int u, int v, int p, int num) {
    edg e1 = {u, 1, p, 0, gr[u].size(), num};
    edg e2 = {v, 0, -p, 0, gr[v].size(), num};

    gr[v].push_back(e1);
    gr[u].push_back(e2);

    e1 = {v, 1, p, 0, gr[v].size(), num};
    e2 = {u, 0, -p, 0, gr[u].size(), num};

    gr[u].push_back(e1);
    gr[v].push_back(e2);
}

vector<pair<int, int> > pred;

void dijkstra() {
    vector<int> newdij(n, 1e9);
    newdij[s] = 0;
    set<pair<int, int> > st;
    st.insert({0, s});
    pred.assign(n, {0, 0});

    while (!st.empty()) {
        int v = (*st.begin()).second;
        st.erase(st.begin());

        for (int i = 0; i < gr[v].size(); ++i) {
            if (gr[v][i].cp == 0)
                continue;

            int u = gr[v][i].to;
            int phi = dij[u] - dij[v];
            if (newdij[v] + gr[v][i].pr + phi < newdij[u]) {
                st.erase({newdij[u], u});
                newdij[u] = newdij[v] + gr[v][i].pr + phi;
                st.insert({newdij[u], u});
                pred[u] = {v, i};
            }
        }
    }
    dij = newdij;
}

int findMin() {
    dijkstra();
    if (dij[t] >= 1e9)
        return 0;
    int v = t, cmin = 1e9;
    while (v) {
        cmin = min(cmin, gr[pred[v].first][pred[v].second].cp);
        v = pred[v].first;
    }

    v = t;

    while (v) {
        gr[pred[v].first][pred[v].second].f += cmin;
        gr[pred[v].first][pred[v].second].cp -= cmin;

        gr[v][gr[pred[v].first][pred[v].second].bc].f -= cmin;
        gr[v][gr[pred[v].first][pred[v].second].bc].cp += cmin;

        v = pred[v].first;
    }
    return cmin;
}

void printAns() {
    int ans = 0;
    vector<vector<pair<int, int> > > anspath;
    for (int tt = 0; tt < k; ++tt) {
        vector<pair<int, int> > path;
        vector<char> used(n, false);
        int v = s;
        while (v != t) {
            used[v] = true;
            for (int i = 0; i < gr[v].size(); ++i) {
                if (gr[v][i].f <= 0)
                    continue;

                if (used[gr[v][i].to]) {
                    while (gr[path.back().first][path.back().second].to != gr[v][i].to)
                        path.pop_back();
                } else {
                    path.push_back({v, i});
                }

                v = gr[v][i].to;
                break;
            }
        }

        for (int i = 0; i < path.size(); ++i) {
            ans += gr[path[i].first][path[i].second].pr;
            --gr[path[i].first][path[i].second].f;
        }

        anspath.push_back(path);
    }
    cout << fixed << setprecision(5) << 1.0 * ans / k << '\n';
    for (int i = 0; i < k; ++i) {
        cout << anspath[i].size() << ' ';
        for (int j = 0; j < anspath[i].size(); ++j) {
            cout << gr[anspath[i][j].first][anspath[i][j].second].num << ' ';
        }
        cout << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m >> k;

    s = 0;
    t = n - 1;

    dij.resize(n, 1e9);
    gr.resize(n);
    for (int i = 0; i < m;) {
        int u, v, p;
        cin >> u >> v >> p;
        --u, --v;
        addedg(u, v, p, ++i);
    }

    int res = 0;
    while (res < k) {
        int cur = findMin();
        res += cur;
        if (cur == 0)
            break;
    }

    if (res >= k) {
        printAns();
    } else {
        cout << -1 << '\n';
    }
    return 0;
}
