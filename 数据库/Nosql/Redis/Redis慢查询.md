1. 正文

    通常Redis执行命令速度非常快， 但也存在例外， 如对一个包含上万个
    元素的hash结构执行hgetall操作， 由于数据量比较大且命令算法复杂度是
    O（n） ， 这条命令执行速度必然很慢。 这个问题就是典型的不合理使用API
    和数据结构。 对于高并发的场景我们应该尽量避免在大对象上执行算法复杂
    度超过O（n） 的命令。

    Redis原生提供慢查询统计功能， 执行slowlog get{n}命令可以获取最近
    的n条慢查询命令， 默认对于执行超过10毫秒的命令都会记录到一个定长队
    列中， 线上实例建议设置为1毫秒便于及时发现毫秒级以上的命令。

    发现慢查询后，可以按照以下两个方向去调整：

    - 修改为低算法度的命令， 如hgetall改为hmget等， 禁用keys、 sort等命
    令。
    - 调整大对象：缩减大对象数据或把大对象拆分为多个小对象， 防止一次命令操作过多的数据。 大对象拆分过程需要视具体的业务决定， 如用户
    好友集合存储在Redis中， 有些热点用户会关注大量好友， 这时可以按时间
    或其他维度拆分到多个集合中。

        - 如何发现大对象：使用 **redis-cli --bigkeys**命令
        ```bash
        redis-cli --bigkeys

        # Scanning the entire keyspace to find biggest keys as well as
        # average sizes per key type.  You can use -i 0.1 to sleep 0.1 sec
        # per 100 SCAN commands (not usually needed).

        [00.00%] Biggest string found so far 'key:__rand_int__' with 3 bytes
        [00.00%] Biggest hash   found so far 'website' with 2 fields
        [00.00%] Biggest string found so far 'counter:__rand_int__' with 5 bytes

        -------- summary -------

        Sampled 5 keys in the keyspace!
        Total key length in bytes is 51 (avg len 10.20)

        Biggest string found 'counter:__rand_int__' has 5 bytes
        Biggest   hash found 'website' has 2 fields

        4 strings with 12 bytes (80.00% of keys, avg size 3.00)
        0 lists with 0 items (00.00% of keys, avg size 0.00)
        0 sets with 0 members (00.00% of keys, avg size 0.00)
        1 hashs with 2 fields (20.00% of keys, avg size 2.00)
        0 zsets with 0 members (00.00% of keys, avg size 0.00)
        ```

        可以看到，此命令清晰的把key对应的内容大小全都列出来了

