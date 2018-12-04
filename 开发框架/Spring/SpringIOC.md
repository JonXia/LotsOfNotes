## 注入方式
- setter方式注入

    比较简单不写了
- 构造方式注入

    同上

## bean配置项和作用域
- id

- class

- scope
    - singleton：每个ioc容器中只存在一份
    - propertype: 每次请求都会创建一个新的实例，destroy方式不生效
    - request：每个http请求只在当前request有效
    - session：同上
    - global session:
- constructor arguments

- properties

- autowiring mode

- lazy-init mode

- init/destruction mode

## Spring常用注解
@Autowired自动装配，可以理解为一个传统的setter方法，可以用在属性，setter方法，构造等
除了常用的属性注入，还可以通过用在setter方法上，以提供特定泛型的bean给集合对象用
```java
private Set<Apple> apples;

@Autowired
public void setApples(Set<Apple> apples){
    this.apples=apples;
}
```
如果希望有序加载，可以让bean实现Order接口或@Order(int)注解

@Qualifier("xxx")
指定bean注入到具体位置（与@Autowired使用缩小注入范围）
如果要用集合的bean用@Resource

总结：
@Autowired适用于field,constructors,multi-argument,methods这些允许在参数级别使用@Qualifier注解缩小范围的情况；
@Resource适用于成员变量，只有一个参数的setter方法，所以在目标是构造器，或者一个多参数方法时，用qualifiers



