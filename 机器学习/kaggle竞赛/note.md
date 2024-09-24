## XGBoost回归的原生代码实现

![](pics/note/xgb%E5%BB%BA%E6%A8%A1%E6%B5%81%E7%A8%8B.png)

特征工程要在转化成DMatrix类型之前做完，此类型不可修改

参数可以写在train等函数的参数列表里面，也可以在param字典里写，但会有警告

使用predict预测之后的数据格式是数组，可以直接使用sklearn的评估指标

### 以下是xgboost原生代码的关键方法：

> *class* `xgboost.DMatrix`(data, label=None, *, weight=None, base_margin=None, missing=None, silent=False, feature_names=None, feature_types=None, nthread=None, group=None, qid=None, label_lower_bound=None, label_upper_bound=None, feature_weights=None, enable_categorical=False)
>
> *function* `xgboost.train`(*params, dtrain, num_boost_round=10, *, evals=None, obj=None, feval=None, maximize=None, early_stopping_rounds=None, evals_result=None, verbose_eval=True, xgb_model=None, callbacks=None, custom_metric=None)
>
> *function* `xgboost.cv`(*params, dtrain, num_boost_round=10, nfold=3, stratified=False, folds=None, metrics=(), obj=None, feval=None, maximize=None, early_stopping_rounds=None, fpreproc=None, as_pandas=True, verbose_eval=None, show_stdv=True, seed=0, callbacks=None, shuffle=True, custom_metric=None)

其中，方法`xgb.train`和`xgb.cv`的第一个参数`params`就是我们需要使用字典自定义的参数列表，第二个参数`dtrain`就是DMatrix结构的训练数据，第三个参数`num_boost_round`其实就等同于sklearn中的`n_estimators`，表示总共建立多少棵提升树，也就是提升过程中的迭代次数。

- 转化数据的格式

  例子：

```python
data_xgb = xgb.DMatrix(X,y) #xgboost.core.DMatrix
```

DMatrix会将特征矩阵与标签打包在同一个对象中，且一次只能转换一组数据。并且，我们无法通过索引或循环查看内部的内容，一旦数据被转换为DMatrix，就难以调用或修改了

- 写param

  一般来说params中会包含至少7~10个参数，如果我们在大型数据上使用xgboost，则可能会涉及到十几个参数或以上：

  ```py
  params = {"max_depth":5, "seed":1209}
  reg = xgb.train(params, data_xgb, num_boost_round=100)
  ```

  可以看出来**XGBoost不需要实例化，`xgb.train`函数包揽了实例化和训练的功能**，一行代码解决所有问题。同时，**XGBoost在训练时没有区分回归和分类器，它默认是执行回归算法**，因此当我们执行回归任务时，代码是最为简单的。

  在运行完`xgb.train`后，我们就已经训练完毕后，训练到的reg可以直接用于预测：

  ```py
  y_pred = reg.predict(data_xgb)
  ```

  由于输出的是数组，可以使用sklearn的评估指标评估：

  ```py
  from sklearn.metrics import mean_squared_error as MSE
  MSE(y,y_pred,squared=False) #RMSE当squared=False
  #对回归类算法，xgboost的默认评估指标是RMSE
  ```

- 使用xgb.cv交叉验证

  ```py
  params = {"max_depth":5,"seed":1412}
  result = xgb.cv(params,data_xgb,num_boost_round=100
                  ,nfold=5 #补充交叉验证中所需的参数，nfold=5表示5折交叉验证
                  ,seed=1412 #交叉验证的随机数种子，params中的是管理boosting过程的随机数种子
                 )
  ```

  

result会返回一个100行，4列的矩阵，格式为DataFrame。

该矩阵行数与迭代次数一致，当我们规定迭代次数为100时，这个矩阵就有100行，如果我们规定的迭代次数为10，这个矩阵就只会有10行。每一行代表了每次迭代后进行交叉验证的结果的均值，例如索引为0的行就表示迭代了一次时（刚建立第一棵树时），进行5折交叉验证的结果，最后一行的结果也就是当前模型迭代完毕后（建好了全部的`nun_boost_round`棵树时）输出的结果，

