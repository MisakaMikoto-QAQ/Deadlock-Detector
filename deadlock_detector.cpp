// ============================================================
// 项目五：死锁检测 (Deadlock Detection)
// 文件：deadlock_detector.cpp
// 语言：C++17
// 编译：g++ -std=c++17 -Wall -Wextra -O2 -o deadlock_detector deadlock_detector.cpp
// 运行：./deadlock_detector input1_deadlock.txt
// ============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// ---------------- 系统状态 ----------------
struct SystemState {
    int n = 0;                        // 进程数
    int m = 0;                        // 资源类型数
    vector<int> available;            // 每类资源的当前可用数量
    vector<vector<int>> allocation;   // allocation[i][j]：Pi 已持有的 Rj 数量
    vector<vector<int>> request;      // request[i][j]   ：Pi 还额外请求的 Rj 数量
};

// ---------------- 检测结果 ----------------
struct DetectResult {
    bool hasDeadlock = false;   // 是否存在死锁
    vector<int> deadlocked;     // 死锁进程编号
    vector<bool> finish;        // 最终的 Finish 向量
    vector<int> order;          // 被成功回收（可完成）的进程顺序
};

// ---------------- 小工具：vector<int> -> "[a, b, c]" ----------------
static string vec2str(const vector<int>& v) {
    ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) oss << ", ";
        oss << v[i];
    }
    oss << "]";
    return oss.str();
}

// ---------------- 小工具：打印矩阵 ----------------
static void printMatrix(const vector<vector<int>>& mat, const string& title) {
    cout << title << ":\n";
    for (size_t i = 0; i < mat.size(); ++i) {
        cout << "  P" << i << ": " << vec2str(mat[i]) << "\n";
    }
}

// ============================================================
// 死锁检测核心算法
//   思想：在“资源分配图”上做化简（等价于银行家算法的安全性检查）
//   1) Work = Available;  Finish[i] = (Allocation[i] 全为 0) ? true : false
//   2) 反复寻找满足 (Finish[i]==false) 且 (Request[i] <= Work) 的进程 i：
//         Work += Allocation[i];  Finish[i] = true;
//   3) 若最终仍存在 Finish[i]==false 的进程，则这些进程处于死锁
// ============================================================
static DetectResult detectDeadlock(const SystemState& s, bool verbose = true) {
    const int n = s.n;
    const int m = s.m;

    vector<int>  work = s.available;
    vector<bool> finish(n, false);
    vector<int>  order;
    DetectResult res;

    // ---------- 步骤 1：初始化 ----------
    for (int i = 0; i < n; ++i) {
        bool holdsAny = false;
        for (int j = 0; j < m; ++j) {
            if (s.allocation[i][j] > 0) { holdsAny = true; break; }
        }
        // 不持有任何资源的进程不可能参与死锁，直接视作“可完成”
        if (!holdsAny) finish[i] = true;
    }

    if (verbose) {
        cout << "---- 检测过程 ----\n";
        cout << "初始 Work   = " << vec2str(work) << "\n";
        cout << "初始 Finish = ";
        for (int i = 0; i < n; ++i)
            cout << (finish[i] ? 'T' : 'F') << (i + 1 < n ? ' ' : '\n');
        cout << "\n";
    }

    // ---------- 步骤 2：反复扫描，回收能完成的进程 ----------
    int step = 0;
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < n; ++i) {
            if (finish[i]) continue;

            // 判断 Request[i] <= Work 是否成立
            bool ok = true;
            for (int j = 0; j < m; ++j) {
                if (s.request[i][j] > work[j]) { ok = false; break; }
            }
            if (!ok) continue;

            // 该进程可以顺利执行完，并归还它持有的全部资源
            if (verbose) {
                cout << "[第 " << ++step << " 个可完成] P" << i
                     << ": Request=" << vec2str(s.request[i])
                     << " <= Work=" << vec2str(work)
                     << " -> 释放 Allocation=" << vec2str(s.allocation[i]);
            }
            for (int j = 0; j < m; ++j) work[j] += s.allocation[i][j];
            finish[i] = true;
            order.push_back(i);
            changed = true;

            if (verbose) cout << "，新 Work=" << vec2str(work) << "\n";
        }
    }

    if (verbose) {
        bool anyLeft = false;
        for (int i = 0; i < n; ++i) if (!finish[i]) anyLeft = true;
        if (anyLeft) cout << "[扫描] 已找不到任何可以继续完成的进程，检测结束。\n";
        else         cout << "[扫描] 所有进程都能完成，系统不会死锁。\n";
    }

    // ---------- 步骤 3：汇总结果 ----------
    res.finish = finish;
    res.order  = order;
    for (int i = 0; i < n; ++i) {
        if (!finish[i]) res.deadlocked.push_back(i);
    }
    res.hasDeadlock = !res.deadlocked.empty();
    return res;
}

