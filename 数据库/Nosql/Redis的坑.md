
## Redis的坑
Redis，虽然很快很好用，但是开发过程中的bug还是不少...好在官方和社区支持都不错，百度不到，google也能解决的差不多，
这里把遇到的异常记录一下，大部分遇到的坑都有记录，推荐Redis运维与开发和Redis设计与实现，收到的启发不少。

1. Redis超时异常
```java
redis.clients.jedis.exceptions.JedisConnectionException: Unknown reply: 5
```
过期时间3600*24 *30  TimeUnit.SECONDS；
修改为TimeUnit.DAYS 30解决
[](https://blog.csdn.net/Rylan11/article/details/51911126?utm_source=itdadao&utm_medium=referral)
[](https://www.cnblogs.com/pier2/p/spring-data-redis-bug.html)

2. 内存不够用报错，但是Redis服务器还有生于内存

- 两个解决方法(overcommit_memory)

    - echo "vm.overcommit_memory=1" > /etc/sysctl.conf  或 vi /etcsysctl.conf , 然后reboot重启机器
    - echo 1 > /proc/sys/vm/overcommit_memory  不需要启机器就生效
overcommit_memory参数说明：
设置内存分配策略（可选，根据服务器的实际情况进行设置）
/proc/sys/vm/overcommit_memory
可选值：0、1、2。
0， 表示内核将检查是否有足够的可用内存供应用进程使用；如果有足够的可用内存，内存申请允许；否则，内存申请失败，并把错误返回给应用进程。
1， 表示内核允许分配所有的物理内存，而不管当前的内存状态如何。
2， 表示内核允许分配超过所有物理内存和交换空间总和的内存

    注意：redis在dump数据的时候，会fork出一个子进程，理论上child进程所占用的内存和parent是一样的，比如parent占用 的内存为8G，这个时候也要同样分配8G的内存给child,如果内存无法负担，往往会造成redis服务器的down机或者IO负载过高，效率下降。所 以这里比较优化的内存分配策略应该设置为 1（表示内核允许分配所有的物理内存，而不管当前的内存状态如何）。

    这里又涉及到Overcommit和OOM。

    > 什么是Overcommit和OOM
  在Unix中，当一个用户进程使用malloc()函数申请内存时，假如返回值是NULL，则这个进程知道当前没有可用内存空间，就会做相应的处理工作。许多进程会打印错误信息并退出。
  Linux使用另外一种处理方式，它对大部分申请内存的请求都回复"yes"，以便能跑更多更大的程序。因为申请内存后，并不会马上使用内存。这种技术叫做Overcommit。
  当内存不足时，会发生OOM killer(OOM=out-of-memory)。它会选择杀死一些进程(用户态进程，不是内核线程)，以便释放内存。

  > Overcommit的策略
    Linux下overcommit有三种策略(Documentation/vm/overcommit-accounting)：
    0. 启发式策略。合理的overcommit会被接受，不合理的overcommit会被拒绝。
    1. 任何overcommit都会被接受。
    2. 当系统分配的内存超过swap+N%*物理RAM(N%由vm.overcommit_ratio决定)时，会拒绝commit。
    overcommit的策略通过vm.overcommit_memory设置。
    overcommit的百分比由vm.overcommit_ratio设置。

        # echo 2 > /proc/sys/vm/overcommit_memory

        # echo 80 > /proc/sys/vm/overcommit_ratio

        当oom-killer发生时，linux会选择杀死哪些进程
  选择进程的函数是oom_badness函数(在mm/oom_kill.c中)，该函数会计算每个进程的点数(0~1000)。
  点数越高，这个进程越有可能被杀死。
  每个进程的点数跟oom_score_adj有关，而且oom_score_adj可以被设置(-1000最低，1000最高)。

3. redis中的incr实行自增

   incr是从0开始，如果你需要从1开始，需要多加一次。
   [用RedisAtomicLong处理访问量等实时性要求高的需求](https://blog.csdn.net/qq_35264464/article/details/79490664)

4. Redis的sentinel不能布在marster上...这个一定要小心
    另起一台虚拟机就好，我用一主二从三哨兵，一台marster布3哨兵就行

5. 使用pipline提升效率

    这个博客讲的不错[链接](https://blog.csdn.net/babylove_BaLe/article/details/76258904)
    ```java
    Jedis jedis = new Jedis("127.0.0.1");
    Pipeline pipeline = jedis.pipelined();
    pipeline.set("hello", "world");
    pipeline.incr("counter");
    List<Object> resultList = pipeline.syncAndReturnAll();
    for (Object object : resultList) {
        System.out.println(object);
    }
    ```

6.  但是192.168.5.127中不能用scan操作

    而且：对于 SCAN 这类增量式迭代命令来说， 因为在对键进行增量式迭代的过程中， 键可能会被修改， 所以增量式迭代命令只能对被返回的元素提供有限的保证 （offer limited guarantees about the returned elements）。

    所以对于key的设计与存储使用方面要注意，不能经常修改.