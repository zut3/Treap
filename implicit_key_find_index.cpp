#include<bits/stdc++.h>

using namespace std;

class Treap {
    static minstd_rand generator;

    struct Node {
        int value, priority, size = 1;
        Node *l = nullptr, *r = nullptr, *p = nullptr;
        Node(int value): value(value), priority(generator()) {}
    } *root = nullptr;

    map<int, Node*> location;

    static int getSize(Node *n) {
        return n ? n->size : 0;
    }

    static void setParent(Node *&n, Node *p) {
        if(n) {
            n->p = p;
        }
    }

    static void update(Node *&n) {
        if(n) {
            n->size = getSize(n->l) + 1 + getSize(n->r);
            setParent(n->l, n);
            setParent(n->r, n);
        }
    }

    static int getIndex(Node *n) {
        int index = getSize(n->l);
        while(n->p) {
            if(n->p->r == n) {
                index += getSize(n->p->l) + 1;
            }
            n = n->p;
        }
        return index;
    }

    static Node *merge(Node *a, Node *b) {
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

    static void split(Node* n, int k, Node *&a, Node *&b) {
        if(!n) {
            a = b = nullptr;
            return;
        }

        if(getSize(n->l) < k) {
            split(n->r, k - getSize(n->l) - 1, n->r, b);
            a = n;
        } else {
            split(n->l, k, a, n->l);
            b = n;
        }
        update(a);
        update(b);

        setParent(a, nullptr);
        setParent(b, nullptr);
    }

public:

    void push_back(int value) {
        location[value] = new Node(value);
        root = merge(root, location[value]);
    }

    void to_back(int l, int r) {
        Node *less, *equal, *greater;
        split(root, l, less, greater);
        split(greater, r - l + 1, equal, greater);

        root = merge(merge(less, greater), equal);
    }

    void erase(int id) {
        Node *less, *equal, *greater;
        split(root, id, less, greater);
        split(greater, 1, equal, greater);

        if(equal) location.erase(equal->value);

        root = merge(merge(less, equal), greater);
    }

    int get(int id) {
        Node *less, *equal, *greater;
        split(root, id, less, greater);
        split(greater, 1, equal, greater);

        int ans = equal->value;
        root = merge(merge(less, equal), greater);

        return ans;
    }

    int find(int value) {
        if(!location.count(value)) return -1;
        return getIndex(location[value]);
    }
};

minstd_rand Treap::generator;

int main() {
    int n, q;
    cin >> n >> q;

    Treap t;
    for(int i = 0; i < n; i++) {
        t.push_back(i);
    }

    for(int i = 0; i < q; i++) {
        string f; cin >> f;
        if(f == "shuffle") {
            int l, r; cin >> l >> r;
            t.to_back(l, r);
        }
        if(f == "find") {
            int v; cin >> v;
            cout << t.find(v) << endl;
        }
    }

}
