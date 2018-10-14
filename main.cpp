#include <iostream>
#include <vector>
#include "SegmentTree.h"

using namespace std;

enum Actions {SET = 1, INCREMENT, ODD_SUM, EVENT_SUM};
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
                tree.set(operation.first_arg, operation.second_arg);
                break;
            case INCREMENT:
                tree.increment(operation.first_arg, operation.second_arg);
                break;
            case ODD_SUM:
                cout << tree.odd_sum(operation.first_arg, operation.second_arg);
            case EVENT_SUM:
                cout << tree.even_sum(operation.first_arg, operation.second_arg);
        }
    }


    return 0;
}