MYSQL

1.表table

1. 表是一种结构化的文件，表中有，字段，行，主键
2. 每一个字段都有:
   1. 字段名称，
   2. 字段数据类型，
   3. 字段约束（空/非空），
   4. 字段长度
   命令：
   desc +tablename; 查看表结构
   show create table +tablename; 查看建表语句

2. SQL的分类五

1. 数据查询语言（DQL-data query language）——操作数据；select
2. 数据操纵语言（DML-data manipulation language）——操作数据insert,delete,update；
3. 数据定义语言（DDL-data definition language）——增删改表结构create,drop,alter；
4. 事物控制语言（TCL-transactional control language）——操作事物commit,rollback
5. 数据控制语言（DCL-data control language）——控制权限；grant,revoke

3. 条件查询六

1. between and用在数字是闭区间，前面是小值后面是大值；应用在字符区间则是前闭后开；
2. null表示什么都没有，不是值，只能用is/is not查询;
3. and 优先级比or高，其实无所谓，想让谁优先级高就在哪个条件加括号；
4. 包含查询，in相当于多个or，注意not in中的数据不能有null，这么用 not in(...is not null)就可以
       1. in(not in)
       select ename from emp where job in(not in) ('manager','SAlesman');
       #job是manager和job是salesman
       等价于
       select ename from emp where job = 'manager' or job='salesman';
5. 模糊查询：%任意字符，_任意一个字符
6. select distinct... ：distinct：去重只能出现在所有字段的最前，只是将结果集去除重复记录
7. case...when...then...when...then...else...end
   类似于java中的switch...case...


4.排序三

1. 排序采用order by子句，order by后面跟上排序字段，排序字段可以放多个，多个采用逗号间隔，order by默认采用升序；降序加desc，升序用asc；
2. 如果存在where子句那么order by必须放到where语句的后面；
       select ename,sal from emp order by sal desc,ename asc;
       当sal相同的时候，后面字段才会用得上，所以，
       通过sal降序排序，当sal相同时，ename正序排序；
3. order by后可以加数字表示根据第几个字段排序，但是健壮性不强；

5.数据处理函数/单行处理函数九

1. lower(字段)/upper()：转大/小写
2. substr(字段，起始位置，截取长度)
       select * from emp where substr(ename, 1,1)=upper('m');
       查询姓名以M开头所有的员工
3. length(..)：取长度
       select ename from emp where length(ename)=5;
4. trim(..)：去空格
       select ename,job from emp where job=trim(upper('   Manager '));
5. round(数字/数字字段[，四舍五入到小数点n位])：四舍五入
6. rand()：随机数
7. ifnull(字段，目标)：如果空，那么...
       select ename,(sal+ifnull(comm,0))*15  as yearsal from emp;
       如果comm补助为null，那么算0，加月薪*15=年薪yearsal；
8. str_to_date (字符串，匹配格式)：可以将字符串转换成date日期格式
       select * from emp where HIREDATE=str_to_date('1981-02-20','%Y-%m-%d');
       select * from emp where HIREDATE=str_to_date('02-20-1981','%m-%d-%Y');
       指定雇佣日期的信息
9. date_formate
       select date_format(now(),'%Y-%m-%d %H %i %s');
       2018-10-01 17:39:27

       select date_format(now(),'%Y-%M-%D %H:%i:%s');
       2018-October-1st 17:40:50
       查询现在时间，以格式化输出

6.分组函数/聚合函数/多行处理函数五

1. count()
   count(字段)，不为空的记录总数
   count(*)，满足条件的记录总数
       select count(*) from emp where comm is null;
       补助为空的总数
2. sum()
3. avg()
4. max()
5. min()
   注意：
   1. 多行处理函数自动忽略空值
   2.分组函数不能直接使用在where子句中 ：分组函数必须在分组完成之后进行，因为分组需要用到group by，而此时group by还没被执行
   mysql> select ename,sal from emp where sal > avg(sal);
   ERROR 1111 (HY000): Invalid use of group function

7.分组查询

1. 分组查询涉及到的两个字句是：
   1. group by
   2. having
