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

