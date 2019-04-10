```
前情提要：线上spark的jdk版本为1.7，而pmml要求jdk再1.8以上；所以通过pmml来进行从python->Java的跨平台机器学习模型上线的思路不通。
```

- 基于spark MLlib做支持分布式计算的机器学习
  - python：py 3.6

  - mlflow：是去年中旬databricks发布的项目，近一年来迭代很快现在已经到了0.9.0(2019.3.28)版本了；[点击进入官网](https://mlflow.org/docs/latest/index.html)，[点击进入博文](http://www.oreilly.com.cn/ideas/?p=1925)，[点击进入论文](https://cs.stanford.edu/~matei/papers/2018/ieee_mlflow.pdf)；官网上写的比较明确，它主要有三个特点：
    - 跟踪试验用于记录、比较参数和结果 (MLflow Tracking)。
    - 打包 ML 代码为可重用、可重复生产的形式，以与其他数据科学家分享，或者转入生产阶段 (MLflow Projects)。
    - 从不同的ML库中管理和部署models，到不同的model serving 和推理平台(MLflow Models)。

    个人认为mlflow的意义在于，给予机器学习流程一个相对统一的开发流程标准，并给调参过程的参数、结果、模型等做了记录，且有相应的web界面直观的展示。

  - pyspark：spark提供的python api。网上查资料说用python相较于scala性能较低，具体情况了解不多。

  - [spark MLLib](http://spark.apache.org/docs/latest/ml-guide.html)：spark的机器学习api库有两个：spark.mllib和spark.ml
    - spark.mllib基于RDD，所以支持分布式计算，但Mllib中也只包含能够在集群上运行良好的并行算法。
    - spark官方推荐使用spark.ml，spark.ml基于[Dataframe](https://www.jianshu.com/p/c0181667daa0)提供更高层次的api，它的逻辑接近sklearn，使用ml Pipeline api可以很方便、清晰地把数据处理，特征转换，正则化，和机器学习算法联合起来。

    spark官方文档说，未来会主要支持ml，mllib很有可能会被废弃。


**假如python语言不适合的话，应该是可以不用mlflow，直接用scala调用spark MLLib完成机器学习的模型开发、部署上线，但是python很多好用的包就不能用了**

进度：
  - 目前在spark分布式环境的模型训练、部署预测等工作没有完成，
  - mlflow ui有些bug，
  - 正在寻找更人性化的模型训练模式(jupyter notebook的方式，但这种方式仅限于训练，正式部署网上说会有性能问题)

环境：
  - centos7+anaconda3-5.2.0(python 3.6.5)
  - mlflow 0.9.0（说实话，这个东西有一些莫名其妙的bug，中文资料也少，只能看issue/google去解决）