2. group by
   若sql中含有group by子句，那么select 后面只能跟参与分组的字段和分组函数
   1.查询每个部门的平均薪水
       select deptno,avg(sal) avgsal
       from emp group by deptno ;

   2.计算不同部门中中不同工作岗位的最高薪水
       select deptno,job,max(sal) maxsal
       from emp group by deptno,job;
   就是deptno和job两个字段联合起来分组

       +--------+-----------+---------+
       | deptno | job       | maxsal  |
       +--------+-----------+---------+
       |     10 | CLERK     | 1300.00 |
       |     10 | MANAGER   | 2450.00 |
       |     10 | PRESIDENT | 5000.00 |
       |     20 | ANALYST   | 3000.00 |
       |     20 | CLERK     | 1100.00 |
       |     20 | MANAGER   | 2975.00 |
       |     30 | CLERK     |  950.00 |
       |     30 | MANAGER   | 2850.00 |
       |     30 | SALESMAN  | 1600.00 |
       +--------+-----------+---------+
       9 rows in set (0.03 sec)
   3.找出每个工作岗位的最高薪水，除了Manager之外
       select job,max(sal) from emp where job != 'MANAGER' group by job;
   4.找出每个工作岗位的平均薪水，要求显示平均薪水大于1500
       select job,avg(sal) from emp group by job having avg(sal)>1500;
3. having
   having和where功能都是为了完成数据的过滤；where和having 后面都要添加条件；
   	where在group by之前完成过滤，having在group by之后完成过滤；
   但是能在where中过滤的一定要过滤，无法过滤的数据，通常都是需要先分好组的，group by分好组了，又要在having中过滤肯定是不行的；
4. DQL的执行顺序
       select
       	...
       from
       	...
       where
       	...
       group by
       	...
       having
       	...
       order by
       	...
   执行顺序：
   1. from	从某张表中检索数据
   2. where     经过某条件进行过滤
   3. group by  然后分组
   4. having    分组之后不满意再分组
   5. select       查询出来
   6. order by   排序输出

8. 连接查询

1. 什么是连接查询？
   - 查询的时候只从一张表中检索数据，即单表查询
   - 在实际的开发中，数据并不是存储在一张表中的，是同时存储在多张表中，这些表和表之间存在关系，我们在检索的时候通常是需要将多张表联合起来取得有效数据，这种多表查询被称为连接查询或者叫跨表查询。
2. 连接查询根据出现年代分类：
   - SQL92
   - SQL99
3. 连接查询根据连接方式可以分为：
   - 内链接：返回链接所满足的链接条件记录
     - 等值连接：是非常常用的一种连接，前提必须是表之间有关联列
           select e.ename,d.dname
           from emp e,dept d
           where e.deptno=d.deptno;
       #SQL92语法，再想过滤的话用and加条件
           select e.ename,d.dname
           from emp e
           (inner ) join dept d
           on e.deptno=d.deptno;
       #SQL99语法把表连接操作用(inner )join...on...分离出来，再想过滤数据的话用where，结构清晰
     - 非等值连接：..
     - 自连接：同一张表，不常用
   - 外连接：不仅返回满足链接条件的记录，还将返回不满足链接条件的记录
     - 左外连接（左链接）：将左表全显示出来
     - 右外连接（右链接）：..
