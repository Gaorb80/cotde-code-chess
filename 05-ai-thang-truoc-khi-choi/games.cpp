// Cốt Đê – Chess Code 05: Máy biết ai thắng trước khi chơi
// Video: <link YouTube>
// Biên dịch: g++ -O2 -std=c++20 games.cpp -o chay
// "Thế thua" = người phải đi tiếp sẽ thua nếu đối thủ chơi đúng.

#include <iostream>
#include <string>
#include <vector>
using namespace std;

string nameOf(int file, int rank) { return string(1, 'a' + file) + to_string(rank + 1); }

// Đánh dấu thế thua từ a1 ra: ô thua khi mọi nước đi đều tới ô thắng
// moves: các hướng được đi (đi bao nhiêu ô cũng được), đều hướng về a1
void solve(const vector<pair<int, int>>& moves, bool losing[8][8]) {
    for (int sum = 0; sum <= 14; sum++)                  // xét ô gần a1 trước
        for (int f = 0; f < 8; f++) {
            int r = sum - f;
            if (r < 0 || r > 7) continue;
            bool canReachLosing = false;
            for (auto [df, dr] : moves) {
                int x = f + df, y = r + dr;
                while (x >= 0 && y >= 0) {
                    if (losing[y][x]) canReachLosing = true;
                    x += df;
                    y += dr;
                }
            }
            losing[r][f] = !canReachLosing;              // a1: không còn nước nào → người phải đi đã thua
        }
}

int main() {
    // 1. Xe chỉ đi xuống hoặc sang trái
    bool rook[8][8] = {};
    solve({{-1, 0}, {0, -1}}, rook);
    cout << "xe, the thua:";
    for (int r = 0; r < 8; r++) for (int f = 0; f < 8; f++) if (rook[r][f]) cout << " " << nameOf(f, r);
    cout << "\n";

    // 2. Hậu đi xuống, sang trái hoặc chéo xuống trái (Wythoff)
    bool queen[8][8] = {};
    solve({{-1, 0}, {0, -1}, {-1, -1}}, queen);
    cout << "hau, the thua:";
    for (int r = 0; r < 8; r++) for (int f = 0; f < 8; f++) if (queen[r][f]) cout << " " << nameOf(f, r);
    cout << "\nhau o h5: " << (queen[4][7] ? "the thua, nguoi di truoc thua" : "the thang") << "\n";

    // 3. Nim: XOR bằng 0 là thế thua
    int a = 5, b = 3, c = 2;
    cout << "5 xor 3 xor 2 = " << (a ^ b ^ c) << "; boc 4 o dong 5: 1 xor 3 xor 2 = " << (1 ^ 3 ^ 2) << "\n";
    vector<int> piles = {7, 5, 3};
    int x = piles[0] ^ piles[1] ^ piles[2];
    cout << "7 xor 5 xor 3 = " << x << "; nuoc thang:";
    for (int i = 0; i < 3; i++) {
        int target = piles[i] ^ x;                       // đống i còn lại bao nhiêu thì XOR về 0
        if (target < piles[i]) cout << " dong " << piles[i] << " -> " << target;
    }
    cout << "\n";

    // 4. Northcott: khoảng trống giữa hai quân trên mỗi hàng là một đống sỏi
    int red[3] = {0, 0, 0}, green[3] = {6, 4, 3};        // hàng 4, 5, 6: đỏ ở cột a; xanh ở g4, e5, d6
    int gaps[3], g = 0;
    for (int i = 0; i < 3; i++) { gaps[i] = green[i] - red[i] - 1; g ^= gaps[i]; }
    cout << "Northcott khoang trong: " << gaps[0] << ", " << gaps[1] << ", " << gaps[2] << "; xor = " << g << "\n";
}
