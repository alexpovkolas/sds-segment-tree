//
// Created by Aliaksandr Paukalas on 10/15/18.
//

#include "SegmentTree.h"
#include <math.h>
#include <algorithm>

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
        nodes[index].value++;
        if (nodes[index].isLeaf)
            nodes[index] = make(index);
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
        nodes[index].value = value;
        nodes[index] = make(index);
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
    if (current.value != 0) {
        nodes[index * 2].value += current.value;
        nodes[index * 2 + 1].value += current.value;
        nodes[index].value = 0;
    }
}

SegmentTree::Node SegmentTree::make(int index) {
    Node current = nodes[index];
    if (current.isLeaf) {
        return current.value % 2 == 1 ? Node(current.value, 0, 1, 0, current.value, true) : Node(0, current.value, 0, 1, current.value, true);
    } else {
        Node left = nodes[index * 2];
        Node right = nodes[index * 2 + 1];
        return Node(left.get_odd_sum() + right.get_odd_sum(), left.get_even_sum() + right.get_even_sum(),
                    left.get_odd_count() + right.get_odd_count(), left.get_even_count() + right.get_even_count(), current.value, false);
    }
}