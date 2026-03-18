# NURBS BOOK 笔记

## 目录

- 1 曲线曲面基础知识

    - [1.1 隐函数与参数函数](#11-隐函数与参数函数)

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