// Date: 2026-07-26
// Problem: Largest Integer With Given Digit Sum
// Link: https://leetcode.com/problems/largest-integer-with-given-digit-sum/
// Code:

class Solution {
public:
    int largestInteger(int n, int s) {
        if (n == 1) {
            for (int i = 9; i >= 0; i--) {
                if (i == s)
                    return i;
            }
        } else if (n == 2) {
            for (int i = 9; i >= 0; i--) {
                for (int j = 9; j >= 0; j--) {
                    if (i + j == s) {
                        return i * 10 + j;
                    }
                }
            }
        } else if (n == 3) {
            for (int i = 9; i >= 0; i--) {
                for (int j = 9; j >= 0; j--) {
                    for (int k = 9; k >= 0; k--) {
                        if (i + j + k == s) {
                            return i * 100 + j * 10 + k;
                        }
                    }
                }
            }
        } else if (n == 4) {
            for (int i = 9; i >= 0; i--) {
                for (int j = 9; j >= 0; j--) {
                    for (int k = 9; k >= 0; k--) {
                        for (int l = 9; l >= 0; l--) {
                            if (i + j + k + l == s) {
                                return i * 1000 + j * 100 + k * 10 + l;
                            }
                        }
                    }
                }
            }
        } else {
            for (int i = 9; i >= 0; i--) {
                for (int j = 9; j >= 0; j--) {
                    for (int k = 9; k >= 0; k--) {
                        for (int l = 9; l >= 0; l--) {
                            for (int m = 9; m >= 0; m--) {
                                if (i + j + k + l + m == s) {
                                    return i * 10000 + j * 1000 + k * 100 +
                                           l * 10 + m;
                                }
                            }
                        }
                    }
                }
            }
        }
        return -1;
    }
};