**前情提要：** 线上spark(1.6.0)的jdk版本为1.7，而pmml要求jdk再1.8以上；所以通过pmml来进行从python->Java的跨平台机器学习模型上线的思路不通。现在想解决的问题是搭建一个完整的、相对人性化的大数据机器学习工程流程，并且有一定的向后兼容性，在技术更迭的时候不至于大改。

### 基于spark MLlib做支持分布式计算的机器学习
  - python：py 3.6

  - [spark MLLib](http://spark.apache.org/docs/latest/ml-guide.html)：spark的机器学习api库有两个：spark.mllib和spark.ml
    - spark.mllib基于RDD将一些机器学习算法改写，所以支持分布式计算，**但Mllib中也只包含能够在集群上运行良好的基础机器学习算法。**
    - spark官方推荐使用spark.ml，spark.ml基于[Dataframe](https://www.jianshu.com/p/c0181667daa0)提供更高层次的api，它的逻辑接近sklearn，使用ml Pipeline api可以很方便、清晰地把数据处理，特征转换，正则化，和机器学习算法联合起来。**关键是可以把Dataframe转成pandas的DataFrame。**

    spark官方文档说，未来会主要支持ml，mllib很有可能会被废弃，查看官网mllib升级的历史纪录发现某些api有很大的改动，说明**向上兼容并不好，低版本的代码并不会适用于更高版本。** <font size="3" color="red">总之mllib能做基础机器学习，但是是有一些局限性的。</font>

  - mlflow：是去年中旬databricks发布的项目，近一年来迭代很快现在已经到了0.9.0(2019.3.28)版本了；[点击进入官网](https://mlflow.org/docs/latest/index.html)，[点击进入博文](http://www.oreilly.com.cn/ideas/?p=1925)，[点击进入论文](https://cs.stanford.edu/~matei/papers/2018/ieee_mlflow.pdf)；官网上写的比较明确，它主要有三个特点：
    - 跟踪试验用于记录、比较参数和结果 (MLflow Tracking)。
    - 打包 ML 代码为可重用、可重复生产的形式，以与其他数据科学家分享，或者转入生产阶段 (MLflow Projects)。
    - 从不同的ML库中管理和部署models，到不同的model serving 和推理平台(MLflow Models)。

    个人认为mlflow的意义在于，给予机器学习流程一个相对统一的开发流程标准，并给调参过程的参数、结果、模型等做了记录，且有相应的web界面直观的展示。

  - pyspark：spark提供的python api。网上查资料说用python相较于scala性能较低，具体情况了解不多。

**假如python语言不适合的话，应该是可以不用mlflow，直接用scala调用spark MLLib完成机器学习的模型开发、部署上线，但是python的优势就不能发挥出来了**

[scala和python的对比](http://www.cnblogs.com/andre-ma/p/8539098.html)



进度：
  - 学习Scala、spark算子等（长期任务，买本书《Spark 大数据集群计算的生产实践》）
  - 训练模式
    - Scala：构建Scala项目打包丢在服务器上跑，看输出日志。（迭代太麻烦，已实现）
    - python的话，用mlflow ui管理机器学习训练过程，可以通过python脚本部署restapi跑，也可以通过jupyter notebook远程（jupyter待实现）。
  - TODO：spark流式机器学习与规则引擎实现

目前环境：
  - 集群（以spark1.6为主）
    - jdk7
    - centos7+anaconda3-5.2.0(python 3.6.5)
    - mlflow 0.9.0
    - CDH5.15.2(spark1.6+hadoop2.6.x)
  - standalone
    - python3.5
    - jdk7
    - spark1.6+

### 10w条数据的观测结果：
  总的来说，这些特征的离群值较多，0值很多，关键是不能判断出这些异常值是否是正样本（诈骗），[箱型图](https://blog.csdn.net/clairliu/article/details/79217546)的表现不好，大多数特征都不能有一个相对漂亮的箱型图，证明了异常值很多，但是这些应该可以用对数缩放、限制特征值等特征工程方法去解决，但是需要时间去研究。

  - 特征相关性（线性）图分析：
    流量、短信等特征和其他相关性不大，在这个数据集里可以看作干扰特征，但是感觉还是能挖掘出点啥
  - 均值比较结论：
    通话相关特征诈骗用户都明显大于正常用户，短信数量和正常用户差不多*，流量时长诈骗用户都明显小于正常用户，基站数量诈骗用户都明显小于正常用户
  - 逻辑回归系数分析：calltime通话时长，flowsize流量大小,flowduration流量时长，msgcount短信数量系数大小较低（为0），但回归显著性为0（说明具有一定的意义），上述特征对模型有较小的影响。
  - 1w条正样本数据通话相关特征的0值较多，无参考意义

    ps：*诈骗数据集有两个，其中一个数据集的均值明显高于另一个。*<br>
    pps：两个诈骗数据集和10w个正常用户数据集都未经过处理
