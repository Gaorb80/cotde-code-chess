// Cốt Đê – Chess Code 02: 64 ô, 64 bit
// Video: <link YouTube>
// Biên dịch: g++ -O2 -std=c++20 bitboard.cpp -o chay
// Quy ước ô: sq = file + rank * 8 (a1 = 0 là bit 0, h8 = 63 là bit 63)

#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
using namespace std;

uint64_t bitOf(int file, int rank) { return 1ULL << (file + rank * 8); }
bool onBoard(int f, int r) { return 0 <= f && f < 8 && 0 <= r && r < 8; }

// Ô bị đánh của quân nhảy, dạng bitboard
uint64_t leapAttacks(int file, int rank, const int steps[][2], int n) {
    uint64_t b = 0;
    for (int i = 0; i < n; i++) {
        int f = file + steps[i][0], r = rank + steps[i][1];
        if (onBoard(f, r)) b |= bitOf(f, r);
    }
    return b;
}

// Ô bị đánh của quân trượt: dừng ở ô có quân đầu tiên (ô đó vẫn bị đánh)
uint64_t slideAttacks(int file, int rank, const int dirs[][2], int n, uint64_t occupied) {
    uint64_t b = 0;
    for (int i = 0; i < n; i++) {
        int f = file + dirs[i][0], r = rank + dirs[i][1];
        while (onBoard(f, r)) {
            b |= bitOf(f, r);
            if (occupied & bitOf(f, r)) break;
            f += dirs[i][0];
            r += dirs[i][1];
        }
    }
    return b;
}

const int KNIGHT[8][2] = {{1, 2}, {2, 1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {-1, -2}, {-2, -1}};
const int KING[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const int ROOK[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
const int BISHOP[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

int main() {
    // Hạt thóc: ô thứ k có 2^(k-1) hạt
    uint64_t first8 = 0;
    for (int k = 1; k <= 8; k++) first8 += 1ULL << (k - 1);
    cout << "tong 8 o dau = " << first8 << ", rieng o 9 = " << (1ULL << 8) << "\n";
    cout << "o 64 = " << (1ULL << 63) << "\n";
    uint64_t total = 0;
    for (int k = 1; k <= 64; k++) total += 1ULL << (k - 1);
    cout << "tong 64 o = " << total << " = gioi han uint64_t: " << numeric_limits<uint64_t>::max() << "\n";

    // Tràn số: thêm 1 hạt
    uint64_t x = ~0ULL;
    x += 1;
    cout << "them 1 hat: " << x << "\n";
    cout << "gioi han int64_t (co dau): " << numeric_limits<int64_t>::max() << "\n";
    // Lựa chọn trong video: 1 triệu đồng mỗi ô, hay 1 đồng ở ô đầu rồi gấp đôi mỗi ô
    for (int k = 1; k <= 64; k++)
        if ((1ULL << k) - 1 > 1000000ULL * k) {
            cout << "gap doi vuot 1 trieu dong moi o tu o " << k << ": " << ((1ULL << k) - 1) << " dong > " << 1000000ULL * k << " dong\n";
            break;
        }
    cout << "rieng o 33 (xu) = " << (1ULL << 32) << ", tong 33 o (xu) = " << ((1ULL << 33) - 1) << "\n";

    // Mỗi ô một bit: hàng tốt trắng
    uint64_t whitePawns = 0;
    for (int f = 0; f < 8; f++) whitePawns |= bitOf(f, 1);
    cout << "tot trang = " << whitePawns << " = 0x" << hex << whitePawns << dec << "\n";

    // OR và AND: hai mã ở b1, d1
    uint64_t knightB1 = leapAttacks(1, 0, KNIGHT, 8), knightD1 = leapAttacks(3, 0, KNIGHT, 8);
    uint64_t both = knightB1 | knightD1, common = knightB1 & knightD1;
    cout << "ma b1: " << popcount(knightB1) << " o, ma d1: " << popcount(knightD1) << " o, OR: " << popcount(both)
         << " o, AND: " << popcount(common) << " o (bit " << countr_zero(common) << " = c3)\n";

    // Câu đố cuối: vua e1, hậu d1, tượng f1, mã g1, xe h1, tốt e2
    uint64_t occupied = bitOf(4, 0) | bitOf(3, 0) | bitOf(5, 0) | bitOf(6, 0) | bitOf(7, 0) | bitOf(4, 1);
    uint64_t attacked = 0;
    attacked |= leapAttacks(4, 0, KING, 8);                                          // vua e1
    attacked |= slideAttacks(3, 0, ROOK, 4, occupied) | slideAttacks(3, 0, BISHOP, 4, occupied);  // hậu d1
    attacked |= slideAttacks(5, 0, BISHOP, 4, occupied);                             // tượng f1
    attacked |= leapAttacks(6, 0, KNIGHT, 8);                                        // mã g1
    attacked |= slideAttacks(7, 0, ROOK, 4, occupied);                               // xe h1
    attacked |= bitOf(3, 2) | bitOf(5, 2);                                           // tốt e2 ăn chéo d3, f3
    cout << "doi quan danh " << popcount(attacked) << " o, o trong bi danh: " << popcount(attacked & ~occupied) << "\n";
}
