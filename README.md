# 逾渗模型
## Deployment in server
+ `tar -xJvf latest.tar.xz`解压
+ `sed -i -e 's/nullptr/0/g' *` nullptr

## 算力
+ 每个电脑每天算力为`2.5GHz*86400s=2.16e5G`
+ 三维p=0.2488当最大宽度为256的时候大概消耗算力6.5G每次
+ 每台电脑每天计算约3e4次，100台2天大概6e6

## 低维键逾渗的键分类
### 初始化
确定好总键数

### 剪枝 Prune Branches
+ 剪枝就是把所有度为1的点循环剪除，直到所有点度都非1。建立树根数组存储被剪除的树枝大小。
+ 数出来最大的树枝

### 缩环 Shrink Circuits
+ 成环时，把中间有分岔的节点的父节点都回溯到这个环的最基础的根节点
+ 这个过程是半破坏性的

### 计数

+ 进行BFS遍历的时候统计好所有类型逾渗的最大团的大小
+ 统计每个丛的大小，进而得到最大丛
+ 统计每个无枝丛的大小，进而得到Branch-free的大小，以及最大无枝丛
+ 统计每个无桥丛的大小，进而得到Bridge-free的大小，以及最大无桥丛

### 性能分析
+ 内存占用约为`(10+2*D)*W^D` byte
    + `2048*2048`对应`(10+4)*2048*2048`
+ 时间上`128*128`大小的格子跑`1000`次用时`1.6s`，平均每秒1e7个Site

## 低维键逾渗的拓扑结构

+ 格点空间的基矢量的线性组合
+ 不定方程？
