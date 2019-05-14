- 了解什么是Machine learning

  人工智能是我们想要达成的目标，机器学习是我们达成目标的手段。机器学习是一门讨论各式各样的适用于不同问题的函数形式，以及如何使用数据来有效地获取函数参数具体值的学科。

  ![](https://user-gold-cdn.xitu.io/2019/5/13/16ab0c33ea12b99d?w=772&h=578&f=png&s=316689)

- 学习中心极限定理，学习正态分布，学习最大似然估计
  - 中心极限定理

    样本的平均值约等于总体的平均值。 不管总体是什么分布，任意一个总体的样本平均值都会围绕在总体的整体平均值周围，并且呈正态分布。

  - 正态分布

    正态分布（英语：normal distribution）又名高斯分布（英语：Gaussian distribution），是一个非常常见的连续概率分布。正态分布在统计学上十分重要，经常用在自然和社会科学来代表一个不明的随机变量。

    若随机变量$X$服从一个位置参数为$mu$、尺度参数为$sigma$的正态分布，记为：

    $$X\thicksim N(\mu , \sigma)$$

    则其概率密度函数为：

    $$f(x)=\frac{1}{\sqrt{2\pi}\sigma}exp(- \frac{(x-\mu)^2}{2\sigma^2})$$

    正态分布的数学期望值或期望值 $\mu $等于位置参数，决定了分布的位置；其方差$\sigma^2$的开平方或标准差 $\sigma $等于尺度参数，决定了分布的幅度。

  - 极大似然估计

      最大似然函数提供了一种给定观察数据来估计模型参数的方法，即“模型已定，参数未知”，也就说在给定概率的情况下我们可以反推出参数。

  - 推导回归Loss function

      常用的损失函数有：均方误差（Mean Squared Error）、自定义和交叉熵（Cross Entropy）等。

      均方误差（Mean Squared Error）：n个样本的预测值$\hat{y_{i}}$与已知值$f(x_{i})$之差的平方和，再求平均值。表达式如下：

      $L(f) = \frac{1}{n} \sum_{i=1}^n \left[\left( \hat{y_i}f(x_i) \right) ^2\right]$

      基于均方误差最小化来进行模型求解的方法称为最小二乘法(Least Square Method)。在线性回归中，最小二乘法就是试图找到一条直线，使得所有样本到直线上的欧式距离之和最小。

  - 损失函数与凸函数

      凸函数是有且只有全局最优解的，而非凸函数可能有多个局部最优解。例如针对逻辑回归、线性回归这样的凸函数，使用梯度下降或者牛顿法可以求出参数的全局最优解，针对神经网络这样的非凸函数，我们可能会找到许多局部最优解。  

  - 全局最优和局部最优  

      函数有多个凹谷时，普通迭代可能会陷入局部最优
      ![](https://user-gold-cdn.xitu.io/2019/5/13/16ab0f089c6c47df?w=600&h=176&f=jpeg&s=16868)

- 学习导数，泰勒展开
  $$
  P(x)=f\left(x_{0}\right)+f^{\prime}\left(x_{0}\right)\left(x-x_{0}\right)+\frac{f^{(2)}\left(x_{0}\right)}{2 !}\left(x-x_{0}\right)^{2}+\cdots+\frac{f^{(n)}\left(x_{0}\right)}{n !}\left(x-x_{0}\right)^{n}
  $$


- 推导梯度下降公式
  ![](https://user-gold-cdn.xitu.io/2019/5/13/16ab100963f21152?w=384&h=371&f=png&s=47142)


- 写出梯度下降的代码

  ```python
  #梯度下降法实现
  # numpy实现
  import numpy as np
  x = np.array([1,2,3])
  y = np.array([2,4,6])

  epoches = 10
  lr = 0.1
  w = 0
  cost=[]

  for epoch in range(epoches):
      yhat = x*w
      loss = np.average((yhat-y)**2)
      cost.append(loss)
      dw = -2*(y-yhat)@ x.T/(x.shape[0])
      w=w-lr*dw
      print(w)
  ```

- 学习L2-Norm，L1-Norm，L0-Norm

  为了防止过拟合，对参数进行正则化。将正则化项加入到损失函数中一起优化，约束参数取值不要过大。

   **L0范数是指向量中非0的元素的个数**。如果我们用L0范数来规则化一个参数矩阵W的话，就是希望W的大部分元素都是0。换句话说，让参数W是稀疏的。

  **L1范数是指向量中各个元素绝对值之和**。L1适用于特征选择，可解释性。

  **L2范数是指向量各元素的平方和然后求平方根**。我们让L2范数的规则项||W||2最小，可以使得W的每个元素都很小，都接近于0,平滑，平滑的function对异常值不敏感，但不能平滑成一条水平线，$\lambda$越大，越考虑$w$。

  L1会趋向于产生少量的特征，而其他的特征都是0，而L2会选择更多的特征，这些特征都会接近于0。

  ![这里写图片描述](https://user-gold-cdn.xitu.io/2019/5/13/16ab07c7a43073f4?w=416&h=264&f=png&s=30360)

- 推导正则化公式

  ```python
  # L1,L2,elastic-net正则化
  def l1_normal(x):
      return torch.abs(x).sum()
  def l2_normal(x):
      return torch.pow(x,2).sum()
  def elastic_net(x,a,b):
      return l1_normal(x)*a+l2_normal(x)*b
  ```



- 说明为什么用L1-Norm代替L0-Norm

  L1范数是L0范数的最优凸近似。任何的规则化算子，如果他在Wi=0的地方不可微，并且可以分解为一个“求和”的形式，那么这个规则化算子就可以实现稀疏。W的L1范数是绝对值，|w|在w=0处是不可微。 虽然L0可以实现稀疏，但是实际中会使用L1取代L0。因为L0范数很难优化求解，L1范数是L0范数的最优凸近似，它比L0范数要容易优化求解。

- 学习为什么只对w/Θ做限制，不对b做限制

  因为b影响的式整体的平移，并不影响函数拟合的程度及模型的复杂性。