// ============================================================
// 读取输入
//   第 1 行：n m
//   第 2 行：Available[0..m-1]
//   接下来 n 行：Allocation 矩阵（每行 m 个整数）
//   再接下来 n 行：Request 矩阵（每行 m 个整数）
// ============================================================
static bool readState(istream& in, SystemState& s) {
    if (!(in >> s.n >> s.m)) return false;
    if (s.n <= 0 || s.m <= 0) return false;

    s.available.assign(s.m, 0);
    for (int j = 0; j < s.m; ++j)
        if (!(in >> s.available[j])) return false;

    s.allocation.assign(s.n, vector<int>(s.m, 0));
    for (int i = 0; i < s.n; ++i)
        for (int j = 0; j < s.m; ++j)
            if (!(in >> s.allocation[i][j])) return false;

    s.request.assign(s.n, vector<int>(s.m, 0));
    for (int i = 0; i < s.n; ++i)
        for (int j = 0; j < s.m; ++j)
            if (!(in >> s.request[i][j])) return false;

    return true;
}

// ============================================================
// 主函数
// ============================================================
int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SystemState s;
    ifstream fin;

    if (argc > 1) {
        // 从文件读入
        fin.open(argv[1]);
        if (!fin) {
            cerr << "错误：无法打开输入文件 \"" << argv[1] << "\"\n";
            return 1;
        }
        if (!readState(fin, s)) {
            cerr << "错误：文件 \"" << argv[1] << "\" 的数据格式不正确。\n";
            return 1;
        }
    } else {
        // 从标准输入读入
        cout << "（未指定输入文件，请按格式从标准输入录入，Ctrl+D 结束）\n";
        if (!readState(cin, s)) {
            cerr << "错误：标准输入的数据格式不正确。\n";
            return 1;
        }
    }

    cout << "========== 死锁检测 ==========\n";
    cout << "进程数 n = " << s.n << "，资源类型数 m = " << s.m << "\n";
    cout << "Available = " << vec2str(s.available) << "\n";
    printMatrix(s.allocation, "Allocation 矩阵（已持有）");
    printMatrix(s.request,    "Request 矩阵（还在申请）");
    cout << "\n";

    DetectResult r = detectDeadlock(s, /*verbose=*/true);

    cout << "\n---------- 检测结果 ----------\n";
    if (r.hasDeadlock) {
        cout << "系统处于【死锁】状态！\n";
        cout << "死锁进程：";
        for (size_t k = 0; k < r.deadlocked.size(); ++k) {
            cout << "P" << r.deadlocked[k];
            if (k + 1 < r.deadlocked.size()) cout << ", ";
        }
        cout << "\n";
    } else {
        cout << "系统【安全】，未检测到死锁。\n";
    }

    cout << "可顺利完成的进程顺序：";
    if (r.order.empty()) {
        cout << "(无)";
    } else {
        for (size_t k = 0; k < r.order.size(); ++k) {
            cout << "P" << r.order[k];
            if (k + 1 < r.order.size()) cout << " -> ";
        }
    }
    cout << "\n";

    cout << "最终 Finish 向量：";
    for (int i = 0; i < s.n; ++i)
        cout << (r.finish[i] ? 'T' : 'F') << (i + 1 < s.n ? ' ' : '\n');

    return 0;
}