每次迭代后xgboost会执行5折交叉验证，并收集交叉验证上的训练集RMSE均值、训练集RMSE的标准差、测试集RMSE的均值、测试集RMSE的标准差，这些数据构成了4列数据。实际上，这个矩阵展示了每次迭代过后，进行5折交叉验证的结果，也展示出了随着迭代次数增多，模型表现变化的趋势，因此输出结果可以被用于绘制图像。

### 基本的参数

#### xgb根据objective的参数指定来判断任务类型和损失函数

`objective`参数中可以输入数十种不同的选项，常见的有：

- 用于回归
> - **reg:squarederror**：平方损失，即$\frac{1}{2}(y - \hat{y})^2$，其中1/2是为了计算简便<br><br>
> - **reg:squaredlogerror**：平方对数损失，即$\frac{1}{2}[log(\hat{y} + 1) - log(y + 1)]^2$，其中1/2是为了计算简便

- 用于分类
> - **binary:logistic**：二分类交叉熵损失，使用该损失时`predict`接口输出概率。如果你对该损失不熟悉，你需要学习逻辑回归算法。<br><br>
> - **binary:logitraw**：二分类交叉熵损失，使用该损失时`predict`接输出执行sigmoid变化之前的值<br><br>
> - **multi:softmax**：多分类交叉熵损失，使用该损失时`predict`接口输出具体的类别。如果你对该损失不熟悉，你需要学习AdaBoost与GBDT。<br><br>
> - **multi:softprob**：多分类交叉熵，适用该损失时`predict`接口输出每个样本每个类别下的概率

除此之外，还有众多用于排序算法、计数算法的损失函数。xgboost几乎适用于所有可微的损失函数，不同的损失函数会影响`predict`的输出，但却不会影响交叉验证方法`xgb.cv`的输出。当不填写任何内容时，参数`objective`的默认值为`reg:squarederror`。接下来我们来看看xgboost分类器的实现：

#### eval_matric，用于设置分类的评估指标

分类任务需要搭配这个参数指定损失函数，二分类一般是"eval_matric":"logloss"，多分类对应的是"eval_matric":"mlogloss","num_class":10

#### 预测与评估

二分类直接返回概率，类别需要自己转换

```py
y_pred_binary = clf_binary.predict(data_binary)
y_pred_multi = clf_multi.predict(data_multi)
y_pred_binary[:20] #二分类直接返回概率
y_pred_multi #多分类，选择`multi:softmax`时返回具体类别，也可以选择`multi:softprob`返回概率。
```

```py
from sklearn.metrics import accuracy_score as ACC #当返回具体类别时，可以使用准确率
from sklearn.metrics import log_loss as logloss #当返回概率时，则必须使用交叉熵损失
```

```py
(y_pred_binary > 0.5).astype("int")

ACC(y_binary,(y_pred_binary > 0.5).astype(int)) #对二分类计算准确率，则必须先转换为类别，这个数字一般不为1
logloss(y_binary,y_pred_binary) #只有二分类输出了概率，因此可以查看交叉熵损失
```

#### 交叉验证

分类算法与回归算法执行交叉验证的流程基本一致，但需要注意的是，当使用`xgb.train`时，我们会将评估指标参数`eval_matric`写在params中，在使用`xgb.cv`时，我们却需要将评估指标参数写在`xgb.cv`当中，否则有时候会报出警告。这是一个奇怪的bug，可能会随着xgboost库的迭代更新而消失。如果不介意警告，可以继续将评估指标写在params里的`eval_matric`参数下。在`xgb.cv`当中，我们需要将评估指标打包成元组，写在参数`metrics`内部，如下所示：

