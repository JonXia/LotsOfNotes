## 1.环境
服务器Centos7
## 2.安装和配置
使用yum安装mysql必须安装mariadb，安装过后不习惯，所以选择手动下载rpm安装。
```bash
# wget http://dev.mysql.com/get/mysql-community-release-el7-5.noarch.rpm
# rpm -ivh mysql-community-release-el7-5.noarch.rpm
# yum install mysql-community-server
# service mysqld restart
```
安装后没有设置密码，需要进入命令行界面手动设置一下
```bash
mysql> set password for 'root'@'localhost' =password('password');
Query OK, 0 rows affected (0.00 sec)
```
然后配置远程登陆的权限
```bash
mysql> grant all privileges on *.* to root@'%'identified by 'password';
```
设置配置文件
```bash
vim /etc/my.cnf
[mysql]
default-character-set =utf8
```
## 3.卸载
```bash
yum remove mysql
...
# rpm -qa|grep -i mysql
mysql-community-libs-5.6.34-2.el7.x86_64
mysql-community-release-el7-5.noarch
mysql-community-common-5.6.34-2.el7.x86_64

# yum remove mysql-community-libs.noarch
...
# rpm -qa|grep -i mysql
依此类推
```

#### 推一波自己的网站[Makeprogress.xyz](makeprogress.xyz)，里面的博文由4只猿共同更新，包含的东西比较广，包括大数据、Java后台、前端等，内容目前比较初级，欢迎来访和交流~