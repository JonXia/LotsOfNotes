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
- 特征提取：tf-idf、word2ve
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

缺点：

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

- 余弦相似度：$d（\boldsymbol s_1,\boldsymbol s_2）=\boldsymbol  s_1 · \boldsymbol  s_2/(|\boldsymbol  s_1|\times|\boldsymbol  s_2|)$，$内积/范数$

    但是以上根据词频构建的向量，并不能表示出句子中表达重要含义的单词。

- tf-idf：$tfidf(w)=tf(d,w)\times idf(w)$

    $tf（d,w）$:代表文档$d$中词$w$的词频；

    $idf(w)=log\frac N {N(w+1)}$：$N$：文档总数，$N(w)$：词w出现在多少个文档

#### 词向量

通过上述方法，我们可以发现，

- 利用字典+词频做One-hot的方法是属于一种词袋模型，假设词与词之间相互独立，也不考虑词语词之间的顺序，这是不符合文本常识的；

- 而且随则词典增多，向量大小会无限扩展，其中很多0，单词的向量会离散、稀疏。

##### 分布式的表示方法介绍：

word2vec！

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

语言模型：判断语法上是否通顺。

