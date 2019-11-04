怎么计算p(...)

- p(He is studying AI)=p(He)p(is)p(studying)p(AI) ——Uni-gram
- p(He is studying AI)=p(He)p(is | He)p(studying | is)p(AI | studying) ——Bi-gram
- p(He is studying AI)=p(He)p(is | He)p(studying | He is)p(AI | is studying)——Tri-gram

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

原始文本$\rightarrow$分词$\rightarrow$清洗$\rightarrow$标准化$\rightarrow$特征提取$\rightarrow$建模

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

- unigram model

    

- 