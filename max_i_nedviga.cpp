#include<bits/stdc++.h>

using namespace std;

#define ll long long

// ДД как map
// Разные типы операций
// Прибавления на отрезке
// https://codeforces.com/group/zij6WrDiuE/contest/288013/problem/A

class Treap {
    static minstd_rand generator;

    struct Node {
        int key, priority, cnt = 1;
        ll value, sum, add = 0;
        Node *l = nullptr, *r = nullptr;
        Node (int key, ll value) : key(key), value(value), priority(generator()), sum(value), cnt(1), add(0) {}
    } *root = nullptr;

    static ll getSum(Node *n) {
        return n ? n->sum + n->add * n->cnt : 0;
    }

    static int getCnt(Node *n) {
        return n ? n->cnt : 0;
    }

    static void update(Node *&n) {
        if(n) {
            n->sum = getSum(n->l) + n->value + getSum(n->r);
            n->cnt = getCnt(n->l) + 1 + getCnt(n->r);
        }
    }

    static void push(Node *&n) {
        if(n && n->add) {
            n->value += n->add;
            n->sum += n->add * n->cnt;
            if(n->l) n->l->add += n->add;
            if(n->r) n->r->add += n->add;
            n->add = 0;
        }
    }

    static Node *merge(Node *a, Node *b) {
        push(a);
        push(b);
        if(!a || !b) return a ? a : b;

        if(a->priority > b->priority) {
            a->r = merge(a->r, b);
            update(a);
            return a;
        } else {
            b->l = merge(a, b->l);
            update(b);
            return b;
        }
    }

    static void split(Node *n, int key, Node *&a, Node *&b) {
        push(n);
        if(!n) {
            a = b = nullptr;
            return;
        }

        if(n->key < key) {
            split(n->r, key, n->r, b);
            a = n;
        } else {
            split(n->l, key, a, n->l);
            b = n;
        }
        update(a);
        update(b);
    }


public:

    void add(int l, int r, int x) {
        Node *less, *equal, *greater;
        split(root, l, less, greater);
        split(greater, r+1, equal, greater);

        if(equal) equal->add += x;
        root = merge(merge(less, equal), greater);
    }

    void insert(int key, ll value) {
        Node *less, *greater;
        split(root, key, less, greater);
        root = merge(merge(less, new Node(key, value)), greater);
    }

    void erase(int l, int r) {
        Node *less, *equal, *greater;
        split(root, l, less, greater);
        split(greater, r+1, equal, greater);

        root = merge(less, greater);
    }

    ll sum(int l, int r) {
        Node *less, *equal, *greater;
        split(root, l, less, greater);
        split(greater, r+1, equal, greater);

        ll ans = getSum(equal);
        root = merge(merge(less, equal), greater);
        return ans;
    }

    int cnt(int l, int r) {
        Node *less, *equal, *greater;
        split(root, l, less, greater);
        split(greater, r+1, equal, greater);

        int ans = getCnt(equal);
        root = merge(merge(less, equal), greater);
        return ans;
    }
};

minstd_rand Treap::generator;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    Treap t;

    for(int i = 0; i < n; i++) {
        string f;
        cin >> f;

        if(f == "Add") {
            int k; ll v; cin >> k >> v;
            t.insert(k, v);
        }

        if(f == "Sell") {
            int l, r;
            cin >> l >> r;
            t.erase(l, r);
        }

        if(f == "Change") {
            int l, r, x;
            cin >> l >> r >> x;
            t.add(l, r, x);
        }

        if(f == "Price") {
            int l, r;
            cin >> l >> r;

            int cnt = t.cnt(l, r);

            if(cnt) cout << t.sum(l, r) / cnt << endl;
            else cout << 0 << endl;
        }
    }
}
