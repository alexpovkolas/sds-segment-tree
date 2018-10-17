//
// Created by Aliaksandr Paukalas on 10/17/18.
//
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>


class SegmentTree {

    // TODO: check if clean up possible
    struct Node {
    private:
        int odd_sum;
        int even_sum;
        int odd_count;
        int even_count;
        int value;
    public:

        bool isLeaf;
        Node(): odd_sum(0), even_sum(0), odd_count(0), even_count(0), value(0), isLeaf(false) {}

        Node(int odd_sum, int even_sum, int odd_count, int even_count, int value, bool isLeaf) :
                odd_sum(odd_sum), even_sum(even_sum), odd_count(odd_count), even_count(even_count), value(value), isLeaf(isLeaf) {}

        int get_odd_sum();
        int get_even_sum();
        int get_odd_count();
        int get_even_count();

        int get_value();
        void set_value(int new_value);
    };

    int items_size;
    std::vector<Node> nodes;

    int traversal(bool odd, int index, int cur_l, int cur_r, int l, int r);
    void inc_traversal(int index, int cur_l, int cur_r, int l, int r);
    void set_traversal(int index, int cur_l, int cur_r, int position, int value);
    Node make(int index);
    void push(int index);

public:
    SegmentTree(const std::vector<int> items);
    void set(int index, int value);
    void increment(int left, int right);
    int odd_sum(int left, int right);
    int even_sum(int left, int right);

};

int SegmentTree::Node::get_even_count() {
    if (isLeaf || value == 0) {
        return even_count;
    } else {
        return value % 2 == 0 ? even_count : odd_count;
    }
}

int SegmentTree::Node::get_odd_count() {
    if (isLeaf || value == 0) {
        return odd_count;
    } else {
        return value % 2 == 1 ? even_count : odd_count;
    }
}

int SegmentTree::Node::get_even_sum() {
    if (isLeaf || value == 0) {
        return even_sum;
    } else {
        return value % 2 == 0 ? even_sum + value * even_count : odd_sum + value * odd_count;
    }
}

int SegmentTree::Node::get_odd_sum() {
    if (isLeaf || value == 0) {
        return odd_sum;
    } else {
        return value % 2 == 1 ? even_sum + value * even_count : odd_sum + value * odd_count;
    }
}

int SegmentTree::Node::get_value() {
    return value;
}

void SegmentTree::Node::set_value(int new_value) {
    value = new_value;
    if (isLeaf) {
        if (new_value % 2 == 0) {
            even_sum = new_value;
            odd_sum = 0;
            even_count = 1;
            odd_count = 0;
        } else {
            even_sum = 0;
            odd_sum = new_value;
            even_count = 0;
            odd_count = 1;
        }
    }
}

SegmentTree::SegmentTree(const std::vector<int> items): items_size(items.size()), nodes( pow(2, ceil(log2(items.size())) + 1) ) {
    int k = nodes.size() / 2;
    for (int i = 0; i < k ; ++i) {
        int value = items.size() > i ?  items[i] : 0;
        nodes[i + k] = value % 2 == 1 ? Node(value, 0, 1, 0, value, true) : Node(0, value, 0, 1, value, true);
    }

    for (int i = k - 1; i > 0 ; --i) {
        nodes[i] = make(i);
    }
}

void SegmentTree::set(int index, int value) {
    set_traversal(1, 0, items_size - 1, index, value);
}

void SegmentTree::increment(int left, int right) {
    inc_traversal(1, 0, items_size - 1, left, right);
}

int SegmentTree::odd_sum(int left, int right) {
    return traversal(true, 1, 0, items_size - 1, left, right);;
}

int SegmentTree::even_sum(int left, int right) {
    return traversal(false, 1, 0, items_size - 1, left, right);
}

int SegmentTree::traversal(bool odd, int index, int cur_l, int cur_r, int l, int r) {
    if (l > r)
        return 0;

    if (cur_l == l && cur_r == r) {
        return odd ? nodes[index].get_odd_sum() : nodes[index].get_even_sum();
    }
    push(index);
    int middle = (cur_l + cur_r) / 2;
    return traversal(odd, index * 2, cur_l, middle, l, std::min(r, middle))
           + traversal(odd, index * 2 + 1, middle + 1, cur_r, std::max(l, middle + 1), r);
}

void SegmentTree::inc_traversal(int index, int cur_l, int cur_r, int l, int r) {
    if (l > r)
        return;

    if (cur_l == l && cur_r == r) {
        nodes[index].set_value(nodes[index].get_value() + 1);
        return;
    }

    push(index);

    // TODO: update to have 1 recursion call
    int middle = (cur_l + cur_r) / 2;
    inc_traversal(index * 2, cur_l, middle, l, std::min(r, middle));
    inc_traversal(index * 2 + 1, middle + 1, cur_r, std::max(l, middle + 1), r);

    nodes[index] = make(index);
}

void SegmentTree::set_traversal(int index, int cur_l, int cur_r, int position, int value) {
    if (cur_l == cur_r) {
        nodes[index].set_value(value);
        return;
    }

    push(index);
    int middle = (cur_l + cur_r) / 2;
    if (position <= middle) {
        set_traversal(index * 2, cur_l, middle, position, value);
    } else {
        set_traversal(index * 2 + 1, middle + 1, cur_r, position, value);
    }

    nodes[index] = make(index);
}

void SegmentTree::push(int index) {
    Node current = nodes[index];
    if (current.get_value() != 0) {
        int left = index * 2;
        int right = index * 2 + 1;

        nodes[left].set_value(nodes[left].get_value() + current.get_value());
        nodes[right].set_value(nodes[right].get_value() + current.get_value());
        nodes[index].set_value(0);
        nodes[index] = make(index);
    }
}

SegmentTree::Node SegmentTree::make(int index) {
    Node current = nodes[index];
    if (!current.isLeaf) {
        Node left = nodes[index * 2];
        Node right = nodes[index * 2 + 1];
        return Node(left.get_odd_sum() + right.get_odd_sum(), left.get_even_sum() + right.get_even_sum(),
                    left.get_odd_count() + right.get_odd_count(), left.get_even_count() + right.get_even_count(), current.get_value(), false);
    } else {
        return current;
    }
}

using namespace std;

enum Actions {SET = 1, INCREMENT, EVENT_SUM, ODD_SUM};
struct Operation {
    int action;
    int first_arg;
    int second_arg;
} ;

int main() {
    int n = 0;
    int q = 0;

    cin >> n >> q;
    vector<int> items(n);
    vector<Operation> operations(q);

    for (int i = 0; i < n; ++i) {
        cin >> items[i];
    }

    for (int j = 0; j < q; ++j) {
        Operation operation;
        cin >> operation.action >> operation.first_arg >> operation.second_arg;
        operations[j] = operation;
    }

    SegmentTree tree(items);

    for (int j = 0; j < q; ++j) {
        Operation operation = operations[j];
        switch (operation.action) {
            case SET:
                tree.set(operation.first_arg - 1, operation.second_arg);
                break;
            case INCREMENT:
                tree.increment(operation.first_arg - 1, operation.second_arg - 1);
                break;
            case ODD_SUM:
                cout << tree.odd_sum(operation.first_arg - 1, operation.second_arg - 1) << std::endl;
                break;
            case EVENT_SUM:
                cout << tree.even_sum(operation.first_arg - 1, operation.second_arg - 1) << std::endl;
                break;
        }
    }


    return 0;
}
