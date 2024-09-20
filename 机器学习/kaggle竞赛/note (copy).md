## LightGBM

lgbm在计算效率和内存占用都有显著的优势

是第一个使用直方图近似的BGDT算法，XGBoost-hist则是受LightGBM启发产生的算法。LGBM也是处理海量数据最高效、通用的机器学习算法。

论文推荐：

- &emsp;&emsp;[LightGBM: A Highly Efficient Gradient Boosting Decision Tree (2017)](https://papers.nips.cc/paper/6907-lightgbm-a-highly-efficient-gradient-boosting-decision-tree)        
  &emsp;&emsp;作者：Guolin Ke, Qi Meng, Thomas Finley, Taifeng Wang, Wei Chen, Weidong Ma, Qiwei Ye, Tie-Yan Liu      
  &emsp;&emsp;该论文是 LightGBM 的最初论文，详细阐述了 LightGBM 算法的设计思想、技术特点和实验结果。

- &emsp;&emsp;[Comparison between XGBoost, LightGBM and CatBoost using a home credit dataset (2019)](https://publications.waset.org/10009954/comparison-between-xgboost-lightgbm-and-catboost-using-a-home-credit-dataset)      
  &emsp;&emsp;作者：Essam Al Daoud      
  &emsp;&emsp;该论文详细对比了LGBM、XGB和CatB三个模型在信用卡数据上的性能差异，并提出了不同模型的超参数优化基本思路。

&emsp;&emsp;LightGBM的官方文档：https://lightgbm.readthedocs.io/en/v3.3.5/index.html

&emsp;&emsp;LightGBM的GitHub地址：https://github.com/microsoft/LightGBM



### Ch.1 LightGBM基本原理与EFB降维方法

LGBM是如何做效率和精度“两手抓”的呢？简而言之就是LGBM充分借鉴了XGB提出的一系列提升精度的优化策略，同时在此基础之上进一步提出了一系列的数据压缩和决策树建模流程的优化策略。

其中，数据压缩方法能够让实际训练的数据量在大幅压缩的同时仍然保持较为完整的信息；而决策树建模流程方面的优化，则是在XGB提出的直方图优化算法基础上进行了大幅优化，不仅能够加速决策树建模速度，同时也能非常好的处理经过压缩后的数据，从而最终大幅提升每棵树的训练效率。并且有理论能够证明，哪怕LGBM实际建模是基于压缩后的数据进行训练，但其预测精度受到的影响也是微乎其微。

LGBM提出了非常多针对于实际计算过程的优化，例如Voting Parallel（投票特征并行）方法、特征多线程并行处理方法、GPU加速方法和分布式计算等，这些方法进一步提升了LGBM实际建模效率，并且一定程度拓宽了算法的使用场景。并且需要注意的是，所谓的计算效率优化，不仅体现在计算时间的大幅缩短，同时得益于LGBM所提出的一系列数据压缩技术，使得实际建模时数据内存占用也大幅减少。

LGBM算法可以看成是迭代过程几乎全盘借鉴XGB、而在数据压缩方法和决策树训练方法上有大量创新的算法

--第一部分我们重点介绍LGBM创新性提出的一系列方法，第二部分再来探讨LGBM损失函数求解流程。考虑到LGBM的推导流程和XGB几乎完全一样，原理部分的讲解的重点将会是LGBM创新性提出的一系列数据压缩和优化策略--

>  XGB几乎可以说是GBDT类算法的原理层面的里程碑，开创性的提出了拟合二阶泰勒展开的思路，并据此设计了全套关键数学表达式，包括包含Hessian值得伪残差、分裂增益计算公式化和叶节点权重计算公式。
>
> 而后继的LGBM和CatBoost，在损失函数求解过程几乎没有再提出超出XGB理论框架的内容，而是在数据预处理和决策树训练方法上提出了进一步优化方法。这点甚至也可以从LGBM原论文中看出，在LGBM原始论文中几乎没有任何关于损失函数求解的说明，**通篇几乎都在强调数据压缩方法和决策树优化流程的有效性**，我们也是通过查阅官方文档和源码才得知LGBM的具体迭代流程。因此，从这个角度来说，XGB是迄今为止GBDT类算法框架的理论最高峰。

### 数据压缩

LightGBM建模过程总共会进行三方面的数据压缩，根据实际建模顺序，会现在全样本上连续变量分箱（连续变量离散化），然后同时带入离散特征和离散后的连续变量进行离散特征捆绑（合并）降维，最终在每次构建一颗树之前进行样本下采样。

- 其中连续变量的分箱就是非常简单的等宽分箱，并且具体箱体的数量可以通过超参数进行人工调节；
- 而离散特征的降维，则是采用了一种所谓的互斥特征捆绑（Exclusive Feature Bundling, EFB）算法，该算法也是由LGBM首次提出，该方法的灵感来源于独热编码的逆向过程，通过把互斥的特征捆绑到一起来实现降维，这种方法能够很好的克服传统降维方法带来的信息大量损耗的问题，并且需要注意的是，输入EFB进行降维的特征，即包括原始离散特征，也包括第一阶段连续变量离散化之后的特征；在这一系列数据压缩之后，
- LGBM在每次迭代（也就是每次训练一颗决策树模型）的时候，还会围绕训练数据集进行下采样，此时的下采样不是简单的随机抽样，而是一种名为基于梯度的单边采样（Gradient-based One-Side Sampling, GOSS）的方法，和EFB类似，这种方法能够大幅压缩数据，但同时又不会导致信息的大量损失。

### 建模优化

- 直方图优化算法

  通过直方图的形式更加高效简洁的表示每次分裂前后数据节点的核心信息，并且父节点和子节点也可以通过直方图减法计算直接算得，从而加速数据集分裂的计算过程:

  <center><img src="https://ml2022.oss-cn-hangzhou.aliyuncs.com/img/202303221537628.png" alt="981861843f708f9efb5f74829c336b3" style="zoom: 40%;" />

- 其二则是leaf wise tree growth的叶子节点优先的决策树生长策略，这其实是一种树生长的模式，对于其他大多数决策树算法和集成算法来说，树都是一次生长一层，也就是所谓的Level-wise tree growth（深度优先的生长策略），生长过程如下。而LGBM则允许决策树生长过程优先按照节点进行分裂，即允许决策树“有偏”的生长，也就是所谓的leaf wise tree growth的叶子节点优先的决策树生长策略，具体生长过程如下：

<center><img src="https://ml2022.oss-cn-hangzhou.aliyuncs.com/img/202303221541288.png" alt="b4030247c4f8acaee3a8337bb6e7bb2" style="zoom:50%;" />

<center><img src="https://ml2022.oss-cn-hangzhou.aliyuncs.com/img/202303221543679.png" alt="33f0d4c90d591b2577698a730d24dfc" style="zoom:50%;" />

根据LGBM论文的论述，但从Level-wise tree growth远离层面，这种方法其实是有利有弊，其优势在于能够大幅提升每颗树的收敛速度，从总体来看相当于是提升了每次迭代效率；而问题则在于会每棵树的计算过程会变得更加复杂，并且存在一定的过拟合风险。不过对于LGBM来说，这些问题都能够被很好的克服，比如计算过程复杂的问题可以通过数据压缩来对冲，而过拟合风险则可以通过限制最大树深度来解决，因此总的来看Level-wise tree growth就是最适合LGBM的决策树生长策略。
### 连续变量分箱

lgbm用的是等宽分箱

xgb用的是分位数分箱
