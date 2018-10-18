//
// Created by Aliaksandr Paukalas on 10/18/18.
//

// template.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
struct vertex {
    long long sum_even;
    long long sum_odd;
    int to_add = 0;
    int count_even;
    int count_odd;
};

vertex get_leaf(int v) {
    vertex res;
    res.to_add = 0;
    if (v % 2 == 0) {
        res.sum_even = v;
        res.sum_odd = 0;
        res.count_even = 1;
        res.count_odd = 0;
    } else {
        res.sum_even = 0;
        res.sum_odd = v;
        res.count_even = 0;
        res.count_odd = 1;
    }
    return res;
}

vertex operation(vertex l, vertex r) {
    vertex res;
    res.sum_even = l.sum_even + r.sum_even;
    res.sum_odd = l.sum_odd + r.sum_odd;
    res.count_even = l.count_even + r.count_even;
    res.count_odd = l.count_odd + r.count_odd;
    return res;
}

void build(vector<vertex> &segment_tree, vector<int> input, int v, int l, int r) {
    if (r == l) {
        segment_tree[v] = get_leaf(input[r]);
    } else {
        int m = l + (r - l) / 2;
        build(segment_tree, input, 2 * v + 1, l, m);
        build(segment_tree, input, 2 * v + 2, m + 1, r);
        segment_tree[v] = operation(segment_tree[2 * v + 1], segment_tree[2 * v + 2]);
    }
    return;
}

void propagate(vector<vertex> &segment_tree, int v, int l, int r) {
    if (segment_tree[v].to_add != 0) {
        if (segment_tree[v].to_add % 2 != 0) {
            int even_before = segment_tree[v].count_even;
            int odd_before = segment_tree[v].count_odd;
            segment_tree[v].count_even = odd_before;
            segment_tree[v].count_odd = even_before;
            long long sum_even_before = segment_tree[v].sum_even;
            long long sum_odd_before = segment_tree[v].sum_odd;
            segment_tree[v].sum_even = sum_odd_before + odd_before * segment_tree[v].to_add;
            segment_tree[v].sum_odd = sum_even_before + even_before * segment_tree[v].to_add;
        } else {
            segment_tree[v].sum_even = segment_tree[v].sum_even + segment_tree[v].count_even * segment_tree[v].to_add;
            segment_tree[v].sum_odd = segment_tree[v].sum_odd + segment_tree[v].count_odd * segment_tree[v].to_add;
        }
        if (l != r) {
            segment_tree[2 * v + 1].to_add += segment_tree[v].to_add;
            segment_tree[2 * v + 2].to_add += segment_tree[v].to_add;
        }
        segment_tree[v].to_add = 0;
    }
}

void modify(vector<vertex> &segment_tree, int v, int l, int r, int pos, int new_val) {
    propagate(segment_tree, v, l, r);
    if (l == r) {
        if (new_val % 2 != 0) {
            segment_tree[v].sum_even = 0;
            segment_tree[v].sum_odd = new_val;
            segment_tree[v].count_even = 0;
            segment_tree[v].count_odd = 1;
        } else {
            segment_tree[v].sum_even = new_val;
            segment_tree[v].sum_odd = 0;
            segment_tree[v].count_even = 1;
            segment_tree[v].count_odd = 0;
        }
    } else {
        int m = l + (r - l) / 2;
        if (pos <= m) {
            modify(segment_tree, v * 2 + 1, l, m, pos, new_val);
            propagate(segment_tree, v * 2 + 2, m + 1, r);
        } else {
            modify(segment_tree, v * 2 + 2, m + 1, r, pos, new_val);
            propagate(segment_tree, v * 2 + 1, l, m);
        }
        segment_tree[v] = operation(segment_tree[2 * v + 1], segment_tree[2 * v + 2]);
    }
}

void interval_modify_by(vector<vertex> &segment_tree, int v, int mod_l, int mod_r, int l, int r, int modify_by) {
    propagate(segment_tree, v, l, r);
    if (mod_l > r || mod_r < l) {
        return;
    }
    if (mod_l <= l && mod_r >= r) {
        segment_tree[v].to_add += modify_by;
        propagate(segment_tree, v, l, r);
        return;
    }
    int m = l + (r - l) / 2;
    interval_modify_by(segment_tree, 2 * v + 1, mod_l, mod_r, l, m, modify_by);
    interval_modify_by(segment_tree, 2 * v + 2, mod_l, mod_r, m + 1, r, modify_by);
    segment_tree[v] = operation(segment_tree[2 * v + 1], segment_tree[2 * v + 2]);
}

long long get_sum_odd(vector<vertex> &segment_tree, int v, int get_l, int get_r, int l, int r) {
    propagate(segment_tree, v, l, r);
    if (l > r) {
        return 0;
    }
    if (get_l > r || get_r < l) {
        return 0;
    }
    if (get_l <= l && get_r >= r) {
        return segment_tree[v].sum_odd;
    }
    int m = l + (r - l) / 2;
    return get_sum_odd(segment_tree, 2 * v + 1, get_l, get_r, l, m) +
           get_sum_odd(segment_tree, 2 * v + 2, get_l, get_r, m + 1, r);
}

long long get_sum_even(vector<vertex> &segment_tree, int v, int get_l, int get_r, int l, int r) {
    propagate(segment_tree, v, l, r);
    if (l > r) {
        return 0;
    }
    if (get_l > r || get_r < l) {
        return 0;
    }
    if (get_l <= l && get_r >= r) {
        return segment_tree[v].sum_even;
    }
    int m = l + (r - l) / 2;
    return get_sum_even(segment_tree, 2 * v + 1, get_l, get_r, l, m) +
           get_sum_even(segment_tree, 2 * v + 2, get_l, get_r, m + 1, r);
}

int main() {
    int n = 0;
    int request_numbers = 0;
    cin >> n >> request_numbers;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    vector<vertex> segment_tree(4 * n);
    build(segment_tree, a, 0, 0, n - 1);
    vector<vector<int>> params;

    for (int i = 0; i < request_numbers; i++) {
        vector<int> p(3);
        cin >> p[0] >> p[1] >> p[2];
        params.push_back(p);
    }

    using milli = std::chrono::milliseconds;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < request_numbers; i++) {
        int req = params[i][0];
        int s = params[i][1];
        int t = params[i][2];
        if (req == 1) {
            modify(segment_tree, 0, 0, n - 1, s - 1, t);
        } else if (req == 2) {
            interval_modify_by(segment_tree, 0, s - 1, t - 1, 0, n - 1, 1);
        } else if (req == 3) {
            cout << get_sum_even(segment_tree, 0, s - 1, t - 1, 0, n - 1) << endl;
        } else if (req == 4) {
            cout << get_sum_odd(segment_tree, 0, s - 1, t - 1, 0, n - 1) << endl;
        }

    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "myFunction() took "
              << std::chrono::duration_cast<milli>(finish - start).count()
              << " milliseconds\n";
    return 0;
}