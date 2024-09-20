## XGBoost回归的原生代码实现

![](pics/note/xgb%E5%BB%BA%E6%A8%A1%E6%B5%81%E7%A8%8B.png)

特征工程要在转化成DMatrix类型之前做完，此类型不可修改

参数可以写在train等函数的参数列表里面，也可以在param字典里写，但会有警告

使用predict预测之后的数据格式是数组，可以直接使用sklearn的评估指标

以下是xgboost原生代码的关键方法：

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

### 其它参数

#### xgb根据objective的参数指定来判断任务类型

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

