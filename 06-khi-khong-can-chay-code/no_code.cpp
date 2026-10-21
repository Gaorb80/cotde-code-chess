// Cốt Đê – Chess Code 06: Khi không cần chạy code
// Video: <link YouTube>
// Biên dịch: g++ -O2 -std=c++20 no_code.cpp -o chay
// Quy ước ô: sq = file + rank * 8 (a1 = 0, h8 = 63); ô tối khi (file + rank) chẵn

#include <iostream>
#include <random>
#include <vector>
using namespace std;

bool isDark(int f, int r) { return (f + r) % 2 == 0; }

vector<int> kingMoves(int sq) {
    vector<int> m;
    for (int df = -1; df <= 1; df++)
        for (int dr = -1; dr <= 1; dr++) {
            if (df == 0 && dr == 0) continue;
            int f = sq % 8 + df, r = sq / 8 + dr;
            if (f >= 0 && f < 8 && r >= 0 && r < 8) m.push_back(f + r * 8);
        }
    return m;
}

int main() {
    // 1. Bàn cờ cắt hai góc a1, h8
    int dark = 0, light = 0;
    for (int r = 0; r < 8; r++)
        for (int f = 0; f < 8; f++) {
            if ((f == 0 && r == 0) || (f == 7 && r == 7)) continue;
            isDark(f, r) ? dark++ : light++;
        }
    cout << "ban cut goc: " << light << " o sang, " << dark << " o toi\n";

    // 2. Xe đi từng ô: ô thứ k có màu đổi theo k
    cout << "a1 " << (isDark(0, 0) ? "toi" : "sang") << ", h8 " << (isDark(7, 7) ? "toi" : "sang")
         << "; o thu 64 phai khac mau o thu 1\n";
    int n = 7, steps = 0, f = 0, r = 0;                  // bàn 7 x 7: đi kiểu rắn bò
    for (r = 0; r < n; r++)
        for (int i = 0; i < n; i++) { f = (r % 2 == 0) ? i : n - 1 - i; steps++; }
    cout << "ban 7x7 ran bo: di " << steps << " o, ket thuc o cot " << f << ", hang " << r - 1 << "\n";

    // 3. Hình vuông, hình chữ nhật: vòng lặp và công thức
    int squares = 0, rects = 0;
    for (int x1 = 0; x1 <= 8; x1++) for (int x2 = x1 + 1; x2 <= 8; x2++)
        for (int y1 = 0; y1 <= 8; y1++) for (int y2 = y1 + 1; y2 <= 8; y2++) {
            rects++;
            if (x2 - x1 == y2 - y1) squares++;
        }
    int formulaSquares = 0;
    for (int k = 1; k <= 8; k++) formulaSquares += k * k;
    int pairs = 9 * 8 / 2;
    cout << "hinh vuong: " << squares << " (cong thuc " << formulaSquares << "), hinh chu nhat: " << rects
         << " (cong thuc " << pairs * pairs << "), khong vuong: " << rects - squares << "\n";
    int twoByTwo = 7 * 7;
    cout << "hinh vuong 2x2: " << twoByTwo << "\n";

    // 4. Vua đi ngẫu nhiên từ a8 (ô 56)
    const int A8 = 56;
    // Cách đếm hay nhầm: coi mọi đường đi có xác suất bằng nhau
    int paths2 = 0, back2 = 0;
    for (int s1 : kingMoves(A8)) for (int s2 : kingMoves(s1)) { paths2++; back2 += (s2 == A8); }
    int paths3 = 0, back3 = 0;
    for (int s1 : kingMoves(A8)) for (int s2 : kingMoves(s1)) for (int s3 : kingMoves(s2)) { paths3++; back3 += (s3 == A8); }
    cout << "dem duong 2 nuoc: " << back2 << "/" << paths2 << ", 3 nuoc: " << back3 << "/" << paths3 << "\n";
    // Tính đúng: mỗi nước chọn đều trong các ô đi được
    double exact2 = 0, exact3 = 0;
    for (int s1 : kingMoves(A8)) {
        double p1 = 1.0 / kingMoves(A8).size();
        for (int s2 : kingMoves(s1)) {
            double p2 = p1 / kingMoves(s1).size();
            if (s2 == A8) exact2 += p2;
            for (int s3 : kingMoves(s2)) if (s3 == A8) exact3 += p2 / kingMoves(s2).size();
        }
    }
    cout << "xac suat dung: 2 nuoc " << exact2 << " (7/40 = " << 7.0 / 40 << "), 3 nuoc " << exact3 << " (3/50 = " << 3.0 / 50 << ")\n";
    // Mô phỏng một triệu lần
    mt19937 rng(2026);
    long long hit2 = 0, hit3 = 0, N = 1000000;
    for (long long i = 0; i < N; i++) {
        int sq = A8;
        for (int step = 1; step <= 3; step++) {
            vector<int> m = kingMoves(sq);
            sq = m[uniform_int_distribution<int>(0, (int)m.size() - 1)(rng)];
            if (step == 2 && sq == A8) hit2++;
            if (step == 3 && sq == A8) hit3++;
        }
    }
    cout << "mo phong " << N << " lan: 2 nuoc " << 100.0 * hit2 / N << "%, 3 nuoc " << 100.0 * hit3 / N << "%\n";
}
