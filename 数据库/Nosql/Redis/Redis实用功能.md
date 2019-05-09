1. redis-server
    - redis-server --test-memory 2048
    用来测试机器是否能给redis 2048m 的内存
2. redis-cli
    - redis-cli -r 100 -i 1 info | grep used_memory_human
        - -r 表示repeat 100次
        - -i 表示隔1秒
        - info | grep used_memory_human 输出redis内存占用率

    - redis-cli --raw和--no-raw

       --no-raw选项是要求命令的返回结果必须是原始的格式， --raw恰恰相
       186反， 返回格式化后的结果。
       在Redis中设置一个中文的value：
       ```bash
          redis-cli set hello "你好"
          OK
       ```
       如果正常执行get或者使用--no-raw选项， 那么返回的结果是二进制格
       式：
        ```bash
           redis-cli get hello
           "\xe4\xbd\xa0\xe5\xa5\xbd"
           redis-cli --no-raw get hello
           "\xe4\xbd\xa0\xe5\xa5\xbd"
        ```
       如果使用了--raw选项， 将会返回中文：
       ```bash
           redis-cli --raw get hello
           你好
       ```

3. redis-benchmark

   简明之意，这东西就是为了做性能基准测试用的

   - redis-benchmark -c 100 -n 20000

        代表100各个客户端同时请求Redis， 一共执行20000次。

       ```bash
        ====== PING_INLINE ======
          20000 requests completed in 0.44 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        0.00% <= 1 milliseconds
        54.90% <= 2 milliseconds
        92.01% <= 3 milliseconds
        99.55% <= 4 milliseconds
        99.99% <= 5 milliseconds
        100.00% <= 5 milliseconds
        45248.87 requests per second

        ====== PING_BULK ======
          20000 requests completed in 0.40 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        1.22% <= 1 milliseconds
        89.35% <= 2 milliseconds
        99.39% <= 3 milliseconds
        99.44% <= 4 milliseconds
        99.50% <= 5 milliseconds
        99.51% <= 6 milliseconds
        99.76% <= 7 milliseconds
        100.00% <= 7 milliseconds
        49875.31 requests per second

        ====== SET ======
          20000 requests completed in 0.48 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        0.00% <= 1 milliseconds
        44.29% <= 2 milliseconds
        88.36% <= 3 milliseconds
        96.86% <= 4 milliseconds
        98.39% <= 5 milliseconds
        98.96% <= 6 milliseconds
        99.04% <= 7 milliseconds
        99.12% <= 8 milliseconds
        99.15% <= 9 milliseconds
        99.50% <= 11 milliseconds
        99.51% <= 12 milliseconds
        99.79% <= 13 milliseconds
        100.00% <= 13 milliseconds
        41580.04 requests per second

        ====== GET ======
          20000 requests completed in 0.41 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        2.32% <= 1 milliseconds
        93.97% <= 2 milliseconds
        99.91% <= 3 milliseconds
        100.00% <= 3 milliseconds
        49019.61 requests per second

        ====== INCR ======
          20000 requests completed in 0.42 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        1.10% <= 1 milliseconds
        77.43% <= 2 milliseconds
        95.63% <= 3 milliseconds
        99.89% <= 4 milliseconds
        100.00% <= 5 milliseconds
        100.00% <= 5 milliseconds
        47846.89 requests per second

        ====== LPUSH ======
          20000 requests completed in 0.47 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        0.00% <= 1 milliseconds
        39.29% <= 2 milliseconds
        86.71% <= 3 milliseconds
        97.57% <= 4 milliseconds
        98.88% <= 5 milliseconds
        99.53% <= 6 milliseconds
        100.00% <= 6 milliseconds
        42283.30 requests per second

        ====== LPOP ======
          20000 requests completed in 0.44 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        0.32% <= 1 milliseconds
        61.51% <= 2 milliseconds
        93.03% <= 3 milliseconds
        98.91% <= 4 milliseconds
        99.38% <= 5 milliseconds
        99.73% <= 6 milliseconds
        99.76% <= 8 milliseconds
        100.00% <= 8 milliseconds
        45045.04 requests per second

        ====== SADD ======
          20000 requests completed in 0.43 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        0.03% <= 1 milliseconds
        63.65% <= 2 milliseconds
        93.00% <= 3 milliseconds
        99.83% <= 4 milliseconds
        100.00% <= 4 milliseconds
        46296.30 requests per second

        ====== SPOP ======
          20000 requests completed in 0.41 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        1.65% <= 1 milliseconds
        87.15% <= 2 milliseconds
        98.46% <= 3 milliseconds
        99.46% <= 4 milliseconds
        99.88% <= 5 milliseconds
        100.00% <= 5 milliseconds
        48543.69 requests per second

        ====== LPUSH (needed to benchmark LRANGE) ======
          20000 requests completed in 0.44 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        0.00% <= 1 milliseconds
        47.38% <= 2 milliseconds
        92.78% <= 3 milliseconds
        99.64% <= 4 milliseconds
        100.00% <= 4 milliseconds
        45351.47 requests per second

        ====== LRANGE_100 (first 100 elements) ======
          20000 requests completed in 1.09 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        0.00% <= 1 milliseconds
        0.14% <= 2 milliseconds
        80.49% <= 3 milliseconds
        97.26% <= 4 milliseconds
        99.40% <= 5 milliseconds
        99.65% <= 6 milliseconds
        99.75% <= 8 milliseconds
        99.86% <= 9 milliseconds
        99.88% <= 10 milliseconds
        100.00% <= 12 milliseconds
        18348.62 requests per second

        ====== LRANGE_300 (first 300 elements) ======
          20000 requests completed in 2.02 seconds
          100 parallel clients
          3 bytes payload
          keep alive: 1

        0.00% <= 1 milliseconds
        0.04% <= 2 milliseconds
        0.13% <= 3 milliseconds
        2.82% <= 4 milliseconds
        54.31% <= 5 milliseconds
        91.54% <= 6 milliseconds
        99.06% <= 7 milliseconds
        99.29% <= 8 milliseconds
        99.57% <= 9 milliseconds
        99.71% <= 10 milliseconds
        99.74% <= 11 milliseconds
        99.75% <= 12 milliseconds
        99.86% <= 13 milliseconds
        99.96% <= 14 milliseconds
        100.00% <= 17 milliseconds
        100.00% <= 17 milliseconds
        9891.20 requests per second

        ...
        ```
        比如===get===，表示一共执行了20000次get操作， 在0.41秒完成， 每个请求数据量
        是3个字节， 99.91%的命令执行时间小于1毫秒， Redis每秒可以处理
        49019.61次get请求，其他的类似。
    -