```py
params2 = {"seed":1412
           , "objective":"multi:softmax" #无论填写什么损失函数都不影响交叉验证的评估指标
           , "num_class":10}
result = xgb.cv(params2,data_multi,num_boost_round=100
                ,metrics = ("mlogloss") #交叉验证的评估指标由cv中的参数metrics决定，也支持多个指标，在result出现对应的表头
                ,nfold=5 #补充交叉验证中所需的参数，nfold=5表示5折交叉验证
                ,seed=1412 #交叉验证的随机数种子，params中的是管理boosting过程的随机数种子
               )
```

由于XGBoost是一个极其复杂的系统，因此这些参数并不是XGBoost全部的参数，但上面表格中的参数已经覆盖了95%你需要用到的参数，在理解这些参数的基础上，再使用XGBoost的其他参数也会相对容易。从下一节开始将讲解XGBoost使用贝叶斯优化的调参流程。

| 类型                   | 参数                                                         |
| ---------------------- | ------------------------------------------------------------ |
| **迭代过程/目标函数**  | **params**: eta, base_score, objective, <font color="green">**lambda, gamma, alpha, max_delta_step**</font><br>**xgb.train()**: num_boost_round |
| **弱评估器结构**       | **params**: max_depth, <font color="green">**booster, min_child_weight**</font> |
| **dart树**             | **params**: <font color="green">**sample_type, normalized_type, rate_drop, one_drop, skip_drop**</font> |
| **弱评估器的训练数据** | **params**: subsample, <font color="green">**sampling_method, colsamle_bytree, colsample_bylevel, colsample_bynode**</font> |
| **提前停止**           | **xgb.train()**: <font color="green">**early_stopping_rounds, evals**</font>, eval_metric |
| **其他**               | **params**: seed, <font color="green">**verbosity, scale_pos_weight, nthread**</font> |

### 参数列表

1.

$$Obj_k = \sum_{i=1}^Ml(y_i,\hat{y_i}) + \boldsymbol{\color{red}\gamma} T + \frac{1}{2}\boldsymbol{\color{red}\lambda}\sum_{j=1}^Tw_j^2 + \boldsymbol{\color{red}\alpha}\sum_{j=1}^Tw_j$$

不难发现，所有可以自由设置的系数都与结构风险有关，这三个系数也正对应着xgboost中的三个参数：`gamma`，`alpha`与`lambda`。



\- 参数`gamma`：乘在一棵树的叶子总量$T$之前，依照叶子总量对目标函数施加惩罚的系数，默认值为0，可填写任何[0, ∞]之间的数字。当叶子总量固定时，`gamma`越大，结构风险项越大；同时，当`gamma`不变时，叶子总量越多、模型复杂度越大，结构风险项也会越大。在以上两种情况下，目标函数受到的惩罚都会越大，因此***\*调大****`gamma`****可以控制过拟合\****，在$T$很大的时候$\gamma$才会有效果。<br><br>

\- 参数`alpha`与`lambda`：乘在正则项之前，依照叶子权重的大小对目标函数施加惩罚的系数，也就是正则项系数。`lambda`的默认值为1，`alpha`的默认值为0，因此xgboost默认使用L2正则化。通常来说，我们不会同时使用两个正则化，但我们也可以尝试这么做。$\sum_{j=1}^Tw_j$是当前树上所有叶子的输出值之和，因此当树上的叶子越多、模型复杂度越大时，$\sum_{j=1}^Tw_j$自然的数值自然会更大，因此当正则项系数固定时，模型复杂度越高，对整体目标函数的惩罚就越重。当$w$固定时，正则项系数越大，整体目标函数越大，因此***\*调大****`alpha`****或****`lambda`****可以控制过拟合\****。

在实际使用和调节这些参数时，要先考虑适合的参数范围（找出公式中哪些项影响较大，并对这项调参），否则再多的搜索也是无用。

总结一下，在整个迭代过程中，我们涉及到了如下参数：

