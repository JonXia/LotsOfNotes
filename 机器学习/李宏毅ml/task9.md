- 阅读《李航统计学习方法》的65-74页
  - 学习Gini指数
  - 学习回归树
  - 剪枝

1. CART算法

   1984年提出，1和2由此引入，CART算法同样由特征选择、树生成、剪枝组成，既可用来分类也可用于回归。

  - 分类树的生成
	本质是递归的构建二叉决策树，回归树用平方误差最小化准则，分类树用Gini指数选择最优特征，同时决定该特征的最优二值切分点。
	- 回归树的生成：
		![](https://imgconvert.csdnimg.cn/aHR0cHM6Ly91cGxvYWQtaW1hZ2VzLmppYW5zaHUuaW8vdXBsb2FkX2ltYWdlcy82OTYzODQ0LTBiMWQyMjBhZTM4NWM0YzYucG5n)
	停止条件可以为树深度等。
	- 分类树生成：
		![在这里插入图片描述](https://img-blog.csdnimg.cn/20190618195810806.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zOTUzNDM5OA==,size_16,color_FFFFFF,t_70)
	- Gini指数：分类问题中，假设有K个类，样本点属于第k类的概率为$p_{k}$，则概率分布的Gini指数定义为：
	$Gini(p)=\sum_{k=1}^{K}p_{k}(1-p_{k})=1-\sum_{k=1}^{K}p_{k}^{2}$
	对于二分类问题：
	$Gini(p)=2p(1-p)$
	对于给定的样本集合D：
	$Gini(p)=1-\sum_{k=1}^{K}\left ( \frac{\left | C_{k} \right |}{|D|} \right )^{2}$
	如果样本集合D根据特征A是否为a被分割成D1和D2，即：
	$D_{1}=\{ (x,y)\in D|A(x)=a)\},D_{2}=D-D_{1}$
	则在特征A的条件下，集合D的基尼指数：
	$Gini(D,A)=\frac{D_{1}}{D}Gini(D_{1})+\frac{D_{2}}{D}Gini(D_{2}))$
    Gini$Gini(D)$指数表示集合D的不确定性，Gini(D,A)表示经过A=a分割后D的不确定性。**Gini指数越大表示不确定性越大**。
    - 下图显示二分类问题中Gini指数、1/2熵和分类误差率的关系。横坐标表示概率，纵坐标表示损失。可以看出基尼指数和熵之半的曲线很接近，可以近似地表示分类误差率。![在这里插入图片描述](https://img-blog.csdnimg.cn/20190618192324924.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zOTUzNDM5OA==,size_16,color_FFFFFF,t_70)

2. 剪枝
 - 剪枝：决策树过拟合风险很大，理论上可以完全分得开数据
 - 策略：
    - 预剪枝:一边建立树一边进行剪枝操作
    - 后剪枝:当建立完决策树后进行剪枝操作
 - 预剪枝：限制深度，叶子节点个数，叶子节点样本数，
 - 后剪枝：通过一定标准衡量
 决策树的剪枝往往通过极小化决策树整体的损失函数来实现，设树T的叶结点个数为|T|，t是树T的叶结点，该叶结点有$N_{t}$个样本点，其中k类的样本点有N_{tk}个，k=1,2,...,K，H_{t}(T)为叶结点t上的经验熵，$\alpha\geq 0$为参数，则决策树学习的损失函数可以定义为。
 $C_{a}(T)=\sum_{t=1}^{\left | T \right |}N_{t}H_{t}(T)+\alpha |T|$

$\sum_{t=1}^{\left | T \right |}N_{t}H_{t}(T)$表示模型对训练数据的预测误差，即模型与训练数据的拟合程度，|T|表示模型复杂度，参数$\alpha\geq0$控制两者之间的影响。
