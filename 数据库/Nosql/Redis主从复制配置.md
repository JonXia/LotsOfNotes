## Redis主从复制

1. 环境 centos7,redis 3.2.10 本地测试
2. .conf配置，一主三从
    - server-6379.conf
        ```bash
        bind 192.168.5.128
        port 6379
        daemonize yes
        logfile "6379.log"
        dbfilename "db-6379.rdb"
        ```
    - server-6380.conf
        ```bash
        bind 192.168.5.129
        port 6380
        daemonize yes
        logfile "6380.log"
        dbfilename "db-6380.rdb"
        slaveof 127.0.0.1 6379
        ```
        可以看到只需要一行配置就ok
        > slaveof 127.0.0.1 6379

        其他的类似
    - server-6381.conf
        ```bash
        bind 192.168.5.130
        port 6381
        daemonize yes
        logfile "6381.log"
        dbfilename "db-6381.rdb"
        slaveof 127.0.0.1 6379
        ```
    - server-6382.conf
        ```bash
        bind 192.168.5.131
        port 6382
        daemonize yes
        logfile "6382.log"
        dbfilename "db-6382.rdb"
        slaveof 127.0.0.1 6379
        ```
3. 启动
    server正常启动就好
    ```bash
    ./src/redis-server server-6379.conf
    ./src/redis-server server-6380.conf
    ./src/redis-server server-6381.conf
    ./src/redis-server server-6382.conf
    ```
    启动，在master写一个数据，在slave节点就可以查到

4. 原理

   TODO 这个以后有时间再写，现在没研究到Redis这里
    推荐Redis开发与运维这本书，受益匪浅
**ps:推一下以后更新的篇目，SpringMVC源码篇，近期更新[连接]()**
