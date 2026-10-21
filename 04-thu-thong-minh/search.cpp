// Cốt Đê – Chess Code 04: Thử thông minh (cây quyết định, quay lui, vét cạn)
// Video: <link YouTube>
// Biên dịch: g++ -O2 -std=c++20 search.cpp -o chay
// Quy ước ô: sq = file + rank * 8 (a1 = 0, h8 = 63)

#include <algorithm>
#include <bit>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

// ---------- 1. Cân tìm con tốt nhẹ: mỗi lần cân có 3 kết quả ----------
int minWeighings(int pawns) {
    int w = 0, canTell = 1;           // w lần cân phân biệt được 3^w con
    while (canTell < pawns) { canTell *= 3; w++; }
    return w;
}

// ---------- 2–3. Tám quân hậu: mỗi hàng một hậu, quay lui ----------
int col[8];                            // col[row] = cột của hậu ở hàng row
long long tries = 0;                   // số lần thử đặt hậu vào một ô
vector<vector<int>> solutions;

bool safe(int row, int c) {
    for (int r = 0; r < row; r++)
        if (col[r] == c || abs(col[r] - c) == row - r) return false;   // cùng cột hoặc cùng đường chéo
    return true;
}

void placeQueen(int row) {
    if (row == 8) { solutions.push_back(vector<int>(col, col + 8)); return; }
    for (int c = 0; c < 8; c++) {
        tries++;
        if (safe(row, c)) {
            col[row] = c;
            placeQueen(row + 1);       // đặt tiếp hàng sau; quay về đây là "lùi một hàng"
        }
    }
}

// Coi xoay và lật là một: lấy dạng nhỏ nhất trong 8 phép đối xứng
vector<int> canonical(vector<int> s) {
    vector<int> best = s;
    for (int t = 0; t < 8; t++) {
        vector<int> cur(8);
        for (int r = 0; r < 8; r++) {
            int x = s[r], y = r;                       // ô (cột x, hàng y)
            for (int k = 0; k < t % 4; k++) { int nx = 7 - y, ny = x; x = nx; y = ny; }  // xoay 90°
            if (t >= 4) x = 7 - x;                      // lật
            cur[y] = x;
        }
        best = min(best, cur);
    }
    return best;
}

// ---------- 4. Vét cạn: mấy quân hậu thì đánh được mọi ô ----------
uint64_t queenCover[64];               // ô bị đánh + chính ô đứng (bàn trống)

int main() {
    for (int n : {8, 9, 10, 27, 28})
        cout << n << " con tot: " << minWeighings(n) << " lan can\n";

    long long all8 = 1;
    for (int i = 0; i < 8; i++) all8 *= 8;
    long long perm = 1;
    for (int i = 2; i <= 8; i++) perm *= i;
    long long c64_8 = 1;
    for (int i = 0; i < 8; i++) c64_8 = c64_8 * (64 - i) / (i + 1);
    cout << "dat 8 hau vao 64 o: " << c64_8 << " cach; moi hang mot hau: " << all8 << "; them moi cot mot hau: " << perm << "\n";

    auto t0 = chrono::steady_clock::now();
    placeQueen(0);
    double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
    set<vector<int>> distinct;
    for (auto& s : solutions) distinct.insert(canonical(s));
    cout << "quay lui: " << solutions.size() << " nghiem, " << distinct.size() << " neu bo xoay lat, "
         << tries << " lan thu, " << ms << " ms\n";

    int dirs[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (int s = 0; s < 64; s++) {
        uint64_t m = 1ULL << s;
        for (auto& d : dirs) {
            int f = s % 8 + d[0], r = s / 8 + d[1];
            while (f >= 0 && f < 8 && r >= 0 && r < 8) { m |= 1ULL << (f + r * 8); f += d[0]; r += d[1]; }
        }
        queenCover[s] = m;
    }
    long long four = 0, tried4 = 0;
    for (int a = 0; a < 64; a++) for (int b = a + 1; b < 64; b++) for (int c = b + 1; c < 64; c++)
        for (int d = c + 1; d < 64; d++) {
            tried4++;
            if ((queenCover[a] | queenCover[b] | queenCover[c] | queenCover[d]) == ~0ULL) four++;
        }
    cout << "4 hau: thu " << tried4 << " cach, phu kin " << four << " cach\n";

    t0 = chrono::steady_clock::now();
    long long five = 0, tried5 = 0;
    for (int a = 0; a < 64; a++) for (int b = a + 1; b < 64; b++) for (int c = b + 1; c < 64; c++)
        for (int d = c + 1; d < 64; d++) {
            uint64_t abcd = queenCover[a] | queenCover[b] | queenCover[c] | queenCover[d];
            for (int e = d + 1; e < 64; e++) {
                tried5++;
                if ((abcd | queenCover[e]) == ~0ULL) five++;
            }
        }
    ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
    cout << "5 hau: thu " << tried5 << " cach, phu kin " << five << " cach, " << ms << " ms\n";
}
