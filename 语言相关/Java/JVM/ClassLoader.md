## ClassLoader类结构
ClassLoader通常会用到：
- defineClass(byte[],int,int)
- findClass(String)
- loadClass(String)
- resolveClass(Class<?>)

defineClass()通常是和findClass()方法一起使用的，我们通过直接覆盖ClassLoader父类的findClass方法来实现类的加载规则，从而取得要加载类的字节码。然后调用defineClass方法生成类的Class对象，
如果想在类被加载到JVM时就被Link，那么可以调用resolveClass方法。

如果不想重新定义加载类的规则，也没有复杂的处理逻辑，只想在运行时能够加载自己的一个类，直接调用loadClass方法获取这个类的Class对象。

下面会以Tomcat的ClassLoader举例体会ClassLoader的设计、应用

## JVM提供的ClassLoader

整个JVM平台提供三层ClassLoader：
- BootstrapClassLoader根类加载器，它主要加载JVM自身工作需要的类，同时它是有JVM自己控制的，我们访问不到这个类
- ExtClassLoader扩展类加载器，加载位置：jre\lib\ext，**ExtClassLoder是顶层父类**
- AppClassLoader应用类加载器，父类是ExtClassLoader，如果自己实现类加载器，不管直接实现抽象类ClassLoader，还是继承URLClassLoader等，它的父加载器都是AppClassLoader。加载位置：classpath


## 类加载过程

1. findClass 先找到.class文件并加载到内存中

2. Linking
    - .class文件规范验证，对于类的字节码要做检测，保证格式正确、行为正确。
    - Class类数据结构分析以及内存分配，在这个阶段准备代表每个类总定义的字段、方法和实现接口所必需的数据结构
    - 解析，在这个阶段类装入器装入类所引用的其他所有类。
3. init 类中静态属性和初始化赋值，以及静态块的执行

## Tomcat的ClassLoader

Tomcat的非JVM顶级ClassLoader是StandardClassloader，它是在Bootstrap类的initClassLoader方法中创建的，
Bootstrap调用ClassLoaderFactory的createClassLoader()方法创建StandardClassLoader对象。默认指定AppClassLoader为它的父类

我们知道，一个应用在tomcat中由一个StandardContext来表示，由StandardContext来解释Web应用的web.xml配置文件实例化所有的Servlet。
那么Servlet是如何被加载的呢？StandardContext类的startInternal()方法在StandardContext初始化时会检查loader是否存在，不存在就创建一个WebappLoader对象，
```java
if(getLoader()=null){
    WebappLoader webappLoader = new WebappLoader(getParentClassLoader());
    webappLoader.setDelegate(getDelegate()); setLoader(webappLoader);
}
```

再看StandardWrapper类的loadServlet()方法可以发现，所有的Servlet都是InstanceManager实例化的，而且这个类的ClassLoader也是获取StandardContext的Loader中的ClassLoader，
也就是前面设置的WebappClassLoader，所以Servlet的ClassLoader是Webapploader。

WebappClassLoader的加载机制如下：
1. 首先检查在WebappClassLoader中是否已经加载过了，是的话一定在缓存容器resourceEntries中
2. 否则，用findLoadedClass()方法继续检查在JVM中是否已经加载过
3. 如果都没有，先调用AppClassLoader加载请求的类，也就是在classpath中寻找
4. 检查请求的类是否在packageTriggers定义的包名下，如果在则通过StandardClassLoader类来加载它。
5. 如果依然没有找到，将由WebappClassLoader来加载，

## 自定义ClassLoader
```java

import java.net.URL;
import java.net.URLClassLoader;

/**
 * @program: sortalgorithm
 * @description:
 * @author: John_Xia
 * @create: 2018-11-29 17:08
 */
public class URLPathClassLoader extends URLClassLoader {

    private String pakName="com.xsj.sortalgorithm";
    public URLPathClassLoader(URL[] urls, ClassLoader parent) {
        super(urls, parent);
    }

    protected Class<?> findClass(String name) throws ClassNotFoundException{
        Class<?> aClass=findLoadedClass(name);
        if(aClass!=null){
            return aClass;
        }
        if(!pakName.startsWith(name)){
            return super.loadClass(name);
        }else {
            return findClass(name);
        }

    }
}

```
将指定目录转化成URL路径，然后作为参数创建URLPathClassLoader对象，那么这个ClassLoader在加载的时候就在URL指定的目录下查找指定的类文件。