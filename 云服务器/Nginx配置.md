## 1.背景
需要实现80端口到项目端口的转发
## 2.正文
- yum安装
    ```bash
    yum install nginx
    ```
    查看依赖
    ```bash
    yum deplist nginx^
    ```
    ```bash
    [root@VM_0_11_centos ~]# yum deplist nginx
    Loaded plugins: fastestmirror, langpacks
    Loading mirror speeds from cached hostfile
    package: nginx.x86_64 1:1.12.2-2.el7
      dependency: /bin/sh
       provider: bash.x86_64 4.2.46-30.el7
      dependency: libc.so.6(GLIBC_2.14)(64bit)
       provider: glibc.x86_64 2.17-222.el7
      dependency: libcrypt.so.1()(64bit)
       provider: glibc.x86_64 2.17-222.el7
      dependency: libcrypt.so.1(GLIBC_2.2.5)(64bit)
       provider: glibc.x86_64 2.17-222.el7
      dependency: libcrypto.so.10()(64bit)
       provider: openssl-libs.x86_64 1:1.0.2k-12.el7
      dependency: libcrypto.so.10(OPENSSL_1.0.2)(64bit)
       provider: openssl-libs.x86_64 1:1.0.2k-12.el7
      dependency: libcrypto.so.10(libcrypto.so.10)(64bit)
       provider: openssl-libs.x86_64 1:1.0.2k-12.el7
      dependency: libdl.so.2()(64bit)
       provider: glibc.x86_64 2.17-222.el7
      dependency: libdl.so.2(GLIBC_2.2.5)(64bit)
       provider: glibc.x86_64 2.17-222.el7
      dependency: libpcre.so.1()(64bit)
       provider: pcre.x86_64 8.32-17.el7
      dependency: libprofiler.so.0()(64bit)
       provider: gperftools-libs.x86_64 2.6.1-1.el7
      dependency: libpthread.so.0()(64bit)
       provider: glibc.x86_64 2.17-222.el7
      dependency: libpthread.so.0(GLIBC_2.2.5)(64bit)
       provider: glibc.x86_64 2.17-222.el7
      dependency: libssl.so.10()(64bit)
       provider: openssl-libs.x86_64 1:1.0.2k-12.el7
      dependency: libssl.so.10(libssl.so.10)(64bit)
       provider: openssl-libs.x86_64 1:1.0.2k-12.el7
      dependency: libz.so.1()(64bit)
       provider: zlib.x86_64 1.2.7-17.el7
      dependency: nginx-all-modules = 1:1.12.2-2.el7
       provider: nginx-all-modules.noarch 1:1.12.2-2.el7
      dependency: nginx-filesystem
       provider: nginx-filesystem.noarch 1:1.12.2-2.el7
      dependency: nginx-filesystem = 1:1.12.2-2.el7
       provider: nginx-filesystem.noarch 1:1.12.2-2.el7
      dependency: openssl
       provider: openssl.x86_64 1:1.0.2k-12.el7
      dependency: pcre
       provider: pcre.x86_64 8.32-17.el7
       provider: pcre.i686 8.32-17.el7
      dependency: rtld(GNU_HASH)
       provider: glibc.x86_64 2.17-222.el7
       provider: glibc.i686 2.17-222.el7
      dependency: systemd
       provider: systemd.x86_64 219-57.el7_5.3

    ```
    可以看到yum安装已经把zlib、pcre、openssl、glibc等安装好了
- vim /etc/nginx/nginx.conf修改配置文件
- nginx -t 验证配置文件
- service nginx start/systemctl start nginx 启动
-
- 配置文件备份
```conf

# For more information on configuration, see:
#   * Official English Documentation: http://nginx.org/en/docs/
#   * Official Russian Documentation: http://nginx.org/ru/docs/

user nginx;
worker_processes auto;
error_log /var/log/nginx/error.log;
pid /run/nginx.pid;

# Load dynamic modules. See /usr/share/nginx/README.dynamic.
include /usr/share/nginx/modules/*.conf;

events {
    worker_connections 1024;
}

http {
    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for"';

    access_log  /var/log/nginx/access.log  main;

    sendfile            on;
    tcp_nopush          on;
    tcp_nodelay         on;
    keepalive_timeout   65;
    types_hash_max_size 2048;

    include             /etc/nginx/mime.types;
    default_type        application/octet-stream;

    # Load modular configuration files from the /etc/nginx/conf.d directory.
    # See http://nginx.org/en/docs/ngx_core_module.html#include
    # for more information.
    include /etc/nginx/conf.d/*.conf;
        #MyConf-start
    server {
    listen 80;
    server_name makeprogress.xyz;
    #access_log /data/wwwlogs/api.51carry.com_nginx.log combined;

    location / {
        proxy_pass http://127.0.0.1:8083;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
        proxy_set_header X-Forwarded-Port $server_port;
        }
    }
    #MyConf-end


# Settings for a TLS enabled server.
#
#    server {
#        listen       443 ssl http2 default_server;
#        listen       [::]:443 ssl http2 default_server;
#        server_name  _;
#        root         /usr/share/nginx/html;
#
#        ssl_certificate "/etc/pki/nginx/server.crt";
#        ssl_certificate_key "/etc/pki/nginx/private/server.key";
#        ssl_session_cache shared:SSL:1m;
#        ssl_session_timeout  10m;
#        ssl_ciphers HIGH:!aNULL:!MD5;
#        ssl_prefer_server_ciphers on;
#
#        # Load configuration files for the default server block.
#        include /etc/nginx/default.d/*.conf;
#
#        location / {
#        }
#
#        error_page 404 /404.html;
#            location = /40x.html {
#        }
#
#        error_page 500 502 503 504 /50x.html;
#            location = /50x.html {
#        }
#    }

}
```