- 阅读《李航统计学习方法》中p55-p58页

 总结决策树模型结构

 理解决策树递归思想
- 阅读《李航统计学习》中p58-p63页

 学习信息增益

 学习信息增益率
- 阅读《李航统计学习》中p63-65页

 学习ID3算法优缺点

 学习C4.5算法优缺点
- 理解C4.5算法在ID3算法上有什么提升

 学习C4.5算法在连续值上的处理

 学习决策树如何生成
- 阅读《机器学习实战》中p37-p41页

 划分数据集代码
 ```python
 def splitDataSet(dataSet, axis, value):
   retDataSet = []
   for featVec in dataSet:
     if featVec[axis] == value:
       reducedFeatVec = featVec[:axis]
       reducedFeatVec.extend(featVec[axis+1:])
       retDataSet.append(reducedFeatVec)
   return retDataSet
 ```
 选择最好的数据集划分方式代码
 ```python
 def chooseBestFeatureToSplit(dataSet):
     numFeatures = len(dataSet[0]) - 1
     # 上节的代码，计算熵
     baseEntropy = calcShannonEnt(dataSet)
     bestInfoGain = 0.0; bestFeature = -1

     for i in range(numFeatures):
         featList = [example[i] for example in dataSet]
         uniqueVals = set(featList)
         newEntropy = 0.0

         for value in uniqueVals:
             subDataSet = splitDataSet(dataSet, i, value)
             prob = len(subDataSet)/float(len(dataSet))
             newEntropy += prob * calcShannonEnt(subDataSet)     
         infoGain = baseEntropy - newEntropy

         if (infoGain > bestInfoGain):
             bestInfoGain = infoGain
             bestFeature = i
     return bestFeature

 ```
 创建树的函数代码
 ```python
 def createTree(dataSet, labels):
     classList = [example[-1] for example in dataSet]
     if classList.count(classList[0]) == len(classList):
         return classList[0]
     if (len(dataSet[0]) == 1):
         return majorityCnt(classList)
     bestFeat = chooseBestFeatureToSplit(dataSet)
     bestFeatLabel = labels[bestFeat]
     myTree = {bestFeatLabel:{}}
     del(labels[bestFeat])
     featValues = [example[bestFeat] for example in dataSet]
     uniqueVals = set(featValues)
     for value in uniqueVals:
         subLabels = labels[:]
         myTree[bestFeatLabel][value] = createTree(splitDataSet(dataSet, bestFeat, value), subLabels)
     return myTree
 ```


1. 总结决策树模型结构

  决策树是一种基本的分类与回归方法，决策树呈树形结构，在分类问题中，表示基于特征对实例进行分类的过程。它可以认为是“if-then”规则的集合，也可以认为是定义在特征空间与类空间上的概率分布。

  其主要优点是模型具有可读性。分类速度快，学习时，利用训练数据根据损失函数最小化原则简历决策树模型。预测时，对新的数据，利用决策树模型进行分类。

  决策树学习通常包括3个步骤：特征选择、决策树的生成和决策树的修剪。这些方法论主要来源于id3,c4.5和cart算法。

| 算法	| 支持模型|	树结构	|特征选择|	连续值处理|	缺失值处理|	剪枝|
| --------   | ----   | --- |--- |--- |--- |--- |
| ID3| 	分类|	多叉树|	信息增益|	不支持|	不支持|	不支持|
| C4.5|	分类	|多叉树	|信息增益比	|支持	|支持	|支持|
| CART|	分类/回归|	二叉树|	基尼系数，均方差|	支持|	支持|	支持|

  - ID3:
   ID3由Ross Quinlan在1986年提出。ID3决策树可以有多个分支，但是不能处理特征值为连续的情况。决策树是一种贪心算法，每次选取的分割数据的特征都是当前的最佳选择，并不关心是否达到最优。在ID3中，每次根据“最大信息熵增益”选取当前最佳的特征来分割数据，并按照该特征的所有取值来切分建立子节点，也就是说如果一个特征有4种取值，数据将被切分4份，一旦按某特征切分后，该特征在之后的算法执行中，将不再起作用。

   ID3只有树生成，所以容易过拟合。

  - C4.5:
  C4.5是Ross Quinlan在1993年在ID3的基础上改进而提出的。ID3采用的信息增益度量存在一个缺点，它一般会优先选择有较多属性值的Feature,因为属性值多的Feature会有相对较大的信息增益?(信息增益反映的给定一个条件以后不确定性减少的程度,必然是分得越细的数据集确定性更高,也就是条件熵越小,信息增益越大).为了避免这个不足C4.5中是用信息增益比率(gain ratio)来作为选择分支的准则。信息增益比率通过引入一个被称作分裂信息(Split information)的项来惩罚取值较多的Feature。除此之外，C4.5还弥补了ID3中不能处理特征属性值连续的问题。但是，对连续属性值需要扫描排序，会使C4.5性能下降

  - CART:
  CART（Classification and Regression tree）分类回归树由L.Breiman,J.Friedman,R.Olshen和C.Stone于1984年提出。ID3中根据属性值分割数据，之后该特征不会再起作用，这种快速切割的方式会影响算法的准确率。CART是一棵二叉树，采用二元切分法，每次把数据切成两份，分别进入左子树、右子树。而且每个非叶子节点都有两个孩子，所以CART的叶子节点比非叶子多1。相比ID3和C4.5，CART应用要多一些，既可以用于分类也可以用于回归。CART分类时，使用基尼指数（Gini）来选择最好的数据分割的特征，gini描述的是纯度，与信息熵的含义相似。CART中每一次迭代都会降低GINI系数。下图显示信息熵增益的一半，Gini指数，分类误差率三种评价指标非常接近。回归时使用均方差作为loss function。基尼系数的计算与信息熵增益的方式非常类似

2. 决策树学习的算法通常是一个递归地选择最优特征，并根据该特征对训练数据进行分割，使得对各个子数据集有一个最好的分类的过程。这一过程对应着对特征空间的划分，也对应着决策树的构建。

  开始，构建根节点，将所有训练数据都放在根节点，选择一个最优特征，按照这一特征将训练数据集分割成子集，使得各个子集有一个在当前条件下最好的分类。如果这些子集已经能够被正确分类，那么构建叶子节点，并将这些子集分到所对应的叶子节点中去；如果还有子集不能被基本正确分类，那么就对这些子集选择最优的特征，继续对其进行分割，构建相应的节点。如此递归地进行下去，直至所有训练数据子集被基本正确分类，或者没有合适特征为止，最后每个子集都被分到叶子节点上，即都有了明确的分类，这就生成了决策树。

  但是以上方法不一定有好的泛化能力，我们需要对已生成的树自下而上进行剪枝，将树变得简单；具体的，就是去掉过于细分的节点，使其退回到父节点，甚至更高的节点。

决策树的生成只考虑局部最优，相对地，决策树的剪枝则考虑全局最优。

3. 信息增益和信息增益率

  信息增益(information gain)表示得知特征X的信息而使得类Y的信息的不确定性减少的程度。

  信息增益值的大小是相对于训练数据集而言的，并没有绝对意义。在分类问题困难时，也就是说在训练数据集的经验熵大的时候，信息增益值会变大。反之，信息增益值会变小。而使用信息增益率可以对这一问题进行校正。这是特征选择的另一准则。（公式之后补上）
