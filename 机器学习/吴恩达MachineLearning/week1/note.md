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

房价例子（监督学习）：给定正确的俄勒冈州波特兰市的住房价格数据集m，预测Size条件下房价Price的值；看图：

![](http://www.ai-start.com/ml2014/images/8e76e65ca7098b74a2e9bc8e9577adfc.png)

