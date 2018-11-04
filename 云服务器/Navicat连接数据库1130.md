## 1.背景
Navicat连接云服务器的mysql报1130，内容是Host"xxx.xxx.xx.xx"is not allowed...
## 2.正文
- 解决方法（来源于网络[地址](https://www.cnblogs.com/devan/p/7055339.html)）
    > 可能是你的帐号不允许从远程登陆，只能在localhost。这个时候只要在localhost的那台电脑，登入mysql后，更改"mysql" 数据库里的 "user" 表里的 "host"项，从"localhost"改称"%"

总体运行语句:
```sql
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'mysqlpswd' WITH GRANT OPTION;
flush privileges;
```

#### 推一波自己的网站[Makeprogress.xyz](makeprogress.xyz)，里面的博文由4只猿共同更新，包含的东西比较广，包括大数据、Java后台、前端等，内容目前比较初级，欢迎访问和交流~