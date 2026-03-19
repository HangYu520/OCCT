# NURBS BOOK 笔记

## 目录

- 1 曲线曲面基础知识

    - [1.1 隐函数与参数函数](#11-隐函数与参数函数)

    - [1.2 贝塞尔曲线 (Bézier Curves)](#12-贝塞尔曲线-bézier-curves)

## 1. 曲线曲面基础知识

### 1.1 隐函数与参数函数

**隐函数形式 (Implicit Form)：**
$$
f(x, y) = 0
$$

**参数函数形式 (Parametric Form)：**
$$
\mathbf{C}(u) = \bigl( x(u), y(u) \bigr), \quad u \in [a, b]
$$

参数表达通常是不唯一的。例如，对于单位圆，可以有以下两种不同的参数化方式：

a. **三角函数表达：**
$$
\begin{cases}
x(u) = \cos(u) \\
y(u) = \sin(u)
\end{cases}, \quad u \in [0, 2\pi]
$$

b. **有理多项式表达 (代数形式)：**
$$
\begin{cases}
x(t) = \dfrac{1-t^2}{1+t^2} \\
y(t) = \dfrac{2t}{1+t^2}
\end{cases}, \quad t \in [0, 1]
$$

可以将参数 $u$ 看作时间，那么参数曲线的一阶和二阶导数 $\mathbf{C}'(u), \mathbf{C}''(u)$ 就对应了运动轨迹的速度和加速度矢量。

> **均匀参数化 (Uniform Parameterization)：** 若参数曲线满足 $|\mathbf{C}'(u)| = \text{const}$（即速率为常数），则称该参数化为均匀参数化。在物理意义上，这代表点沿曲线做匀速运动。

对于**三维参数曲面**，其定义是一个由两个独立参数 $(u, v)$ 映射到三维空间的函数：

$$
\mathbf{S}(u, v) = \bigl( x(u, v), y(u, v), z(u, v) \bigr), \quad (u, v) \in [a, b] \times [c, d]
$$

**偏导数 (Partial Derivatives)：**
在曲面上任意一点 $(u, v)$，可以定义两个方向的切矢量：
- **$u$ 方向切矢量：** $\mathbf{S}_u = \dfrac{\partial \mathbf{S}}{\partial u} = \left( \dfrac{\partial x}{\partial u}, \dfrac{\partial y}{\partial u}, \dfrac{\partial z}{\partial u} \right)$
- **$v$ 方向切矢量：** $\mathbf{S}_v = \dfrac{\partial \mathbf{S}}{\partial v} = \left( \dfrac{\partial x}{\partial v}, \dfrac{\partial y}{\partial v}, \dfrac{\partial z}{\partial v} \right)$

**切平面 (Tangent Plane)：**
如果 $\mathbf{S}_u$ 和 $\mathbf{S}_v$ 在某点处线性无关（即它们的叉积不为零），那么这两个矢量共同张成该点处的**切平面**。切平面上的任意一点 $\mathbf{P}$ 可以表示为：
$$
\mathbf{P}(s, t) = \mathbf{S}(u, v) + s \mathbf{S}_u + t \mathbf{S}_v
$$

**法矢量 (Surface Normal)：**
曲面在该点处的法矢量可通过两个切矢量的叉积获得：
$$
\mathbf{n}(u, v) = \mathbf{S}_u \times \mathbf{S}_v
$$
归一化后的单位法矢量为：
$$
\mathbf{N}(u, v) = \frac{\mathbf{S}_u \times \mathbf{S}_v}{|\mathbf{S}_u \times \mathbf{S}_v|}
$$

> **正则点 (Regular Point)：** 若满足 $|\mathbf{S}_u \times \mathbf{S}_v| \neq 0$，则该点被称为正则点。在正则点处，曲面的切平面和法矢量是唯一确定的。
>
> **补充说明：** 法矢量的存在性是一个**几何属性**，与具体的参数化方式无关。在某些点处，即使 $|\mathbf{S}_u \times \mathbf{S}_v| = 0$（被称为**奇异点 Singular Point**），也可能仅仅是由于参数化选择不当导致的（例如球体在极点处的极坐标参数化）。在这些点上，几何法矢量依然可能存在，只是无法通过当前的参数化直接计算。可以通过更换参数化方式或取极限的方法来重新获得该点处的法矢量。

### 1.2 贝塞尔曲线 (Bézier Curves)

为了表达空间中的所有曲线，需要有对应的基函数。最简单地，可以在多项式空间构建曲线。但为了保持曲线的交互可编辑性，一般使用贝塞尔曲线。

#### 1.贝塞尔曲线的定义

一个 $n$ 次贝塞尔曲线由 $n+1$ 个控制点 $\{\mathbf{P}_i\}$ 定义：
$$
\mathbf{C}(u) = \sum_{i=0}^n B_{i,n}(u) \mathbf{P}_i, \quad u \in [0, 1]
$$

其中 $n$ 次伯恩斯坦基函数(Bernstein Basis Functions)定义为：
$$
B_{i,n}(u) = \binom{n}{i} u^i (1-u)^{n-i}, \quad i=0, \dots, n
$$
其中二项式系数为：
$$
\binom{n}{i} = \frac{n!}{i!(n-i)!}
$$

#### 2.基函数的性质和特点
*   **非负性 (Non-negativity)：** 对于所有的 $i, n$ 和 $u \in [0, 1]$，$B_{i,n}(u) \ge 0$。
*   **单位分解性 (Partition of Unity)：** 所有基函数之和恒等于 1：
    $$
    \sum_{i=0}^n B_{i,n}(u) = 1
    $$
*   **端点性质：** $B_{0,n}(0) = 1$，$B_{n,n}(1) = 1$。这保证了曲线**经过第一个 $\mathbf{P}_0$ 和最后一个控制点 $\mathbf{P}_n$**。
*   **对称性：** $B_{i,n}(u) = B_{n-i, n}(1-u)$。
*   **端点相切：** 曲线在端点处的切线方向由前两个和最后两个控制点决定：
    $$
    \mathbf{C}'(0) = n(\mathbf{P}_1 - \mathbf{P}_0), \quad \mathbf{C}'(1) = n(\mathbf{P}_n - \mathbf{P}_{n-1})
    $$
*   **凸包性 (Convex Hull Property)：** 整条曲线完全落在其控制点构成的凸包之内。
*   **递推性 (Recurrence)：** $n$ 次基函数可以由两个 $n-1$ 次基函数通过线性组合生成：
    $$
    B_{i,n}(u) = (1-u)B_{i,n-1}(u) + u B_{i-1,n-1}(u)
    $$
    且约定当 $i < 0$ 或 $i > n$ 时，$B_{i,n}(u) = 0$。这一性质构成了 **De Casteljau 算法**的基础。

#### 3. De Casteljau 算法

