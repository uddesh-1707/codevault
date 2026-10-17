// Date: 2026-10-17
// Problem: Range Sum Query - Mutable
// Link: https://leetcode.com/problems/range-sum-query-mutable/
// Code:


class NumArray {
public:
    vector<int> segtree;
    int n;

    NumArray(vector<int>& a) {
        n = a.size();
        init(n);
        build(a, 0, 0, n - 1);
    }

    void init(int n) {
        segtree.resize(4 * n, 0);
    }

    void build(vector<int>& a, int index, int start, int end) {
        if (start == end) {
            segtree[index] = a[start];
            return;
        }

        int mid = (start + end) / 2;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        build(a, left, start, mid);
        build(a, right, mid + 1, end);

        segtree[index] = segtree[left] + segtree[right];
    }

    void update(int pos, int val) {
        updateHelper(0, 0, n - 1, pos, val);
    }

    void updateHelper(int index, int start, int end,
                      int pos, int val) {
        if (start == end) {
            segtree[index] = val;
            return;
        }

        int mid = (start + end) / 2;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (pos <= mid) {
            updateHelper(left, start, mid, pos, val);
        } else {
            updateHelper(right, mid + 1, end, pos, val);
        }

        segtree[index] = segtree[left] + segtree[right];
    }

    int sumRange(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }

    int query(int index, int start, int end, int l, int r) {
        if (r < start || l > end) {
            return 0;
        }

        if (l <= start && end <= r) {
            return segtree[index];
        }

        int mid = (start + end) / 2;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        return query(left, start, mid, l, r)
             + query(right, mid + 1, end, l, r);
    }
};