| 类型                  | 参数                                                         |
| --------------------- | ------------------------------------------------------------ |
| **迭代过程/损失函数** | num_boost_round：集成算法中弱分类器数量，对Boosting算法而言为实际迭代次数<br><br>eta：Boosting算法中的学习率，影响弱分类器结果的加权求和过程<br><br><font color="green">**objective**</font>：选择需要优化的损失函数<br><br><font color="green">**base_score**</font>：初始化预测结果$H_0$的设置<br><br><font color="green">**max_delta_step**</font>：一次迭代中所允许的最大迭代值<br><br><font color="green">**gamma**</font>：乘在叶子数量前的系数，放大可控制过拟合<br><br><font color="green">**lambda**</font>：L2正则项系数，放大可控制过拟合<br><br><font color="green">**alpha**</font>：L1正则项系数，放大可控制过拟合 |

### 三大评估器与dart树

前面的树影响更大，后面的树影响没那么大（可以理解为做了些小修小补），但dart直接随机某些树的做法可以大幅提升抗过拟合的能力。

在这个过程中，我们涉及到以下的几个参数：

- 参数`rate_drop`：每一轮迭代时抛弃树的比例
> 设置为0.3，则表示有30%的树会被抛弃。只有当参数`booster`="dart"时能够使用，只能填写[0.0,1.0]之间的浮点数，默认值为0。
- 参数`one_drop`：每一轮迭代时至少有`one_drop`棵树会被抛弃
> 可以设置为任意正整数，例如`one_drop` = 10，则意味着每轮迭代中至少有10棵树会被抛弃。<br><br>
> 当参数`one_drop`的值高于`rate_drop`中计算的结果时，则按照`one_drop`中的设置执行Dropout。例如，总共有30棵树，`rate_drop`设置为0.3，则需要抛弃9棵树。但`one_drop`中设置为10，则一定会抛弃10棵树。当`one_drop`的值低于`rate_drop`的计算结果时，则按`rate_drop`的计算结果执行Dropout。
参数rate_drop和one_drop他们的抛弃树的值谁大就选择它，都是0的话就是gbdt了，one_drop=N，rate_drop=0，可以控制抛弃树越来越多的现象

- 参数`skip_drop`：每一轮迭代时可以不执行dropout的概率
> 即便参数`booster`='dart'，每轮迭代也有`skip_drop`的概率可以不执行Dropout，是所有设置的概率值中拥有最高权限的参数。该参数只能填写[0.0,1.0]之间的浮点数，默认值为0。当该参数为0时，则表示每一轮迭代都一定会抛弃树。如果该参数不为0，则有可能不执行Dropout，直接按照普通提升树的规则建立新的提升树。<br><br>
> 需要注意的是，`skip_drop`的权限高于`one_drop`。即便`one_drop`中有所设置，例如每次迭代必须抛弃至少10棵树，但只要`skip_drop`不为0，每轮迭代则必须经过`skip_drop`的概率筛选。如果`skip_drop`说本次迭代不执行Dropout，则忽略`one_drop`中的设置。
在抗过拟合过程中模型能力收到影响之后可以设置skip_drop=0.5左右的

- 参数`sample_type`：抛弃时所使用的抽样方法
> 填写字符串"uniform"：表示均匀不放回抽样。<br><br>
> 填写字符串"weighted"：表示按照每棵树的权重进行有权重的不放回抽样。<br><br>
> 注意，该不放回是指在一次迭代中不放回。**每一次迭代中的抛弃是相互独立的，因此每一次抛弃都是从所有树中进行抛弃**。上一轮迭代中被抛弃的树在下一轮迭代中可能被包括。
- 参数`normalize_type`：增加新树时，赋予新树的权重
> 当随机抛弃已经建好的树时，可能会让模型结果大幅度偏移，因此往往需要给与后续的树更大的权重，让新增的、后续的树在整体算法中变得更加重要。所以DART树在建立新树时，会有意地给与后续的树更大的权重。我们有两种选择：<br><br>
> 填写字符串"tree"，表示新生成的树的权重等于所有被抛弃的树的权重的均值。<br><br>
> 填写字符串"forest"，表示新生成的树的权重等于所有被抛弃的树的权重之和。
> 算法默认为"tree"，当我们的dropout比例较大，且我们相信希望给与后续树更大的权重时，会选择"forest"模式。

