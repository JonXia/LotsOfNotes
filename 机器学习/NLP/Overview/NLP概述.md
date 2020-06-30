[TOC] 

语言模型该怎么计算p(...)

- p(He is studying AI)=p(He)p(is)p(studying)p(AI) ——Unigram
- p(He is studying AI)=p(He)p(is | He)p(studying | is)p(AI | studying) ——Bigram
- p(He is studying AI)=p(He)p(is | He)p(studying | He is)p(AI | is studying)——Trigram

深度学习不局限于联合概率的计算，能考虑更多的维度，这种叫Markov假设

## 场景

- 机器翻译

- 文本摘要

  - 自动摘要

- 聊天机器人（小项目）

  *知识图谱的问答系统

  - 闲聊型 （seq2seq）
  - 任务导向型 （意图识别）

- 信息抽取（最重要的领域）（有项目）

  涉及

  - 问题转三元组
  - NER（命名实体识别）KG
  - 事件检测
  - 知识图谱

## 关键技术

自然语言处理四个维度

- Semantic（语义）

  情感分析等

- Syntax（句法）

  ie.(句法分析，依存分析)

- Morphology（单词）

  ie.(分词，POS（词性标注），NER)

  以上属于单词层面的技术，从下到上由点及面：

  Word Segmentation分词，Part of Speech词性，Named Entity Recognition命名实体识别，

  Parsing句法分析:CYK算法，DependencyParsing依存分析，RelationExaction关系抽取

  主流公开的NLP问题：[What are the major open problems in natural language processing.](https://www.quora.com/What-are-the-major-open-problems-in-natural-language-understanding)

- Phonetics（声音）

## 文本处理的流程

**原始文本$\rightarrow$分词$\rightarrow$清洗$\rightarrow$标准化$\rightarrow$特征提取$\rightarrow$建模**

- 清洗：包括无用的标签、特殊符号、数字、停用词(对理解产生不了影响的词)
- 标准化：英文用的多，分为Stemming、Lemmazation
- 特征提取：tf-idf、word2ec
- 建模：。。。

### 分词

分词算法：

- 前向最大匹配

    1. 根据词典库指定初步最大长度

    2. 在一个句子里，从头开始找到max_length个字的段落

    3. for in range(max_length)

        ​	查词典库看是否匹配，匹配到了就不再细分了

        ​	max_length-=1

- 后向最大匹配

    步骤与前向最大匹配相反

缺点：x

- 贪心算法只能找到局部最优

- 效率低，依赖max_length
- 不能细分得到更好的结果

- 歧义，不能考虑语义

#### 考虑语言模型

分词后结果应用语言模型看说的是不是人话的概率，挑最好结果

- [unigram model]( https://blog.csdn.net/pipisorry/article/details/42560693 )

    1. 生成所有可能的分割$s_1,s_2,...$；

    2. 通过LanguageModel（这里讲UnigramModel）找到最好的分词结果：

        找到分词后单词在词库里的概率$P(s_1),P(s_2),...$，之后句子的计算概率：

        $P(s_1)\times P(s_{...})$这种概率相乘的方式会导致Underflow现象，所以把这个式子取对数，转变成加和的形式，然后再把它求平均值，解决词很多概率相乘结果会变小的问题。

        $logP(s)=\frac {log\sum_{i=1}^{m} P(s_i)} m$



- Viterb

    将上述步骤合并，减少时间复杂度，回头整理

#### 拼写纠错 Spell Correction

算法:

- 编辑距离

    通常有三个操作，addcc,delete,replace，看候选单词三个操作的操作成本，成本越小词越相似。通过词频、上下文关联等标准过滤返回可能的纠错单词。（DP算法）

    上述方法**时间复杂度高:O(n)**

    可以更改为：用户输入单词$\rightarrow$生成编辑距离为1,2的字符串$\rightarrow$过滤$\rightarrow$返回

- 过滤

    概念：给定一个字符串s，我们要找出最有可能成为正确的字符串c，也就是

    $
    \hat c = argmac_{c\in candidates}p(c|s)
   $
   $
   \quad = argmac_{c\in candidates}\frac {p(s|c) \times p(c)}  {p(s)}
   $
   因为$p(s)$可以看作为常数，有：
   $
   \quad = argmac_{c\in candidates} p(s|c) \times p(c)
   $

   $p(s|c)$:给定正确的字符串,有多大概率写成了s形式?可以通过数据库统计.

   $p(c)$:给定正确的字符串,在词库里出现的概率多高?

#### 停用词

将停用词,出现频率较低的词过滤掉.但也要考虑应用场景,比如情感分析,有些词就不必过滤.

#### 标准化

通常用在英文场景中。

- stemming

- lemonazation

#### 单词/句子的表示

one-hot encoding:

首先要有词典，类似于这样:

```python
['an', 'apple', 'many']
```

比如 this's a apple.有，这个形式也可以看作**向量**:

```python
(0,1,0)
```

独热编码在文本处理上属于：Bag of words 词袋模型。

#### 文本相似度

- 欧氏距离：$d(\boldsymbol s_1,\boldsymbol s_2)=|\boldsymbol s_1-\boldsymbol s_2|$

    缺点：没有考虑方向

- 余弦相似度：$d（\boldsymbol s_1,\boldsymbol s_2）=\boldsymbol  s_1 · \boldsymbol  s_2/(|\boldsymbol  s_1|\times|\boldsymbol  s_2|)$，$（内积/范数）$

    但是以上根据词频构建的向量，并不能表示出句子中表达重要含义的单词。

- [tf-idf]( http://www.360baidu.cn/seo/tf-idf.html )：$tfidf(w)=tf(d,w)\times idf(w)$

    $tf（d,w）$:代表文档$d$中词$w$的词频；

    $idf(w)=log\frac N {N(w+1)}$：$N$：文档总数，$N(w)$：词w出现在多少个文档

#### 词向量

通过上述方法，我们可以发现，

- 利用字典+词频做One-hot的方法是属于一种词袋模型，假设词与词之间相互独立，也不考虑词语词之间的顺序，这是不符合文本常识的；

- 而且随则词典增多，向量大小会无限扩展，其中很多0，单词的向量会离散、稀疏。

##### 分布式的表示方法介绍：

word2vec

特点：向量大小自定义，稀疏性弱；

理论上分布式表达方法可以表达$+\infty$个单词；

##### 训练词向量：

1. 指定维度：

2. 输入:字符串，包括1Billon单词这种规模

3. 训练：skip-gram、glove、cbow、rnn/lstm、matrix factiorization...

4. 判断相关性：通过上述、欧氏距离、余弦相似度、lstm/rnn等

#### 倒排表

搜索用

### 语言模型

#### Noisy Channel Model

$p(text|source)=p(source|text)p(text)$

理解：$p(source|test)$：翻译，可以理解为词典；$p(test)$：语言模型

#### 语言模型

语言模型：判断语法上是否通顺，计算句子符合语言模型的概率。

##### ChainRule for Language Model

联合概率表达为：$p(A,B)=p(A|B)p(B)=p(B|A)p(A)$ 

$\therefore$要表达句子的联合概率有：

$p(今天,放假,我们,都,休息)=p(今天)p(放假|今天)p(我们|今天放假)p(都|今天放假我们)p(休息|今天放假我们都)$

##### 缺点：

参数空间过大，概率值会很稀疏，依赖语料库，结果没有语言意义

##### Markov Assumption

$1st\quad order:p(w_1,w_2,w_3,...,w_n)=p(w_1)\prod _{i=2} ^n (w_i|w_{i-1})$ 

$2st\quad order:p(w_1,w_2,w_3,...,w_n)=p(w_1|w_2)\prod _{i=3} ^n (w_i|w_{i-2},w_{i-1})$

$...$

##### [N-gram]( https://blog.csdn.net/han_xiaoyang/article/details/50646667#commentBox )

$Unigram=   p(w_1,w_2,w_3,...,w_n)=p(w_1)p(w_2)p(w_3)p(w_...)p(w_n)$

$Bigram= p(w_1,w_2,w_3,...,w_n)=1st Order$

$N>2$时叫，HigherOrderLM

$Trigram= p(w_1,w_2,w_3,...,w_n)=2stOrder$

##### Estimating 

###### Estimating Probability of LM

通过语言模型评估句子，需要平滑项

###### Evaluation of LM

评估模型：perplexity；而且在不同模型里有不同的评估方法，但是通过这种方法，对于再语料库没有出现的词，对句子会有不正确的判断，所以需要平滑项。

##### Smoothing

- add-one smoothing(laplace smoothing)

    $P_{MLE}(w_i|w_{i-1})=\frac {c(w_{i-1},w_i)} {c(w_i)}$，极大似然估计，通过词频计算概率

    $P_{Add1}(w_i|w_{i-1})=\frac {c(w_{i-1},w_i)+1} {c(w_i)+V}$，$V$：词典的大小，加V确保了$P(w_i|w_{i-1})$总概率=1

- add-k smoothing

    $P_{Addk}(w_i|w_{i-1})=\frac {c(w_{i-1},w_i)+k} {c(w_i)+kV}$，$k$：未知数，所以目标变成了设定目标函数优化$k$，使得$k$最小。

- interpolation smoothing

    为了比较公平的比较，需要考虑，N-gram出现的频次。

    $p(w_n|w_{n-1},w_{n-2})=\lambda_1 p(w_n|w_{n-1},w_{n-2})+\lambda_2p(w_n|w_{n-1})+\lambda_3p(w_n)$

    $\lambda_1+\lambda_2+\lambda_3=1$ 。

- goot-turning smoothing

    - 没有出现过的单词

        $P_{MLE}=0$

        $P_{GT}=\frac {N_1} N$，其中$N$代表单词总数，$N_1$代表出现一次的单词。

    - 出现过的单词

        $P_{MLE}=\frac c N$

        $P_{GT}=\frac {(c+1)N_{c+1}} {N_c\times N}$，其中，$c$代表出现的次数，$N_{c+1}$代表出现c+1次的单词。

    这样做解决了未出现在词库的单词带来的最终概率等于0的问题，但是严重依赖下一种($N_{i++1}$)概率。

#### Generating Sentence by LM

可以采用Bigram的方法，从一个词出发考虑能与其相连的概率最大的下个词-》一个句子 

# 两种Learning的方法

## 专家系统

符号主义，涉及到离散数学知识；

**专家系统=推理引擎+知识**

特点：

- 处理不确定性

- 知识的表示

    知识图谱等表示形式

- 可解释性

- 可以做知识推理，通过已有规则生成新规则

缺点：

- 规则数量庞大
- 需要大量的domain knowledge，和专家指导
- 迁移学习的能力差
- 学习能力差
- 人的思维是有限的

## 基于概率的系统

连接主义

机器学习的分类：监督、无监督

模型的分类：生成模型、判别模型

生成模型：记住特点来生成

判别模型：记住区别来判断

| Supervized Learning |  Unsupervized Learning  
---|---|---
 Generative Model     | NaiveBayes | HMM,LDA,GMM 
Discriminative Model | LR、CRF  |                       

### Naive Bayes

先验概率：总样本中，正样本和负样本的占比。

然后了解所有字样本在各类别中的占比，来考虑新数据的分类。

条件独立：

$p(x,y|z)=p(x|z)\times p(y|z)$

### 评估标准

- 准确率和召回率

    准确率：$p(precision)=\frac {TP} {TP+FP}$

    召回率：$p(recall)=\frac {TP} {TP+FN}$

- F1 

    p和r不直观，所以提出了f1等评估标准3

    $f1-measure=\frac {2\times precision \times recall} {precision+recall}$

## LR

略

**TODO：证明LR是线性分类器？**

目标函数：模型的实例化=定义模型的目标函数+优化算法(GD,SGD,Adagrad,Adam)

简化\hat y表达式，取-log求argmin的w,b，转化成优化算法，这里用Gradient Decent。

- GD

    回头整理

当数据线性可分的时候，需要正则化项，对特征参数$w$做出惩罚。

模型复杂度：简单环境用简单模型。复杂用复杂。减少过拟合现象，可以从四个角度考虑：

- 模型选择
- 参数个数
- 参数空间选择？
- 模型拟合过少的样本

[正则化和MLE/MAP]( https://www.jianshu.com/p/4bad38fe07e6 )：

- L0

- L1:$\lambda||w||_1$，特点会使参数变得sparse，使得部分特征去掉，适用于，某些特征不可用的场景。

- L2:$+\lambda||w||_2^2$，$\lambda$步长，超参数,$w$模型的参数。1

- Nuclear Norm

    想让模型准确率提高——l2，l1可以实现特征参数的稀疏性

    l1和l2可以一起使用=>ElasticNet

参数搜索策略：

- 随即搜索：
- 遗传算法：
- 贝叶斯优化：

MLE/MAP：

样本量多的时候，map->mlez

### Lasso 

加入了L1，变成了Lasso/岭回归。有了稀疏性，所以拥有了特征选择的效果。

为什么更期望稀疏

- 特征维度太高，计算量也变得高
- 在稀疏性条件下，计算量只依赖非0项的个数
- 提高可解释性

特征选择技术：

- 对特征进行Exhaustive search，从多种特征组合，时间复杂度高。

- Greedy Approaches

    - Forward
    - Backward

- Lasso，正则的方法选择特征

    加入L1,L2正则项

    - Coordinate Descent

        