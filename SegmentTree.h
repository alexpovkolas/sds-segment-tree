//
// Created by Aliaksandr Paukalas on 10/15/18.
//

#ifndef SSD_PROBLEM2_SEGMENTTREE_H
#define SSD_PROBLEM2_SEGMENTTREE_H

#import <vector>

class SegmentTree {

    // TODO: check if clean up possible
    struct Node {
    private:
        int odd_sum;
        int even_sum;
        int odd_count;
        int even_count;
    public:
        int value;
        bool isLeaf;

        Node(): odd_sum(0), even_sum(0), odd_count(0), even_count(0), value(0), isLeaf(false) {}

        Node(int odd_sum, int even_sum, int odd_count, int even_count, int value, bool isLeaf) :
            odd_sum(odd_sum), even_sum(even_sum), odd_count(odd_count), even_count(even_count), value(value), isLeaf(isLeaf) {}

        int get_odd_sum();
        int get_even_sum();
        int get_odd_count();
        int get_even_count();
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


#endif //SSD_PROBLEM2_SEGMENTTREE_H
