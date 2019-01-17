如何使用Linux的Crontab定时执行PHP脚本的方法
首先说说cron,它是一个linux下的定时执行工具。根用户以外的用户可以使用 crontab 工具来配置 cron 任务。所有用户定义的 crontab 都被保存在/var/spool/cron 目录中，并使用创建它们的用户身份来执行。要以某用户身份创建一个 crontab 项目，登录为该用户，然后键入 crontab -e 命令来编辑该用户的 crontab。该文件使用的格式和 /etc/crontab 相同。当对 crontab 所做的改变被保存后，该 crontab 文件就会根据该用户名被保存，并写入文件 /var/spool/cron/username 中。cron 守护进程每分钟都检查 /etc/crontab 文件、etc/cron.d/ 目录、以及 /var/spool/cron 目录中的改变。如果发现了改变，它们就会被载入内存。这样，当某个 crontab 文件改变后就不必重新启动守护进程了。

安装crontab:

yum install crontabs

说明：
/sbin/service crond start //启动服务
/sbin/service crond stop //关闭服务
/sbin/service crond restart //重启服务
/sbin/service crond reload //重新载入配置

查看crontab服务状态：service crond status

手动启动crontab服务：service crond start

查看crontab服务是否已设置为开机启动，执行命令：ntsysv

加入开机自动启动:
chkconfig –level 35 crond on

每一小时执行myscript.php如下：
```shell
# crontab -e
00 * * * * /usr/local/bin/php /home/john/myscript.php
```
注意：你的脚本文件的执行权限
如果你的PHP脚本可以通过URL触发，你可以使用lynx或curl或wget来配置你的Crontab。
下面的例子是使用Lynx文本浏览器访问URL来每小时执行PHP脚本。Lynx文本浏览器默认使用对话方式打开URL。但是，像下面的，我们在lynx命令行中使用-dump选项来把URL的输出转换来标准输出。

```shell
00 * * * * lynx -dump http://www.jb51.net/myscript.php
```
下面的例子是使用CURL访问URL来每5分执行PHP脚本。Curl默认在标准输出显示输出。使用”curl -o”选项，你也可以把脚本的输出转储到临时文件。

```shell
*/5 * * * * /usr/bin/curl -o temp.txt http://www.jb51.net/myscript.php
```
下面的例子是使用WGET访问URL来每10分执行PHP脚本。-q选项表示安静模式。”-O temp.txt”表示输出会发送到临时文件。

```shell
*/10 * * * * /usr/bin/wget -q -O temp.txt http://www.jb51.net/myscript.php
```

参数：
- -e 　编辑该用户的计时器设置。
- -l 　列出该用户的计时器设置。
- -r 　删除该用户的计时器设置。
- -u<用户名称> 　指定要设定计时器的用户名称。

crontab 格式：

基本格式 :
```
分钟   小时   日   月   星期   命令

*       *     *    *     *      *
```

第1列表示分钟1～59 每分钟用*或者 */1表示</br>
第2列表示小时1～23（0表示0点）</br>
第3列表示日期1～31</br>
第4列 表示月份1～12</br>
第5列标识号星期0～6（0表示星期天）</br>
第6列要运行的命令

记住几个特殊符号的含义:
- “*”代表取值范围内的数字,
- “/”代表”每”,
- “-”代表从某个数字到某个数字,
- “,”分开几个离散的数字


```bash
crontab文件的一些例子：
30 21 * * * /usr/local/etc/rc.d/lighttpd restart
上面的例子表示每晚的21:30重启apache。
45 4 1,10,22 * * /usr/local/etc/rc.d/lighttpd restart
上面的例子表示每月1、10、22日的4 : 45重启apache。
10 1 * * 6,0 /usr/local/etc/rc.d/lighttpd restart
上面的例子表示每周六、周日的1 : 10重启apache。
0,30 18-23 * * * /usr/local/etc/rc.d/lighttpd restart
上面的例子表示在每天18 : 00至23 : 00之间每隔30分钟重启apache。
0 23 * * 6 /usr/local/etc/rc.d/lighttpd restart
上面的例子表示每星期六的11 : 00 pm重启apache。
0 */1 * * * /usr/local/etc/rc.d/lighttpd restart
每一小时重启apache
0 23-7/1 * * * /usr/local/etc/rc.d/lighttpd restart
晚上11点到早上7点之间，每隔一小时重启apache
0 11 4 * mon-wed /usr/local/etc/rc.d/lighttpd restart
每月的4号与每周一到周三的11点重启apache
0 4 1 jan * /usr/local/etc/rc.d/lighttpd restart
*/1 * * * * /usr/local/php7/bin/php -f /usr/local/nginx/html/includes/kdp_user_to_oes_oes_user_amount.php 每分钟执行一次php脚本
```