调dart是个很复杂的过程，通常只会调rate_drop，除非控制过拟合效果不好再用其他。如果dart树效果好再用dart树，否则推荐原生树。

你是否注意到，我们的两个参数`sample_type`与`normalize_type`都使用了概念“树的权重”，但我们在之前讲解XGBoost的基本流程时提到过，XGBoost并不会针对每一棵树计算特定的权重。这个**树的权重其实指的是整棵树上所有叶子权重之和**。那究竟是怎样让新增加的树的权重刚好就等于原本被抛弃的树的权重的均值或和呢？这就需要一个相对复杂的数学过程来进行解答了，如果你感兴趣，可以查看这一篇说明：https://xgboost.readthedocs.io/en/stable/tutorials/dart.html

当我们在应用的时候，这个点并不会对我们造成影响，只要知道参数如何使用即可。**同时，所有dart树相关的参数在原生代码与sklearn代码中都完全一致**。

```python
data_xgb = xgb.DMatrix(X,y)
params_dart = {"max_depth":5 ,"seed":1412, "eta":0.1
                  ,"booster":"dart","sample_type": "uniform"
                  ,"normalize_type":"tree"
                  ,"rate_drop": 0.2
                  ,"skip_drop": 0.5}
result_dart = xgb.cv(params_dart,data_xgb,num_boost_round=100
                ,nfold=5 #补充交叉验证中所需的参数，nfold=5表示5折交叉验证
                ,seed=1412 #交叉验证的随机数种子，params中的是管理boosting过程的随机数种子
               )


```

dart树抗过拟合效果比`gamma`、`lambda`等参数更强，不过在提升模型的测试集表现上，dart树还是略逊一筹，毕竟dart树会伤害模型的学习能力。

####  弱评估器的分枝

**与信息熵、基尼系数等可以评价单一节点的指标不同，结构分数只能够评估结构本身的优劣，不能评估节点的优劣**。

比如说，方案1中的树结构有更高的分数之和，方案2中的树结构的分数之和较低，所以方案1更好。但我们不能说，方案1中的左节点分数低，右节点分数高，所以右节点比左节点更好。因此，在XGBoost原始论文当中，**我们利用一棵树上所有叶子的结构分数之和来评估整棵树的结构的优劣**，分数越高则说明树结构质量越高，因此在原论文中，结构分数也被称为质量分数（quality score）。

控制结构复杂度，一剪枝，二控制数据

##### 1. 弱评估器的剪枝

- max_depth：作用有限，往往其他控制过拟合的参数调好了以后再往大调

- gamma：数学推导可得出`gamma`越大增益小于`gamma`的叶子结点都会被剪枝，越不容易过拟合，效果也越差

