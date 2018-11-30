1，什么是线程安全 （参考书：https://book.douban.com/subject/10484692/）
答：
	多线程环境下，无论怎么使用该线程，程序运行结果都是可预期的，即为线程安全；

2，都说String是不可变的，为什么我可以这样做呢
   String a = "1";
   a = "2";
答：
	string底层是final的char数组，自然是不可变的，倘若把string拆开来看：
	final char data[] = {'a', 'b', 'c'};
	String str = new String(data);
	所以题中第二行a="2"，只不过是换了一个数组对象罢了，所以可以如题那样做；

3，HashMap的实现原理
答：
	jdk1.7：底层是数组+链表
		put()：首先计算出传入key的hashcode，然后根据数组长度取模计算在数组中的index；
		当index冲突时，hashmap会在table处形成链表，将数据插入到链表头处；
		get()：首先计算出传入key的index，如果该位置有链表就遍历链表；
	*jdk8--> 底层是数组+链表+红黑树，红黑树的好处是链表很长的话时间复杂度为0(log n)，相较于链表的0(n)提高了效率

4，写出三种单例模式，如果能考虑线程安全最好
答：
	懒汉式（加同步）：
	public class Singleton {
		private static Singleton instance;
		private Singleton (){}

		public static synchronized  Singleton getInstance() {
		if (instance == null) {
			instance = new Singleton();
		}
		return instance;
		}
	}
	饿汉式：
	public class Singleton {
		private static Singleton instance = new Singleton();
		private Singleton (){}
		public static Singleton getInstance() {
		return instance;
		}
	}
	内部类：
	public class Singleton {
		private static class SingletonHolder {
		private static final Singleton INSTANCE = new Singleton();
		}
		private Singleton (){}
		public static final Singleton getInstance() {
		return SingletonHolder.INSTANCE;
		}
	}
5，ArrayList和LinkedList有什么区别
答：
	ArrayList底层是数组，内部操作都是针对数组的操作；
	LinkedList底层是链表，循环双向链表；
	当需求要在数据的后面添加数据&&需要随机访问数据时--->ArrayList
	当需求要在数据的前面/中间添加数据&&顺序访问数据时--->LinkedList

6，实现线程的2种方式
答：
	1. 继承Thread类
	2. 实现Runnable接口
7，JVM的内存结构
答：
	1.堆：存放对象实例，jvm最大的一部分
	2.栈：每次方法调用创建一个栈帧，每个栈帧遵守后劲先出；此外包括基本数据类型和对象引用等；
	3.方法区：静态的运行时不会被改变，存放常量池，string池，static变量等；
8，Lock与Synchronized的区别
答：
	1.存在层次：Synchronized为关键字，lock是一个类
	2.锁的释放：
		Synchronized--->1、以获取锁的线程执行完同步代码，释放锁 2、线程执行发生异常，jvm会让线程释放锁
		Lock--->在finally中必须释放锁，不然容易造成线程死锁
	3.锁的获取：
		Synchronized--->假设A线程获得锁，B线程等待。如果A线程阻塞，B线程会一直等待
		Lock--->分情况而定，Lock有多个锁获取的方式，具体下面会说道，大致就是可以尝试获得锁，线程可以不用一直等待
	4.锁状态：
		Synchronized--->无法判断
		Lock--->可判断
	5.锁类型：
		Synchronized--->可重入 不可中断 非公平
		Lock--->可重入 可判断 可公平（两者皆可）
	6.性能：
		Synchronized--->少量同步
		Lock--->大量同步
9，数据库隔离级别有哪些，各自的含义是什么，MYSQL默认的隔离级别是是什么。
答：
	1.READ UNCIMMITTED（未提交读）
		事务中发生的修改，即使没有提交，其他事务也可以看得到，会造成脏读等问题；
	2.READ COMMITTED（提交读）
		只能看到事务的结果，会有不可重复读的问题
	3.REPEATABLE READ（可重复读）
		解决了脏读，但无法解决幻读
	4.SERIALIZABLE（可串行化）
		强制串行执行事务，所以避免了幻读，但是由于大量上锁，性能会比较地下
	Mysql默认为可重复读
10，请解释如下jvm参数的含义：
答：
-server -Xms512m -Xmx512m -Xss1024K
	server模式 初始堆内存512m 最大512m 每个线程的堆栈大小1024k
-XX:PermSize=256m -XX:MaxPermSize=512m -XX:MaxTenuringThreshold=20 XX:CMSInitiatingOccupancyFraction=80 -XX:+UseCMSInitiatingOccupancyOnly。
	持久代大小256m 最大512m 垃圾最大年龄20 使用cms作为垃圾回收器，使用80%后进行收集 使用手动定义初始化定义开始CMS收集
