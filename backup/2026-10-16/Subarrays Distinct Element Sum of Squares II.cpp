// Date: 2026-10-16
// Problem: Subarrays Distinct Element Sum of Squares II
// Link: https://leetcode.com/problems/subarrays-distinct-element-sum-of-squares-ii/
// Code:

void lazy_add(int index, int start, int end, long long val,vector<long long>& segtree,vector<long long>& sq_segtree) {
    sq_segtree[index] += 2LL * val * segtree[index] + 1LL * (end - start + 1) * val * val;
    segtree[index] += 1LL * (end - start + 1) * val;
}

long long func(long long a, long long b) {
    return a + b;
}

class SegTree {
public:
    vector<long long> segtree, lazy, sq_segtree;

    void init(int n) {
        segtree.resize(4 * n, 0);
        sq_segtree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }

    long long get() {
        return sq_segtree[0];
    }
    void updaterange(vector<int>& arr, int index, int start, int end,
                     int l, int r, long long val) {
        int mid = (start + end) / 2;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (lazy[index] != 0) {
            lazy_add(index, start, end,lazy[index], segtree, sq_segtree);

            if (start != end) {
                lazy[left] += lazy[index];
                lazy[right] += lazy[index];
            }

            lazy[index] = 0;
        }

        // Out of range
        if (r < start || l > end) {
            return;
        }

        // Completely within range
        if (start >= l && end <= r) {
            lazy_add(index, start, end, val, segtree, sq_segtree);

            if (start != end) {
                lazy[left] += val;
                lazy[right] += val;
            }

            return;
        }

        // Partially included
        updaterange(arr, left, start, mid, l, r, val);
        updaterange(arr, right, mid + 1, end, l, r, val);

        segtree[index] = func(segtree[left], segtree[right]);
        sq_segtree[index] = func(sq_segtree[left], sq_segtree[right]);
    }
};

class Solution {
public:
    int sumCounts(vector<int>& a) {
        const long long MOD = 1e9 + 7;
        int n = a.size();

        SegTree seg;
        seg.init(n);

        vector<int> seen(1e5 + 1, -1);
        long long ans = 0;

        for (int i = 0; i < n; i++) {
            int l = seen[a[i]] + 1;
            int r = i;

            seg.updaterange(a, 0, 0, n - 1, l, r, 1);

            ans = (ans + seg.get()) % MOD;
            seen[a[i]] = i;
        }

        return ans;
    }
};