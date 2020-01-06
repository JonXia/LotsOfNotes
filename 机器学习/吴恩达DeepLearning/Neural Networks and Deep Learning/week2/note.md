[TOC] 

# Symbol stipulations

$(x,y)$ represented a single training example, $x \in \mathbb{R} ^ {n_x}, y \in { \{0,1\} }$;

m_train represented numbers of training set, m_train:$\{(x^{(1)},y^{(1)}),(x^{(2)},y^{(2)}),...,(x^{(m)},y^{(m)})\}$

Usually,we use matrix symbols below to represented dataset,

$$X=\begin{bmatrix}
...&  ...& ...& ...&\\ 
 x^{(1)}&  x^{(2)}& ...& x^{(m)}& \\
...&  ...& ...& ...&\\ 
\end{bmatrix}$$，$Y=\begin{bmatrix} y^{(1)},y^{(2)},...,y^{(m)} \end{bmatrix}$

# Logistic Regression

$\hat y=w^Tx+b$利用这种线性回归的方式预测二分类为题，很容易受到异常值影响；所以用sigmoid函数$\sigma(z)=\frac 1 {1+e^{-z}}$ ，将上式代入z中，得到值域在0-1之间的函数，图像如下。![](https://i.loli.net/2019/10/23/6jnFSgePNb3Mzh1.jpg)

式子为：$\hat y = \sigma(w^Tx+b)$

接下来调整$w$和$b$，注意，这个表示在吴恩达ml课中是不一样的。在神经网络中$b$通常代表了拦截器。

为了得到最合适的$w$,$b$，LossFunction可以这么写：$LossFunction:L(\hat y,y)=\frac 1 2(\hat y-y)^2$，虽然在表面上看，这个式子衡量了$w$,$b$的情况，但是在Logistic Regression中通常不这么做，利用梯度下降后会得到多个局部最优解，既得到了一个非凸(non-convex)问题。

在Logistic Regression中我们用这个式子：

$LossFunction:L(\hat y,y)=-(ylog\hat y+(1-y)log(1-\hat y))$

...衡量所有样本的CostFunction：

$CostFunction: J(w,b)=\frac 1 m \sum ^m _{i=1}L(\hat y ^{(i)},y^{(i)})=-\frac 1 m \sum ^m _{i=1}[y^{(i)}log\hat y^{(i)}+(1-y^{(i)})log(1-y^{(i)})]$

说明：

设：

$If, y=1: p(y|x)=\hat y$

$y=0:	p（y|x）=1-\hat y$

那么可以对上式整合：$p(y|x)=\hat y^y(1-\hat y)^{1-y}$，由于log是严格单调递增的函数，所以两边取对数，有$ylog\hat y+(1-y)log(1-\hat y)$，这就是$LossFunction$的负数，因为我们在Logistic Regression中的目标是最小化，所以会加一个负号；

对于多个样本，假设这些数据服从同一分布，其联合概率：

$p(m个labels)=\prod_{i=1}^m p(y^{(i)}|x^{(i)})$

求其最大值，等同于求其对数，则有：

$log p(labels in training set)=\sum_{i=1}^m logp(y^{(i)}|x^{(i)})=-\sum_{i=1}^mL(\hat y^{(i)},y^{(i)})$

即，用极大似然估计，即求一组参数使得式子得出最大值。

因为我们要最小化LossFunction，所以去掉负号，为了表达方便加上了$\frac 1 m$对LossFunction缩放。

## Gradient Descent

随机选择初始点，因为是凸函数，所以到最后都会得到全局最优/接近全局最优。简化CostFunction只有w来举例，有下图：

![](https://i.loli.net/2019/10/23/SH85OxACnZeDY9w.jpg)


$$
Repeat:\{
 w:= w-\alpha \frac {dJ(w)} {dw}
\}
$$
完整式如下
$$
Repeat:\{
 w:= w-\alpha \frac {dJ(w,b)} {dw}
   
 b:= b-\alpha \frac {dJ(w,b)} {db}
\}
$$



符号约定：

函数只有一个变量时$\frac {dJ(w)} {db}$；

多个变量时$\frac {\partial J(w,b)} {\partial b}$，用偏导数符号。

## Logistic Regression recap

用计算图实现Logistic Regression的梯度下降算法：

![](http://www.ai-start.com/dl2017/images/6403f00e5844c3100f4aa9ff043e2319.jpg)

计算导数，略。

### Logistic Regression on m examples & Vectorizing logistic regression's GD

![](http://www.ai-start.com/dl2017/images/505663d02e8120e30c3d8405f31a8497.jpg)

for循环形式如上，我们将使用向量化实现一次GD过程：

$
Z=w^T+b
 =np.dot(w.T,X)+b
$

$
A = \sigma(Z)
$

$
dZ=A-Y
$

$
dw=\frac 1 m \times dZ^T
$

$
db=\frac 1 m (np.sum(dZ))
$

$
w:=w-\alpha dw
$

$
b:=b-\alpha db
$

当然，一次GD肯定不够，必须要最外层的这个for循环，才能通过GD得到全局最优解。

