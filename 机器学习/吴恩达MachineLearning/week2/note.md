# 1. 多变量线性回归(Linear Regression with Multiple Variables)

## 1.1 多维特征
目前为止，我们探讨了单变量/特征的回归模型，现在我们对房价模型增加更多的特征，例如房间数楼层等，构成一个含有多个变量的模型，模型中的特征为![](https://latex.codecogs.com/gif.latex?\left(&space;{x_{1}},{x_{2}},...,{x_{n}}&space;\right))。

话不多说一图流：

<img width=500  src="pics/indicate_multiplefeatures.png">

- n为特征的数量
- ![](https://latex.codecogs.com/gif.latex?{x^{\left(&space;i&space;\right)}})代表第 ![i](https://latex.codecogs.com/gif.latex?i) 个训练实例，是特征矩阵中的第![i](https://latex.codecogs.com/gif.latex?i)行，是一个**向量**（**vector**）。

比方说，上图

- ![](https://latex.codecogs.com/gif.latex?{x}^{(2)}\text{=}\begin{bmatrix}&space;1416\\\&space;3\\\&space;2\\\&space;40&space;\end{bmatrix})

![](https://latex.codecogs.com/gif.latex?{x}_{j}^{\left(&space;i&space;\right)})代表特征矩阵中第 ![i](https://latex.codecogs.com/gif.latex?i)行的第 ![j](https://latex.codecogs.com/gif.latex?j) 个特征，也就是![i](https://latex.codecogs.com/gif.latex?i)第  个训练实例的第 ![j](https://latex.codecogs.com/gif.latex?j) 个特征，如上图的![x_{2}^{\left( 2 \right)}=3,x_{3}^{\left( 2 \right)}=2](https://latex.codecogs.com/gif.latex?x_{2}^{\left(&space;2&space;\right)}=3,x_{3}^{\left(&space;2&space;\right)}=2)；

- 多维特征的假设![h](https://latex.codecogs.com/gif.latex?h)表示为：![h_{\theta}\left( x \right)={\theta_{0}}+{\theta_{1}}{x_{1}}+{\theta_{2}}{x_{2}}+...+{\theta_{n}}{x_{n}}](https://latex.codecogs.com/gif.latex?h_{\theta}\left(&space;x&space;\right)={\theta_{0}}&plus;{\theta_{1}}{x_{1}}&plus;{\theta_{2}}{x_{2}}&plus;...&plus;{\theta_{n}}{x_{n}})，
那么假设有![x_{0}^{(i)}=1](https://latex.codecogs.com/gif.latex?x_{0}^{(i)}=1)，那么上式就可以转换为![](https://latex.codecogs.com/gif.latex?h_{\theta}&space;\left(&space;x&space;\right)={\theta_{0}}{x_{0}}&plus;{\theta_{1}}{x_{1}}&plus;{\theta_{2}}{x_{2}}&plus;...&plus;{\theta_{n}}{x_{n}})，
那么把![西塔](https://latex.codecogs.com/gif.latex?\theta)转成![](https://latex.codecogs.com/gif.latex?%5Ctheta%5E%7BT%7D)，即这个n+1维的行向量与n+1维的X相乘，即![](https://latex.codecogs.com/gif.latex?h_{\theta}&space;\left(&space;x&space;\right)={\theta^{T}}X)
话不多说再加个图：

<img width=500  src="pics/indicate_multiplefeatures2.png">

##  2.2 多变量梯度下降

<img width=500  src="pics/gradientdescentformultivars1.png"><br>
如图所示，我们假设$\theta_0$项乘以一个$x_0$，那么，常数参数$\theta_0$, $\theta_1$...可以看作是一个向量$\theta$ ，那么看图，梯度下降算法的第二项中被框起来的部分，可以看作是求以向量$\theta$为参数的代价函数的偏导数

即：<br>
![](http://www.ai-start.com/ml2014/images/6bdaff07783e37fcbb1f8765ca06b01b.png)

求偏导后得：

![](http://www.ai-start.com/ml2014/images/dd33179ceccbd8b0b59a5ae698847049.png)
## 2.3 梯度下降技巧——特征缩放（feature scalling）

**在我们面对多维特征问题的时候，我们要保证这些特征都具有相近的尺度，这将帮助梯度下降算法更快地收敛。**

以房价问题为例，假设我们使用两个特征，房屋的尺寸和房间的数量，尺寸的值为 0-2000平方英尺，而房间数量的值则是0-5，以两个参数分别为横纵坐标，绘制代价函数的等高线图能，看出图像会显得很扁，梯度下降算法需要非常多次的迭代才能收敛。

![](http://www.ai-start.com/ml2014/images/966e5a9b00687678374b8221fdd33475.jpg)

解决的方法是尝试将所有特征的尺度都尽量缩放到-1到1之间。如图：

![](http://www.ai-start.com/ml2014/images/b8167ff0926046e112acf789dba98057.png)


最简单的方法是令：![{{x}_{n}}=\frac{{{x}_{n}}-{{\mu}_{n}}}{{{s}_{n}}}](https://latex.codecogs.com/gif.latex?{{x}_{n}}=\frac{{{x}_{n}}-{{\mu}_{n}}}{{{s}_{n}}})，其中 ![{\mu_{n}}](https://latex.codecogs.com/gif.latex?{\mu_{n}})是平均值，![{s_{n}}](https://latex.codecogs.com/gif.latex?{s_{n}})是标准差。

## 2.4 梯度下降技巧——学习率
虽然梯度下降算法在理论上，代价函数可以自行收敛到最小，但是学习率α也是决定收敛效果如何的关键因素。

梯度下降算法的每次迭代受到学习率的影响，如果学习率过小，则达到收敛所需的迭代次数会非常高；如果学习率过大，每次迭代可能不会减小代价函数，可能会越过局部最小值导致无法收敛。

正常情况，代价函数J(θ)的变化情况大体如下图所示：

![](http://www.ai-start.com/ml2014/images/cd4e3df45c34f6a8e2bb7cd3a2849e6c.jpg)

贴一张直观点的图

![](https://ws2.sinaimg.cn/large/006tNc79ly1flvq8946ekj30cr0bi3yp.jpg)

对于梯度下降何时收敛，我们进行自动收敛测试，也就是用一种算法，告诉你这个梯度下降算法何时收敛，举一个例子，如果J(θ)小于某些阈值，比如0.001、0.01、0.1等时，可以判断梯度下降算法收敛，但是这个阈值很难决定用哪些。

所以，还是看J(θ)的变化曲线，并且这个曲线也可以提前报告于你这个算法的运行情况，参考上图的描述，改变α，课程中α的值给的例子是![](https://latex.codecogs.com/gif.latex?\alpha=...0.001,0.003,0.01,0.03,0.1,0.3,1...)

## 2.5 特征与多项式回归

举例，还是房价问题

![](http://www.ai-start.com/ml2014/images/8ffaa10ae1138f1873bc65e1e3657bd4.png)

假设h(θ)如下，![h_{\theta}\left( x \right)={\theta_{0}}+{\theta_{1}}\times{frontage}+{\theta_{2}}\times{depth}](https://latex.codecogs.com/gif.latex?h_{\theta}\left(&space;x&space;\right)={\theta_{0}}&plus;{\theta_{1}}\times{frontage}&plus;{\theta_{2}}\times{depth})

注意frontage和depth，这两个特征其实可以化成一个特征，即把这两个相乘为面积（![x=frontage*depth=area](https://latex.codecogs.com/gif.latex?x=frontage*depth=area)），那么假设h(θ)为：

![{h_{\theta}}\left( x \right)={\theta_{0}}+{\theta_{1}}x](https://latex.codecogs.com/gif.latex?{h_{\theta}}\left(&space;x&space;\right)={\theta_{0}}&plus;{\theta_{1}}area)

所以，灵活利用特征的特征很重要

上例给出数据集如下：

![](http://www.ai-start.com/ml2014/images/3a47e15258012b06b34d4e05fb3af2cf.jpg)

对于这段数据，我们可以给出二次模型来拟合，但是二次函数曲线在size(即area)达到一定数值后就回下降，这显然不符合常理，所以，可以使用三次函数：![h_{\theta}\left( x \right)={\theta_{0}}+{\theta_{1}}{x_{1}}+{\theta_{2}}{x_{2}^2}+{\theta_{3}}{x_{3}^3}](https://latex.codecogs.com/gif.latex?h_{\theta}\left(&space;x&space;\right)={\theta_{0}}&plus;{\theta_{1}}{x_{1}}&plus;{\theta_{2}}{x_{2}^2}&plus;{\theta_{3}}{x_{3}^3})

根据函数图形特性，我们还可以使：

![{{{h}}_{\theta}}(x)={{\theta }_{0}}\text{+}{{\theta }_{1}}(size)+{{\theta}_{2}}{{(size)}^{2}}](https://latex.codecogs.com/gif.latex?{{{h}}_{\theta}}(x)={{\theta&space;}_{0}}\text{&plus;}{{\theta&space;}_{1}}(size)&plus;{{\theta}_{2}}{{(size)}^{2}})

或者:

![{{{h}}_{\theta}}(x)={{\theta }_{0}}\text{+}{{\theta }_{1}}(size)+{{\theta }_{2}}\sqrt{size}](https://latex.codecogs.com/gif.latex?{{{h}}_{\theta}}(x)={{\theta&space;}_{0}}\text{&plus;}{{\theta&space;}_{1}}(size)&plus;{{\theta&space;}_{2}}\sqrt{size})

以期达到这样的效果：

<img width=500  src="pics/PolynomialRegression_better_model.png">

上例总结如下：

<img width=500  src="pics/PolynomialRegression_h(θ)smodel.png">

如果我们采用多项式回归模型，特征的归一化是必要的，这样才能使数据变得有可比性。

## 2.6 正规方程

某些时候用正规方程是更好的解决方案；如下图：假设这是一个关于实数θ的二次函数J(θ)

![](http://www.ai-start.com/ml2014/images/a47ec797d8a9c331e02ed90bca48a24b.png)

那么我们最小化这个二次函数的方法是对它求导，令导数=0之后求出的就是令J(θ)最小的θ值。

**推导过程**

![\theta ={{\left( {X^{T}}X \right)}^{-1}}{X^{T}}y](https://latex.codecogs.com/gif.latex?\theta&space;={{\left(&space;{X^{T}}X&space;\right)}^{-1}}{X^{T}}y) 的推导过程：

![](https://latex.codecogs.com/gif.latex?J\left(&space;\theta&space;\right)=\frac{1}{2m}\sum\limits_{i=1}^{m}{{{\left(&space;{h_{\theta}}\left(&space;{x^{(i)}}&space;\right)-{y^{(i)}}&space;\right)}^{2}}})
其中：![](https://latex.codecogs.com/gif.latex?{h_{\theta}}\left(&space;x&space;\right)={\theta^{T}}X={\theta_{0}}{x_{0}}&plus;{\theta_{1}}{x_{1}}&plus;{\theta_{2}}{x_{2}}&plus;...&plus;{\theta_{n}}{x_{n}})

将向量表达形式转为矩阵表达形式，则有![](https://latex.codecogs.com/gif.latex?J(\theta&space;)=\frac{1}{2}{{\left(&space;X\theta&space;-y\right)}^{2}}) ，其中![](https://latex.codecogs.com/gif.latex?x)为![](https://latex.codecogs.com/gif.latex?m)行![](https://latex.codecogs.com/gif.latex?n)列的矩阵（![](https://latex.codecogs.com/gif.latex?m)为样本个数，![](https://latex.codecogs.com/gif.latex?n)为特征个数），![](https://latex.codecogs.com/gif.latex?\theta)为![](https://latex.codecogs.com/gif.latex?n)行1列的矩阵，![](https://latex.codecogs.com/gif.latex?y)为![](https://latex.codecogs.com/gif.latex?m)行1列的矩阵，对![](https://latex.codecogs.com/gif.latex?J(\theta))进行如下变换

![](https://latex.codecogs.com/gif.latex?J(\theta&space;)=\frac{1}{2}{{\left(&space;X\theta&space;-y\right)}^{T}}\left(&space;X\theta&space;-y&space;\right))

​     ![](https://latex.codecogs.com/gif.latex?=\frac{1}{2}\left(&space;{{\theta&space;}^{T}}{{X}^{T}}-{{y}^{T}}&space;\right)\left(X\theta&space;-y&space;\right))

​     ![](https://latex.codecogs.com/gif.latex?=\frac{1}{2}\left(&space;{{\theta&space;}^{T}}{{X}^{T}}X\theta&space;-{{\theta}^{T}}{{X}^{T}}y-{{y}^{T}}X\theta&space;-{{y}^{T}}y&space;\right))

接下来对![](https://latex.codecogs.com/gif.latex?J(\theta))偏导，需要用到以下几个矩阵的求导法则:

![](https://latex.codecogs.com/gif.latex?\frac{dAB}{dB}={{A}^{T}})

![](https://latex.codecogs.com/gif.latex?\frac{d{{X}^{T}}AX}{dX}=2AX)

所以有:

![](https://latex.codecogs.com/gif.latex?\frac{\partial&space;J\left(&space;\theta&space;\right)}{\partial&space;\theta&space;}=\frac{1}{2}\left(2{{X}^{T}}X\theta&space;-{{X}^{T}}y&space;-{}({{y}^{T}}X&space;)^{T}-0&space;\right))

![](https://latex.codecogs.com/gif.latex?=\frac{1}{2}\left(2{{X}^{T}}X\theta&space;-{{X}^{T}}y&space;-{{X}^{T}}y&space;-0&space;\right))

​           ![](https://latex.codecogs.com/gif.latex?={{X}^{T}}X\theta&space;-{{X}^{T}}y)

令![](https://latex.codecogs.com/gif.latex?\frac{\partial&space;J\left(&space;\theta&space;\right)}{\partial&space;\theta&space;}=0),

则有![](https://latex.codecogs.com/gif.latex?\theta&space;={{\left(&space;{X^{T}}X&space;\right)}^{-1}}{X^{T}}y)

梯度下降与正规方程的比较：

| 梯度下降             | 正规方程                                     |
| ---------------- | ---------------------------------------- |
| 需要选择学习率![](https://latex.codecogs.com/gif.latex?\alpha)  | 不需要                                      |
| 需要多次迭代           | 一次运算得出                                   |
| 当特征数量![](https://latex.codecogs.com/gif.latex?n)大时也能较好适用 | 需要计算![](https://latex.codecogs.com/gif.latex?{{\left(&space;{{X}^{T}}X&space;\right)}^{-1}}) 如果特征数量n较大则运算代价大，因为矩阵逆的计算时间复杂度为![](https://latex.codecogs.com/gif.latex?O\left(&space;{{n}^{3}}&space;\right))，通常来说当![](https://latex.codecogs.com/gif.latex?n)小于10000 时还是可以接受的 |
| 适用于各种类型的模型       | 只适用于线性模型，不适合逻辑回归模型等其他模型                  |

总结一下，只要特征变量的数目并不大，标准方程是一个很好的计算参数![](https://latex.codecogs.com/gif.latex?\theta)的替代方法。具体地说，只要特征变量数量小于一万，通常使用正规方程法，而不使用梯度下降法。

随着我们要讲的学习算法越来越复杂，例如，当我们讲到分类算法，像逻辑回归算法，我们会看到，实际上对于那些算法，并不能使用标准方程法。