1. 正文

    - 导致全表扫描的
        - where子句中用null判断，所有null值都不可以作为索引使用，自然用null做条件也就不会使用索引。
        - !=或<>，会导致全表扫描
        - 应尽量避免在 where 子句中使用 or 来连接条件，如果一个字段有索引，一个字段没有索引，将导致引擎放弃使用索引而进行全表扫描，如：
            ```sql
            select id from t where num=10 or Name = 'admin'
            ```
            可以这样查询：
            ```sql
            select id from t where num = 10
            union all
            select id from t where Name = 'admin'
            ```
        - in 和 not in 也要慎用，最好用在建立索引的字段，否则会导致全表扫描，很多时候用 exists 代替 in 是一个好的选择：
            ```sql
            select num from a where num in(select num from b)
            ```
            用下面的语句替换：
            ```sql
            select num from a where exists(select 1 from b where num=a.num)
            ```
        - like 后加通配符不仅不会使用索引

        - ```sql
          select count(*) from tableName
          ```
            这样不带任何条件的count会引起全表扫描

        -


    - 大表相关
        - 对于多张大数据量的表JOIN，要先分页再JOIN，否则逻辑读会很高，性能很差。
        - 大数据下如何做分页：
            MySQL并不是跳过offset行，而是取offset+N行，然后放弃前offset行，返回N行，所以当offset大的时候，效率自然低下；这样要么控制返回的总页数，要么对超过特定阈值的页数进行SQL改写：
            ```sql
            select a.* from 表1 a,
            (select id from 表1 where 条件 limit 100000,20) b
            where a.id=b.id;
            #定位id段，再进行连接查询
            ```

2. 工具
    - profilling
        ![profilling看这篇](https://blog.csdn.net/u011277123/article/details/53316206)
        ![和这篇](https://www.cnblogs.com/timlearn/p/4052523.html)
        ![输出SQL索引优化建议工具SQLAdvisor](https://blog.csdn.net/qq_14997169/article/details/76054081)