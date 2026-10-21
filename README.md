# Cốt Đê – Chess Code bằng C++

Code đi kèm series **Chess Code** của kênh **Cốt Đê**: mỗi video là một câu đố trên bàn cờ, mỗi lời giải là một khái niệm lập trình. Dùng tự do ([MIT](LICENSE)).

Biên dịch mỗi bài: `g++ -O2 -std=c++20 <file>.cpp -o chay` (cần C++20 cho `std::popcount`).

Quy ước dùng chung cho mọi bài: ô a1 = 0, h8 = 63, `sq = file + rank * 8` (giống thư viện `python-chess`). Tên biến tiếng Anh, chú thích tiếng Việt.

| # | Bài | Khái niệm | Code | Video |
|---|---|---|---|---|
| 01 | Dạy máy nhìn bàn cờ | chỉ số ô, `%`, danh sách hướng, kiểm biên | _sắp có_ | _sắp có_ |
| 02 | 64 ô, 64 bit | `uint64_t`, tràn số, bitboard, OR / AND | _sắp có_ | _sắp có_ |
| 03 | Tìm đường trên bàn cờ | BFS + hàng đợi, quy hoạch động | _sắp có_ | _sắp có_ |
| 04 | Thử thông minh | cây quyết định, quay lui, vét cạn | _sắp có_ | _sắp có_ |
| 05 | Máy biết ai thắng trước khi chơi | thế thắng / thua, làm ngược, XOR | _sắp có_ | _sắp có_ |
| 06 | Khi không cần chạy code | bất biến, công thức, mô phỏng | _sắp có_ | _sắp có_ |

Ý tưởng bài tập theo *50 Chess and Mathematics Exercises for Schools* (dự án CHAMPS, 2019, CC BY-NC-SA 4.0). Code trong repo này do Cốt Đê viết.
