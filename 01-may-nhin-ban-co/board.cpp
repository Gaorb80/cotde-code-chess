// Cốt Đê – Chess Code 01: Dạy máy nhìn bàn cờ
// Video: <link YouTube>
// Biên dịch: g++ -O2 -std=c++20 board.cpp -o chay
// Quy ước ô: sq = file + rank * 8 (a1 = 0, h8 = 63)

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Đổi cột, hàng thành số ô và ngược lại
int toSquare(int file, int rank) { return file + rank * 8; }
int fileOf(int sq) { return sq % 8; }
int rankOf(int sq) { return sq / 8; }
string nameOf(int sq) { return string(1, 'a' + fileOf(sq)) + to_string(rankOf(sq) + 1); }

// Ô tối khi cột + hàng là số chẵn (a1 là ô tối)
bool isDark(int sq) { return (fileOf(sq) + rankOf(sq)) % 2 == 0; }

bool onBoard(int file, int rank) { return 0 <= file && file < 8 && 0 <= rank && rank < 8; }

// Quân trượt: đi theo từng hướng tới khi ra khỏi bàn (bàn trống)
vector<int> slide(int sq, const vector<pair<int, int>>& dirs) {
    vector<int> result;
    for (auto [df, dr] : dirs) {
        int f = fileOf(sq) + df, r = rankOf(sq) + dr;
        while (onBoard(f, r)) {          // kiểm biên bằng cột, hàng: không bao giờ "xuyên tường"
            result.push_back(toSquare(f, r));
            f += df;
            r += dr;
        }
    }
    return result;
}

// Quân nhảy: mỗi bước là một độ lệch cố định
vector<int> leap(int sq, const vector<pair<int, int>>& steps) {
    vector<int> result;
    for (auto [df, dr] : steps) {
        int f = fileOf(sq) + df, r = rankOf(sq) + dr;
        if (onBoard(f, r)) result.push_back(toSquare(f, r));
    }
    return result;
}

// Mọi bước của quân nhảy (m, n): m ô một hướng, rồi n ô vuông góc
vector<pair<int, int>> leaperSteps(int m, int n) {
    vector<pair<int, int>> steps;
    for (auto [a, b] : {pair{m, n}, pair{n, m}})
        for (int sa : {1, -1})
            for (int sb : {1, -1}) {
                pair<int, int> s{sa * a, sb * b};
                bool seen = false;
                for (auto t : steps) seen = seen || t == s;
                if (!seen) steps.push_back(s);
            }
    return steps;
}

const vector<pair<int, int>> ROOK = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
const vector<pair<int, int>> BISHOP = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const vector<pair<int, int>> KING = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

vector<int> queen(int sq) {              // hậu = xe + tượng
    vector<int> a = slide(sq, ROOK), b = slide(sq, BISHOP);
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

int main() {
    int e4 = toSquare(4, 3);
    cout << "e4 = " << e4 << ", h8 = " << toSquare(7, 7) << ", a1 = " << toSquare(0, 0) << "\n";
    cout << "o 28 la " << nameOf(28) << ", o 63 la " << nameOf(63) << "\n";
    cout << "a1 " << (isDark(0) ? "toi" : "sang") << ", c4 " << (isDark(toSquare(2, 3)) ? "toi" : "sang")
         << ", e4 " << (isDark(e4) ? "toi" : "sang") << "\n";

    // Nước đi là phép cộng
    cout << "e4 + 8 = " << nameOf(e4 + 8) << ", +1 = " << nameOf(e4 + 1) << ", +9 = " << nameOf(e4 + 9)
         << ", +7 = " << nameOf(e4 + 7) << ", -8 = " << nameOf(e4 - 8) << "\n";

    // Bẫy xuyên tường: h4 cộng 1 rơi sang mép trái hàng trên
    int h4 = toSquare(7, 3), a5 = toSquare(0, 4);
    cout << "h4 = " << h4 << ", h4 + 1 = " << h4 + 1 << " = " << nameOf(h4 + 1) << " (xuyen tuong)\n";
    cout << "a5 = " << a5 << ", a5 - 1 = " << a5 - 1 << " = " << nameOf(a5 - 1) << " (xuyen tuong)\n";

    // Vòng lặp đếm (đoạn 28 trong video): đặt quân vào từng ô, ghi số ô nó đánh được lên ô đó
    int power[64];
    for (int sq = 0; sq < 64; sq++)
        power[sq] = slide(sq, ROOK).size();
    cout << "ban do xe: a1 = " << power[0] << ", d4 = " << power[toSquare(3, 3)] << ", h8 = " << power[63] << "\n";
    int queenPower[64];                  // short "hậu ở góc yếu hơn hậu ở giữa"
    for (int sq = 0; sq < 64; sq++)
        queenPower[sq] = queen(sq).size();
    cout << "ban do hau: a1 = " << queenPower[0] << ", d4 = " << queenPower[toSquare(3, 3)] << "\n";

    // Bản đồ sức mạnh: số ô đánh được trên bàn trống
    auto range = [](auto attacks) {
        size_t lo = 99, hi = 0;
        for (int sq = 0; sq < 64; sq++) { size_t n = attacks(sq).size(); lo = min(lo, n); hi = max(hi, n); }
        cout << lo << " den " << hi;
    };
    cout << "xe: "; range([](int s) { return slide(s, ROOK); }); cout << "\n";
    cout << "tuong: "; range([](int s) { return slide(s, BISHOP); }); cout << "\n";
    cout << "hau: "; range([](int s) { return queen(s); });
    cout << " (a1: " << queen(0).size() << ", d4: " << queen(toSquare(3, 3)).size() << ")\n";
    auto knight = leaperSteps(2, 1);
    cout << "ma: "; range([&](int s) { return leap(s, knight); }); cout << "\n";
    cout << "vua: "; range([](int s) { return leap(s, KING); }); cout << "\n";
    cout << "lac da (3,1): " << leaperSteps(3, 1).size() << " huong, alfil (2,2): " << leaperSteps(2, 2).size() << " huong\n";

    // Câu đố cuối: hậu + mã ở d2 đánh bao nhiêu ô (không tính trùng)
    int d2 = toSquare(3, 1);
    vector<bool> hit(64, false);
    for (int s : queen(d2)) hit[s] = true;
    for (int s : leap(d2, knight)) hit[s] = true;
    int count = 0;
    for (bool h : hit) count += h;
    cout << "hau + ma o d2 danh " << count << " o\n";
}
