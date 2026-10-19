# 死锁检测器 · Deadlock Detector

> 🧭 基于资源分配图化简的经典死锁检测算法实现 —— 一行命令，告诉你系统是否死锁、谁是凶手。

[![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20WSL2-orange.svg)]()
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![License](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)
[![Version](https://img.shields.io/badge/version-v1.0.0-informational.svg)]()

---

## 📖 项目简介

**死锁检测器（Deadlock Detector）** 是一个用 C++17 编写的操作系统课程实验项目，用于检测多进程系统中是否出现死锁，并输出死锁进程集合与检测过程的详细轨迹。

死锁是并发系统中一类经典问题：多个进程因循环等待对方持有的资源而全部停滞。本项目基于**资源分配图化简算法**（与银行家算法的安全性检查同源），可以在 **O(n²·m)** 的时间内判断系统状态，并定位死锁进程。

### ✨ 亮点

- 🎯 **零依赖**：仅使用 C++ 标准库，无需任何第三方库
- 📊 **过程可视**：逐步打印 `Work` / `Finish` 向量的演化过程，便于教学演示与答辩
- 🧩 **支持多实例资源**：每类资源可拥有多个实例，覆盖经典课本题型
- 📁 **文件 / 标准输入双模式**：既可以从 `.txt` 批量跑用例，也可以交互式录入
- ⚙️ **跨平台**：Linux / WSL2（Ubuntu 24.04）/ macOS 均可编译运行

---

## 🚀 功能特性

- ✅ **死锁检测**：基于资源分配图可化简性判断系统是否处于死锁状态
- ✅ **死锁进程定位**：输出处于死锁环中的所有进程编号（如 `P0, P1, P2`）
- ✅ **检测过程可视化**：打印初始 `Work`、每次资源回收、最终 `Finish` 向量
- ✅ **安全序列输出**：若系统安全，打印可顺利完成的进程顺序
- ✅ **多实例资源支持**：`Available` / `Allocation` / `Request` 均为向量，支持每类资源任意实例数
- ✅ **多种输入方式**：支持从文件读取，也支持从 `stdin` 手动录入
- ✅ **自动跳过空进程**：不持有任何资源的进程被自动视作“可完成”，避免误报
- ✅ **编译期严格**：开启 `-Wall -Wextra`，零警告零报错

---

## 🛠️ 环境依赖

| 项目 | 版本 / 说明 |
|---|---|
| 操作系统 | Linux（Ubuntu 20.04+） / WSL2（Windows 下的 Ubuntu 24.04） / macOS |
| 编译器 | `g++` ≥ 9.0（需支持 C++17）或 `clang++` ≥ 10.0 |
| 构建工具 | `make`（可选） |
| 调试器 | `gdb`（可选，用于断点调试） |
| 编辑器 | VSCode + 扩展 `C/C++`（Microsoft）、`WSL`（Microsoft，仅 WSL 用户） |
| 第三方库 | **无** |

### 安装依赖（Ubuntu / WSL2）

```bash
sudo apt update
sudo apt install -y build-essential gdb
```

---

## 📦 安装 / 编译步骤

### 方式一：命令行直接编译（推荐）

```bash
# 1. 进入项目目录
cd ~/projects/deadlock_detection

# 2. 编译（严格模式，开所有警告）
g++ -std=c++17 -Wall -Wextra -O2 -o deadlock_detector deadlock_detector.cpp

# 3. 确认可执行文件已生成
ls -l deadlock_detector
# 期望：-rwxr-xr-x ... deadlock_detector
```

### 方式二：使用 Makefile（可选）

```bash
make            # 编译
make run1       # 跑死锁用例
make run2       # 跑安全用例
make clean      # 清理构建产物
```

### 方式三：VSCode 一键构建（可选）

在 `.vscode/tasks.json` 中已配置好构建任务，按 `Ctrl + Shift + B` 一键编译。
按 `F5` 使用 `.vscode/launch.json` 中的配置进行断点调试。

---

## ⚡ 快速开始 / 使用示例

### 1. 准备输入文件 `input1_deadlock.txt`

```text
4 3
0 0 0
1 0 0
0 1 0
0 0 1
0 0 0
0 1 0
0 0 1
1 0 0
0 0 0
```

**格式说明**（3 段）：

| 行号 | 内容 |
|---|---|
| 第 1 行 | `n m`：进程数 `n`，资源类型数 `m` |
| 第 2 行 | `m` 个整数：`Available[0..m-1]` |
| 第 3 ~ n+2 行 | `n × m` 矩阵：`Allocation`（进程已持有） |
| 第 n+3 ~ 2n+2 行 | `n × m` 矩阵：`Request`（进程还在申请） |

### 2. 运行

```bash
./deadlock_detector input1_deadlock.txt
```

### 3. 预期输出

```text
========== 死锁检测 ==========
进程数 n = 4，资源类型数 m = 3
Available = [0, 0, 0]
Allocation 矩阵（已持有）:
  P0: [1, 0, 0]
  P1: [0, 1, 0]
  P2: [0, 0, 1]
  P3: [0, 0, 0]
Request 矩阵（还在申请）:
  P0: [0, 1, 0]
  P1: [0, 0, 1]
  P2: [1, 0, 0]
  P3: [0, 0, 0]

---- 检测过程 ----
初始 Work   = [0, 0, 0]
初始 Finish = F F F T
[扫描] 已找不到任何可以继续完成的进程，检测结束。

---------- 检测结果 ----------
系统处于【死锁】状态！
死锁进程：P0, P1, P2
可顺利完成的进程顺序：(无)
最终 Finish 向量：F F F T
```

### 4. 交互模式（从键盘输入）

```bash
./deadlock_detector
# 手动逐行输入数据，最后按 Ctrl + D 结束
```

---

## 📂 项目结构

```text
deadlock_detection/
├── deadlock_detector.cpp      # 主程序（约 240 行，单文件实现）
├── input1_deadlock.txt        # 测试用例：会发生死锁（3 进程环形等待）
├── input2_safe.txt            # 测试用例：安全无死锁
├── Makefile                   # 可选，一键编译/运行
├── README.md                  # 本文件
├── LICENSE                    # MIT 许可证
└── .vscode/
    ├── tasks.json             # VSCode 构建任务
    └── launch.json            # VSCode 断点调试配置
```

### 关键文件说明

| 文件 | 作用 |
|---|---|
| `deadlock_detector.cpp` | 全部逻辑集中于此：数据结构定义、算法核心、I/O 处理、结果展示 |
| `input1_deadlock.txt` | 经典环形等待用例，用于验证死锁检测 |
| `input2_safe.txt` | 安全系统用例，用于验证算法不会误报 |
| `.vscode/tasks.json` | `Ctrl+Shift+B` 触发编译 |
| `.vscode/launch.json` | `F5` 触发 gdb 调试 |

---

## 🧠 核心设计 / 原理说明

### 1. 数据抽象

系统状态被建模为三元组：

```
S = (Available, Allocation, Request)
```

- `Available[j]`：第 `j` 类资源的当前空闲数量
- `Allocation[i][j]`：进程 `P_i` 已持有的第 `j` 类资源数量
- `Request[i][j]`：进程 `P_i` 还额外申请的第 `j` 类资源数量

### 2. 检测算法（资源分配图化简）

**核心思想**：如果某进程当前的所有申请都能被现有空闲资源满足，就假设它执行完并释放全部资源；反复化简，无法继续化简的进程即为死锁进程。

```text
步骤 1（初始化）：
    Work    := Available
    Finish[i] := (Allocation[i] 全为 0) ? true : false

步骤 2（循环扫描）：
    while 存在 i 满足 (!Finish[i] && Request[i] <= Work)：
        Work     += Allocation[i]
        Finish[i] = true

步骤 3（判定）：
    if 存在 i 使 Finish[i] == false：
        这些进程处于死锁
```

**复杂度**：`O(n² · m)`
- 最坏情况下每轮只消去一个进程，共 `n` 轮；
- 每轮扫描 `n` 个进程，每个进程比较 `m` 维向量。

### 3. 为什么这样能检测死锁？

把系统看成**资源分配图**：
- 节点：进程 `P`、资源类 `R`
- 边 `P → R`：进程还在申请资源（对应 `Request`）
- 边 `R → P`：资源已分配给进程（对应 `Allocation`）

> **定理**：系统死锁 ⇔ 资源分配图不可完全化简。

算法中的 `Request[i] <= Work ⟹ Work += Allocation[i]` 正好对应“假设进程执行完并释放资源”的化简动作。最终 `Finish[i] == false` 的进程就是化简不动的、处于死锁中的进程。

### 4. 与银行家算法的关系

本项目算法与银行家算法中的**安全性检查**几乎是同一段代码，区别仅在于语义：

| 对比项 | 银行家算法（安全序列） | 本项目（死锁检测） |
|---|---|---|
| 判断时机 | **分配前**，预防死锁 | **运行中**，检测死锁 |
| 使用向量 | `Need[i] = Max[i] - Allocation[i]` | `Request[i]`（当前实际申请） |
| 输出 | 是否存在安全序列 | 是否存在死锁进程 |

---

## 📚 API 说明

本项目为**单文件命令行程序**，所有公开接口都在 `deadlock_detector.cpp` 中，可根据需要作为头文件引入。

### `struct SystemState`

系统状态数据结构。

| 字段 | 类型 | 说明 |
|---|---|---|
| `n` | `int` | 进程数量 |
| `m` | `int` | 资源类型数量 |
| `available` | `vector<int>` | 长度 `m`，各类资源的空闲数量 |
| `allocation` | `vector<vector<int>>` | `n × m` 矩阵，进程已持有资源 |
| `request` | `vector<vector<int>>` | `n × m` 矩阵，进程还在申请的资源 |

### `struct DetectResult`

检测结果。

| 字段 | 类型 | 说明 |
|---|---|---|
| `hasDeadlock` | `bool` | 是否存在死锁 |
| `deadlocked` | `vector<int>` | 处于死锁的进程编号列表 |
| `finish` | `vector<bool>` | 最终的 Finish 向量 |
| `order` | `vector<int>` | 可顺利完成的进程顺序 |

### `DetectResult detectDeadlock(const SystemState& s, bool verbose = true)`

**功能**：执行死锁检测算法。

| 参数 | 类型 | 说明 |
|---|---|---|
| `s` | `const SystemState&` | 待检测的系统状态 |
| `verbose` | `bool` | 是否打印检测过程，默认 `true` |

**返回**：`DetectResult` 结构体，包含检测结论与过程数据。

**异常**：不抛出异常。若输入不合法（如 `n<=0`），`readState` 会返回 `false`。

### `bool readState(istream& in, SystemState& s)`

**功能**：从输入流中读取系统状态。

**返回**：`true` 表示读取成功，`false` 表示格式错误或遇到流结束。

### `main(int argc, char** argv)`

**用法**：
- `./deadlock_detector <file>`：从文件读入
- `./deadlock_detector`：从标准输入读入（`Ctrl+D` 结束）

**退出码**：
- `0`：检测完成（无论是否死锁）
- `1`：无法打开文件 / 数据格式错误

---

## 🧪 运行测试

### 测试用例 1：死锁系统

```bash
./deadlock_detector input1_deadlock.txt
```

**预期**：
- 检测结论：`系统处于【死锁】状态！`
- 死锁进程：`P0, P1, P2`
- 最终 Finish 向量：`F F F T`

### 测试用例 2：安全系统

```bash
./deadlock_detector input2_safe.txt
```

**预期**：
- 检测结论：`系统【安全】，未检测到死锁。`
- 完成顺序：`P0 -> P1`
- 最终 Finish 向量：`T T T`

### 手动构造测试

修改 `input1_deadlock.txt` 的第 2 行为 `1 0 0`（给系统一个空闲的 R0），重跑：

```bash
./deadlock_detector input1_deadlock.txt
```

**预期**：P0 立即能拿到 R0 完成 → 释放 R0、R1 → P1 完成后 → P2 完成，**死锁消失**，系统变为安全。这是一个验证算法正确性的好方法。

---

## ❓ 常见问题 FAQ

<details>
<summary><b>Q1：提示 <code>g++: command not found</code>？</b></summary>

未安装编译器。执行：
```bash
sudo apt update && sudo apt install -y build-essential
```
</details>

<details>
<summary><b>Q2：报错 <code>fatal error: iostream: No such file or directory</code>？</b></summary>

编译器版本过低或环境异常。重新安装 `build-essential`；若仍报错，检查 `g++ --version` 是否 ≥ 9.0。
</details>

<details>
<summary><b>Q3：提示 <code>Permission denied: ./deadlock_detector</code>？</b></summary>

可能是编译产物不存在或没有可执行权限。先编译再运行：
```bash
g++ -std=c++17 -o deadlock_detector deadlock_detector.cpp
chmod +x deadlock_detector
./deadlock_detector input1_deadlock.txt
```
</details>

<details>
<summary><b>Q4：提示 <code>无法打开输入文件</code>？</b></summary>

当前工作目录不是项目目录。`cd` 到 `deadlock_detection`，或者使用绝对路径：
```bash
./deadlock_detector /home/yourname/projects/deadlock_detection/input1_deadlock.txt
```
</details>

<details>
<summary><b>Q5：提示 <code>数据格式不正确</code>？</b></summary>

输入文件行列数与声明的 `n m` 不匹配。请按照格式仔细核对：
- 第 1 行：`n m`
- 第 2 行：`m` 个整数
- 之后：`n` 行 Allocation + `n` 行 Request
</details>

<details>
<summary><b>Q6：Makefile 报 <code>missing separator. Stop.</code>？</b></summary>

Makefile 中的 recipe 行必须用**真正的 Tab 缩进**，不能用空格。在 VSCode 右下角将 `Indent Using Spaces` 改为 `Indent Using Tabs`。
</details>

<details>
<summary><b>Q7：VSCode 终端跑出来是 Windows 的结果？</b></summary>

当前终端是 PowerShell 而不是 WSL。点击终端右上角的下拉箭头，选择 `Ubuntu (WSL)`；或者 `Ctrl+Shift+P` → `Reopen Folder in WSL`。
</details>

<details>
<summary><b>Q8：中文输出乱码？</b></summary>

极少出现在 WSL 环境中。若出现，执行 `export LANG=C.UTF-8`，或在 `.bashrc` 中加入该行。
</details>

---

## 🤝 贡献指南

欢迎通过 Issue 和 Pull Request 参与改进！

### 报告问题

提交 Issue 时请附上：
1. 操作系统与编译器版本（`uname -a`、`g++ --version`）
2. 完整错误信息（从第一条错误开始）
3. 复现步骤与对应的输入文件

### 提交代码

1. Fork 本仓库并新建分支：`git checkout -b feature/your-feature`
2. 保持单文件实现风格（除非确有拆分必要）
3. 通过严格编译：`g++ -std=c++17 -Wall -Wextra -Wpedantic -o dd deadlock_detector.cpp`
4. 提交信息规范：`feat: ...` / `fix: ...` / `docs: ...` / `refactor: ...`
5. 发起 Pull Request，描述改动动机与验证方式

### 代码风格

- 缩进：4 空格
- 命名：变量 `lowerCamel`，类型 `UpperCamel`，常量 `UPPER_SNAKE`
- 注释：中文，简洁说明“为什么”而不是“是什么”
- 所有新增公开函数需附 Doxygen 风格注释

---

## 📄 许可证 License

本项目采用 **MIT 许可证**，详见 [LICENSE](./LICENSE)。

```text
MIT License

Copyright (c) 2025 MisakaMikoto-QAQ

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 👤 作者与联系方式

| 项目 | 信息 |
|---|---|
| 作者 | MisakaMikoto-QAQ |
| 邮箱 | 3128204593@qq.com |
| GitHub | [MisakaMikoto-QAQ](https://github.com/MisakaMikoto-QAQ) |
| 课程 | 操作系统原理 · 项目五：死锁检测 |

> 📧 如有问题或建议，欢迎通过 Issue 或邮箱联系。

---

## 🙏 致谢 / 参考

- 《操作系统概念》（*Operating System Concepts*, Silberschatz 等著）第 8 章 · 死锁
- 《现代操作系统》（*Modern Operating Systems*, Tanenbaum 著）第 6 章
- 汤小丹等《计算机操作系统（第四版）》 第 3 章 · 处理机调度与死锁
- [cppreference.com](https://en.cppreference.com/) - C++17 标准库参考
- [Shields.io](https://shields.io/) - README 徽章生成
- 项目灵感来源于大学操作系统课程实验要求

---

## 🗺️ 更新日志 / 路线图

### v1.0.0 (2025-01) · 首个正式版本

- ✅ 基于资源分配图化简的死锁检测算法
- ✅ 检测过程可视化输出
- ✅ 支持多实例资源
- ✅ 文件 / 标准输入双模式
- ✅ 完整的 VSCode 构建与调试配置

### 路线图 Roadmap

- [ ] **v1.1.0**：新增等待图（Wait-for Graph）+ DFS 找环，输出死锁环路径（如 `P0 → P1 → P2 → P0`）
- [ ] **v1.2.0**：新增死锁恢复建议（列出“杀掉某进程可释放哪些资源”）
- [ ] **v1.3.0**：单元测试框架（Catch2 / doctest）
- [ ] **v1.4.0**：随机测试生成器 + 批量回归脚本
- [ ] **v1.5.0**：图形化可视化（可选，基于 Graphviz 输出 PDF/PNG 资源分配图）

---

<div align="center">

**⭐ 如果这个项目对你有帮助，欢迎点个 Star ⭐**

Made with ❤️ for Operating Systems Course

</div>