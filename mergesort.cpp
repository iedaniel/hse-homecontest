#include <iostream>

using namespace std;

struct Node_ {
    int val;
    Node_ * next;
    Node_ (int val = 0) : val(val), next(nullptr) {};
};
typedef Node_ * Node;

Node Merge(Node a, Node b) {
    if (a == nullptr)
        return b;
    if (b == nullptr)
        return a;
    if (b->val < a->val) {
        b->next = Merge(b->next, a);
        return b;
    }
    a->next = Merge(a->next, b);
    return a;
}

Node cut(Node a) {
    if (a == nullptr || a->next == nullptr)
        return nullptr;
    Node tmp = a->next;
    a->next = tmp->next;
    tmp->next = cut(tmp->next);
    return tmp;
}

Node Sort(Node a) {
    if (a == nullptr || a->next == nullptr)
        return a;
    Node b = cut(a);
    return Merge(Sort(a), Sort(b));
}

int main() {
    int n;
    cin >> n;
    Node a = nullptr;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        Node b = new Node_(x);
        b->next = a;
        a = b;
    }

    a = Sort(a);
    while (a != nullptr) {
        cout << a->val << ' ';
        Node b = a->next;
        delete a;
        a = b;
    }

    return 0;
}
