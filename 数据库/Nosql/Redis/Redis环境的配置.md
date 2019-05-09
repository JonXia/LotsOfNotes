1. 环境：centos7
2. 安装：
    ```bash
    wget http://download.redis.io/releases/redis-3.2.10.tar.gz
    tar -xzvf redis-3.2.10.tar.gz

    注意，需要gcc依赖
    yum install gcc

    cd redis-3.2.10
    接下来编译
    make clean
    make
    make install或者直接
    make MALLOC=libc
    cd src && make install 都可以

    提示make test，测试没问题
    ```
3. 启动

   提示：redis的可执行文件在src目录下，包括redis-server,redis-cli,redis-benchmark等工具

   直接./redis-server，直接以前台模式启动redis，后台启动要改配置文件
   redis.conf中的daemonize no为yes
   ```bash
    # By default Redis does not run as a daemon. Use 'yes' if you need it.
    # Note that Redis will write a pid file in /var/run/redis.pid when daemonized.
    daemonize yes
   ```
   启动时加上配置文件，
   ```bash
   ./src/redis-server server-6379.conf
   ./src/redis-cli -h 127.0.0.1 -p 6379
   ```
