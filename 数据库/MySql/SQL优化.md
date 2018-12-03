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

    - distinct [为了防止视图合并..](https://blog.csdn.net/ren6370/article/details/73732537)

    - force index(index_name) 可以强制使用索引

    - where ... group by ... having ...这里的条件过滤一定要注意，假如条件过滤设置的比较复杂，那么在同等级的查询条件一定要把能过滤掉比较多的条件放在where后面，group by having再进行进一步过滤

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
    - InnoDB参数
        - innodb_buffer_size:
            数据块和索引块的缓存大小
        - innodb_io_capacity：
            这个参数刷新脏页数量和合并插入数量，改善磁盘IO处理能力

        - max_connections

        - back_log
            MySQL能暂存的连接数量(根据实际设置)这两个负责在连接高的情况下调优，

        - key_buffer_size=1024M(myisam)
            key_buffer_size指定索引缓冲区的大小，它决定索引处理的速度，尤其是索引读的速度。通过检查状态值 Key_read_requests和Key_reads，可以知道key_buffer_size设置是否合理。比例key_reads / key_read_requests应该尽可能的低，至少是1:100，1:1000更好（上述状态值可以使用SHOW STATUS LIKE ‘key_read%’获得）。

        - query_cache_size=128M
            内部内存临时表的最大值,每个线程都要分配
        - join_buffer_size=1024M
            联合查询操作所能使用的缓冲区大小
        - record_buffer_size
            每个进行一个顺序扫描的线程为其扫描的每张表分配这个大小的一个缓冲区。如果你做很多顺序扫描，你可能想要增加该值。
            默认数值是131072(128K)，可改为16773120 (16M)
2. 工具
    - profilling
        [profilling看这篇](https://blog.csdn.net/u011277123/article/details/53316206)
        [和这篇](https://www.cnblogs.com/timlearn/p/4052523.html)

        [输出SQL索引优化建议工具SQLAdvisor](https://blog.csdn.net/qq_14997169/article/details/76054081)

3. 文章
    - [SQL优化](http://lib.csdn.net/article/mysql/5037)

    - [innodb 数据压缩](https://blog.csdn.net/hello188988/article/details/53213118)
