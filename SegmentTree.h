//
// Created by Aliaksandr Paukalas on 10/15/18.
//

#ifndef SSD_PROBLEM2_SEGMENTTREE_H
#define SSD_PROBLEM2_SEGMENTTREE_H

#import <vector>

class SegmentTree {
    std::vector<int> items;

public:
    SegmentTree(const std::vector<int> items);
    void set(int index, int value);
    void increment(int left, int right);
    int odd_sum(int left, int right);
    int even_sum(int left, int right);

};


#endif //SSD_PROBLEM2_SEGMENTTREE_H
