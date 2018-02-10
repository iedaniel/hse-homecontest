#include <iostream>

using namespace std;

struct Node_ {
    int val;
    Node_ * next;
    explicit Node_ (int val) : val(val), next(nullptr) {};
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

class List {
public:
    List(Node _a): a(_a) {}

    void push(int val) {
        Node b = new Node_(val);
        b->next = a;
        a = b;
    }

    void pop() {
        Node b = a->next;
        delete a;
        a = b;
    }

    void sort() {
        a = Sort(a);
    }

    void print() {
        Node b = a;
        while (b != nullptr) {
            cout << b->val << ' ';
            b = b->next;
        }
    }

    ~List() {
        while (a != nullptr) {
            pop();
        }
    }
private:
    Node a;
};

int main() {
    int n;
    cin >> n;

    List a(nullptr);
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        a.push(x);
    }

    a.sort();

    a.print();
    return 0;
}
