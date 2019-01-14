## 1. Introduction
目前存在集中不同的机器学习算法，大概可以分为两种，一种是监督学习，另一种是无监督学习。
- 监督学习：给定已有正确的数据集，运用这些样本学习算法，算出更正确的结果。
    - 回归问题：算法的输出为连续的值，即定量输出
    - 分类问题：算法的输出为离散的值，即定性输出
- 无监督学习：没有正确答案，只有一个数据集，让算法从中找到某种结构，典型例子就是聚类算法。

    鸡尾酒晏例子：看图

    ![](http://www.ai-start.com/ml2014/images/743c1d46d4288f8884f0981d437a15c1.png)

用算法把声音和音乐分开，使用Octave仅需要一行代码：
```octave
[W,s,v] = svd((repmat(sum(x.*x,1),size(x,1),1).*x)*x');
```

### 2. 单变量线性回归(Linear Regression with One Variable)

房价例子（监督学习/回归问题）：给定正确的俄勒冈州波特兰市的住房价格数据集，预测Size条件下房价Price的值；看图：

![](http://www.ai-start.com/ml2014/images/8e76e65ca7098b74a2e9bc8e9577adfc.png)

假如这个回归问题的训练集如图所示：

![](http://www.ai-start.com/ml2014/images/44c68412e65e62686a96ad16f278571f.png)

那么，在这里，我们把这个问题标记如下：

- m代表训练集中实例的数量
- x代表特征/输入变量
- y代表目标变量/输出变量
- (x,y)代表训练集中的实例
- ![](https://latex.codecogs.com/gif.latex?%5Cleft%20%28%20x%5E%7B%28i%29%7D%2Cy%5E%7B%28i%29%7D%20%5Cright%20%29)代表第i个观察实例
- h代表学习算法的解决方案或函数也称为假设（hypothesis）

![](http://www.ai-start.com/ml2014/images/ad0718d6e5218be6e6fce9dc775a38e6.png)

这个是监督学习的工作方式，其中h根据x的输入得出y值，那么也可以说h是从x到y的函数映射

我将选择最初的使用规则h代表hypothesis，因而，要解决房价预测问题，我们实际上是要将训练集“喂”给我们的学习算法，进而学习得到一个假设h，然后将我们要预测的房屋的尺寸作为输入变量输入给h，预测出该房屋的交易价格作为输出变量输出为结果。那么，对于我们的房价预测问题，我们该如何表达 ？
一种可能的表达式为：![h_{\Theta }\left ( x \right )=\Theta _{0}+\Theta _{1}x](https://latex.codecogs.com/gif.latex?h_%7B%5CTheta%20%7D%5Cleft%20%28%20x%20%5Cright%20%29%3D%5CTheta%20_%7B0%7D&plus;%5CTheta%20_%7B1%7Dx)，因为只含有一个特征/输入变量，因此这样的问题叫作单变量线性回归问题。

