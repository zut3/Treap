#include<bits/stdc++.h>

using namespace std;

class Treap {
    static minstd_rand generator;

    struct Node {
        int value, priority, size = 1;
        Node *l = nullptr, *r = nullptr;
        Node(int value): value(value), priority(generator()) {}
    } *root = nullptr;

    static int getSize(Node *n) {
        return n ? n->size : 0;
    }

    static void update(Node *&n) {
        if(n) {
            n->size = getSize(n->l) + 1 + getSize(n->r);
        }
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

    // Первые k попадут в a, остальные - в b
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
    }

public:

    void push_back(int value) {
        root = merge(root, new Node(value));
    }

    void to_front(int l, int r) {
        Node *less, *equal, *greater;
        split(root, l, less, greater);
        split(greater, r - l + 1, equal, greater);

        root = merge(equal, merge(less, greater));
    }

    int get(int id) {
        Node *less, *equal, *greater;
        split(root, id, less, greater);
        split(greater, 1, equal, greater);

        int ans = equal->value;
        root = merge(merge(less, equal), greater);

        return ans;
    }
};

minstd_rand Treap::generator;

int main() {
    int n, q;
    cin >> n >> q;

    Treap t;
    for(int i = 1; i <= n; i++) {
        t.push_back(i);
    }

    for(int i = 0; i < q; i++) {
        int l, r; cin >> l >> r; l--; r--;

        t.to_front(l, r);
    }

    for(int i = 0; i < n; i++) cout << t.get(i) << ' ';
    cout << endl;
}
