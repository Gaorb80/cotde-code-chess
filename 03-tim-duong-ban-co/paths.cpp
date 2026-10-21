// Cốt Đê – Chess Code 03: Tìm đường trên bàn cờ
// Video: <link YouTube>
// Biên dịch: g++ -O2 -std=c++20 paths.cpp -o chay
// Quy ước ô: sq = file + rank * 8 (a1 = 0, h8 = 63)

#include <cstdint>
#include <iostream>
#include <queue>
using namespace std;

const int KNIGHT[8][2] = {{1, 2}, {2, 1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {-1, -2}, {-2, -1}};
bool onBoard(int f, int r) { return 0 <= f && f < 8 && 0 <= r && r < 8; }

// BFS: số nước ít nhất của quân mã từ ô start tới mọi ô
void knightDistances(int start, int dist[64]) {
    for (int i = 0; i < 64; i++) dist[i] = -1;
    queue<int> q;
    dist[start] = 0;
    q.push(start);
    while (!q.empty()) {
        int sq = q.front();
        q.pop();                                     // lấy ô chờ lâu nhất ra trước
        for (auto& s : KNIGHT) {
            int f = sq % 8 + s[0], r = sq / 8 + s[1];
            if (!onBoard(f, r)) continue;
            int next = f + r * 8;
            if (dist[next] == -1) {                  // đánh dấu ngay lúc bỏ vào hàng đợi
                dist[next] = dist[sq] + 1;
                q.push(next);
            }
        }
    }
}

int main() {
    int dist[64];
    knightDistances(3 + 3 * 8, dist);               // mã ở d4
    int layer[8] = {0};
    for (int i = 0; i < 64; i++) layer[dist[i]]++;
    cout << "ma tu d4, so o moi lop:";
    for (int k = 0; k < 8 && layer[k]; k++) cout << " " << layer[k];
    cout << "\n";
    knightDistances(0, dist);                        // mã ở a1
    cout << "ma a1 -> h8: " << dist[63] << " nuoc, a1 -> b2: " << dist[9] << " nuoc\n";

    // Xe chỉ đi lên hoặc sang phải: số đường = ô dưới + ô trái
    long long ways[8][8] = {};
    for (int r = 0; r < 8; r++)
        for (int f = 0; f < 8; f++) {
            if (r == 0 && f == 0) { ways[r][f] = 1; continue; }
            ways[r][f] = (r > 0 ? ways[r - 1][f] : 0) + (f > 0 ? ways[r][f - 1] : 0);
        }
    cout << "xe a1 -> c3: " << ways[2][2] << ", d4: " << ways[3][3] << ", e8: " << ways[7][4] << ", h8: " << ways[7][7] << "\n";

    // Tốt đi lên một ô: thẳng hoặc chéo; số đường = tổng ba ô bên dưới
    auto pawn = [](int file, int rank, int targetFile) {
        long long w[8][8] = {};
        w[rank][file] = 1;
        for (int r = rank + 1; r < 8; r++)
            for (int f = 0; f < 8; f++)
                for (int d = -1; d <= 1; d++)
                    if (f + d >= 0 && f + d < 8) w[r][f] += w[r - 1][f + d];
        long long back = 0;
        for (int f = 0; f < 8; f++) back += w[7][f];
        return pair{targetFile < 0 ? back : w[7][targetFile], back};
    };
    cout << "tot d4 -> d8: " << pawn(3, 3, 3).first << ", tot d5 -> hang 8: " << pawn(3, 4, -1).first << "\n";

    // Tượng đi chéo lên, mỗi nước đổi hướng: cần nhớ hướng vừa đi
    // ways2[r][f][d]: số đường tới ô (f, r) mà nước cuối đi theo hướng d (0: chéo phải, 1: chéo trái)
    auto bishop = [](int file) {
        long long w[8][8][2] = {};
        long long total = 0;
        for (int r = 0; r < 8; r++)
            for (int f = 0; f < 8; f++) {
                long long arrive[2] = {0, 0};
                for (int d = 0; d < 2; d++) {
                    int df = d == 0 ? 1 : -1;
                    for (int k = 1; k <= r; k++) {
                        int pf = f - df * k, pr = r - k;
                        if (pf < 0 || pf > 7) break;
                        long long from = 0;
                        if (pr == 0 && pf == file) from += 1;       // nước đầu tiên đi từ ô xuất phát
                        from += w[pr][pf][1 - d];                    // nước trước phải là hướng kia
                        arrive[d] += from;
                    }
                }
                w[r][f][0] = arrive[0];
                w[r][f][1] = arrive[1];
                if (r == 7) total += arrive[0] + arrive[1];
            }
        return pair{w[7][6][0] + w[7][6][1], total};
    };
    auto [toG8, fromB1] = bishop(1);
    long long whiteAll = 0;
    for (int f = 1; f < 8; f += 2) whiteAll += bishop(f).second;   // ô trắng hàng 1: b1, d1, f1, h1
    cout << "tuong b1 -> g8: " << toG8 << ", b1 -> hang 8: " << fromB1 << ", moi o trang hang 1 -> hang 8: " << whiteAll << "\n";
}