- lambda、alpha：正则化系数，`lambda`越大结构分数越小，``gamma`的效果会被放大，模型剪枝会更严格，所以`lambda`可以通过目标函数将模型学习的重点拉向结构风险，因此`lambda`具有双重扛过拟合能力。

  然而，当`alpha`越大时，结构分数会越大，参数`gamma`的力量会被缩小，模型整体的剪枝会变得更宽松。然而，`alpha`还可以通过目标函数将模型学习的重点拉向结构风险，因此`alpha`会通过放大结构分数抵消一部分扛过拟合的能力。整体来看，`alpha`是比`lambda`更宽松的剪枝方式。

在XGBoost当中，我们可以同时使用两种正则化，则结构分数为：

$$ Score_j = \frac{(\sum_{i \in j}g_i)^2 + \alpha}{\sum_{i \in j}h_i + \lambda}$$

\> 此时，影响模型变化的因子会变得过多，我们难以再从中找到规律，调参会因此变得略有困难。**当你感觉到L2正则化本身不足以抵抗过拟合的时候，可以使用L1+L2正则化的方式尝试调参**。

现在我们已经详细介绍了XGBoost当中最简单的弱评估器，按照CART树规则或DART树规则、使用结构分数增益进行分枝的树在XGBoost的系统中被称为“贪婪树”（Greedy Tree）。大部分时候我们都会使用贪婪树来运行XGBoost算法，但在XGBoost当中还有其他几种不同的建树模式，包括基于直方图的估计贪婪树（approx greedy tree）、快速直方图贪婪树（Fast Histogram Approximate Greedy Tree）、以及基于GPU运行的快速直方图贪婪树等内容。这些算法在XGBoost原始论文中占了较大篇幅，并且在后续的LightGBM算法中被发扬光大，我们将在LGBM算法中详细讲解直方图方法。在使用XGBoost时，我们将专注于贪婪树本身。

到这里，关于XGBoost弱评估器的内容就全部讲解完毕了，总结一下，我们学习了如下参数：

| 类型         | 参数                                                         |
| ------------ | ------------------------------------------------------------ |
| **弱评估器** | <font color="green">**booster**</font>：选择迭代过程中的弱评估器类型，包括gbtree，DART和线性模型<br><br><font color="green">**sample_type**</font>：DART树中随机抽样树的具体方法<br><br><font color="green">**rate_drop**</font>：DART树中所使用的抛弃率<br><br><font color="green">**one_drop**</font>：每轮迭代时至少需要抛弃的树的数量<br><br><font color="green">**skip_drop**</font>：在迭代中不进行抛弃的概率<br><br><font color="green">**normalized_type**</font>：根据被抛弃的树的权重控制新增树权重<br><br>max_depth：允许的弱评估器的最大深度<br><br><font color="green">**min_child_weight**：</font>（广义上）叶子节点上的最小样本权重/最小样本量<br><br><font color="green">**gamma**</font>：目标函数中叶子数量$T$的系数，同时也是分枝时所需的最小结构分数增益值<br><br><font color="green">**lambda**与**alpha**</font>：正则项系数，同时也位于结构分数的公式中，间接影响模型的剪枝<br><br><font color="green">**sample_type**</font>：对样本进行抽样具体方式<br><br><font color="green">**subsample**</font>：对样本进行抽样的具体比例<br><br><font color="green">**colsample_bytree, colsample_bylevel, colsample_bynode**</font>：在建树过程中对特征进行抽样的比例 |

需要注意的是，以上全部参数都需要被写在parmas中，没有任何需要写在`xgb.train`或`xgb.cv`中的参数，故而没有给大家呈现具体的代码。在后续调参章节中，我们将展示使用这些参数的代码。

### XGBoost的其他参数与方法

- 提前停止

  参数`early_stopping_rounds`：位于`xgb.train`方法当中。如果规定的评估指标不能连续`early_stopping_rounds`次迭代提升，那就触发提前停止。

- **模型监控与评估**
> 参数`evals`：位于`xgb.train`方法当中，用于规定训练当中所使用的评估指标，一般都与损失函数保持一致，也可选择与损失函数不同的指标。该指标也用于提前停止。<br><br>
> 参数`verbosity`：用于打印训练流程和训练结果的参数。在最早的版本中该参数为silent，后来经过更新变成了今天的verbosity。然而，经过改进之后的verbosity更倾向于帮助我们打印建树相关的信息，而不像原来的silent一样帮助我们展示训练过程中的模型评估信息，因此verbosity现在不那么实用了。
>> 我们可以在verbosity中设置数字[0,1,2,3]，参数默认值为1。<br>
>> - 0：不打印任何内容<br>
>> - 1：表示如果有警告，请打印警告<br>
>> - 2：请打印建树的全部信息<br>
>> - 3：我正在debug，请帮我打印更多的信息。

- ***\*样本不均衡\****

\> 参数`scale_pos_weight`：调节样本不均衡问题，类似于sklearn中的class_weight，仅在算法执行分类任务时有效。参数`scale_pos_weight`的值时负样本比正样本的比例，默认为1，因此XGBoost时默认调节样本不均衡的。同时，如果你需要手动设置这个参数，可以输入（负样本总量）/（正样本总量）这样的值。

## XGBoost的参数空间与超参数优化
