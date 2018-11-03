## 1.正文
- 停止mysql服务
    > service mysqld stop

- 进入mysql安装目录bin/ 使用safe模式进入mysql
    > mysqld_safe --skip-grant-tables
- 另开一个session并修改密码
    > mysql -u root
      use mysql
      update user set password=PASSWORD("你的密码") where User = 'root';
- kill掉safe模式并启动服务
    > ps -ef|grep mysql
    > service mysqld start
- ok
#### 推一波自己的网站[Makeprogress.xyz](makeprogress.xyz)，里面的博文由4只猿共同更新，包含的东西比较广，包括大数据、Java后台、前端等，内容目前比较初级，欢迎来访和交流~