4. 当多张表进行连接查询，若没有任何条件进行限制，会产生笛卡儿积
5. 案例

   1.找出每一个员工对应的工资等级，要求显示员工名，工资，工资等级
    ```sql
   select e.ename,e.sal,s.grade
   from emp e join salgrade s
   on e.sal between s.losal and s.hisal;
    #SQL99的非等值连接
   select e.ename,e.sal,s.grade from emp e,salgrade s
   where e.sal between s.losal and s.hisal;
    #SQL92的非等值连接
    ```

   2.找出每一个员工的上级领导名称，要求显示员工名以及对应的领导名
    ```sql
   select w.ename,w.mgr,m.ename,m.empno
   from emp w join emp m
   on w.mgr=m.empno;#自连接的等值连接#13
    ```

   3.找出每一个员工对应的部门名称，要求部门名称全部显示
    ```sql
    select d.dname
    from emp e right outer join dept d
    on e.deptno=d.deptno;#15
    ```

   4.找出每一个员工对应的领导名，要求显示所有的员工
    ```sql
   select e.ename,m.ename
   from emp e left join emp m
   on e.mgr = m.empno;#14
    ```
   5.找出每一个员工对应的部门名称，以及该员工对应的工资等级
    ```sql
   select e.ename,d.dname,s.grade
   from emp e,dept d,salgrade s
   where
   e.deptno =d.deptno and
   e.sal between s.losal and s.hisal;#SQL92
   select e.ename,d.dname,s.grade
   from emp e
   join dept d on e.deptno=d.deptno
   join salgrade s on e.sal between s.losal and hisal;
   #SQL99
    ```

9.子查询

1. 什么是子查询？
   - select嵌套select
2. 子查询可以出现在哪儿？
   - select..(select)
   - from..(select)
   - where..(select)
3. 案例
   1.找出薪水比公司平均薪水高的员工，要求显示员工名和薪水（在where后使用子查询）
       select ename,sal from emp where sal>(select avg(sal) from emp);
   2.找出每个部门的平均薪水，并且找出相应的薪水等级（在from后使用子查询）
       select s.grade,t.deptno
       from
       (select avg(sal) avgsal,deptno from emp group by deptno) t join salgrade s
       on t.avgsal between s.losal and s.hisal;#3
   3.找出员工对应的部门名称（在select后使用子查询）
       select e.ename,(select d.dname from dept d where e.deptno=d.deptno) as dname from emp e;
4. 其它操作
   1.union，合并结果集
   注意，mysql中只要求结果集的个数相同，Oracle中不仅有以上要求对数据格式也要求相同；
       select ename,job from emp where job='MANAGER'
       union
       select ename,empno from emp where job='SALESMAN';#mysql可以用
   2.limit
   - limit用来获取一张表中的某部分数据
   - 只在M'ySQL中存在，不通用
         1.select * from t_user limit 3;//查询前三条数据
         2.select * from t_user limit 3,3;//从第四条数据开始，取3条数据
         #limit n 取前n条数据，一般会和order by配合使用
         #limit m,n 分页作法，从m+1条数据开始取n条数据
   - 大数据下如何做分页：
     MySQL并不是跳过offset行，而是取offset+N行，然后放弃前offset行，返回N行，所以当offset大的时候，效率自然低下；这样要么控制返回的总页数，要么对超过特定阈值的页数进行SQL改写：
         select a.* from 表1 a,
         (select id from 表1 where 条件 limit 100000,20) b
         where a.id=b.id;
         #定位id段，再进行连接查询

10.DDL、DML、常用数据类型

1. 格式
   1. DDL
          create table tableName(
             columnName dataType(length),
             ………………..
             columnName dataType(length)
          );
          set character_set_results='utf8';#创建表

          alter table ...add/drop/modify...#增删改字段

          drop table tableName;#删除数据和结构

          truncate table tableName;#删除数据

          create table emp_bak as select empno,ename,sal from emp where sal >3000;#复制表
      2.DML
          insert into tableName[(column[,column...])] values(value[,value...)]);#增，字段可以省略

          delete from tableName [where condition];

          update table_name set colum_name=newvalue,[,column_name=newvalue];#改
2. 常用数据类型
     Char(长度)          	定长字符串，存储空间大小固定，适合作为主键或外键
     Varchar(长度)       	变长字符串，存储空间等于实际数据空间
     double(有效数字位数，小数位)	数值型   DOUBLE(7,2)，7个有效数字2个小数位
     Float(有效数字位数，小数位) 	数值型
     Int( 长度)          	整型   int(3)表示最大为999
     bigint(长度)        	长整型
     Date              	日期型
     BLOB              	Binary Large OBject（二进制大对象）   图片等，视频太大不建议
     CLOB              	Character Large OBject（字符大对象） 4G+的大文本


