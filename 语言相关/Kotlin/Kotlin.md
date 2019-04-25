Kotlin和Scala语法接近，还好学过Scala，记一些笔记

1.
一些kotlin基础
https://blog.csdn.net/lckj686/article/category/7689676

https://blog.csdn.net/weixin_41084236/article/details/83308910#commentBox

https://blog.csdn.net/su19921021/article/details/79244796

tricks

https://blog.csdn.net/sunny2come/article/details/80874066


2. ? !!
```
https://blog.csdn.net/lckj686/article/details/80448471
https://blog.csdn.net/xiaoluoli88/article/details/78082311
```
3. 单表达式函数
单表达式函数，即只有一个表单式的函数。当函数返回单个表达式时，可以省略花括号并且在 = 符号之后指定代码体即可
```kotlin
fun doubleValue(x: Int): Int = x * 2
```
当返回值类型可由编译器推断时，显式声明返回类型是可选的
```kotlin
fun doubleValue(x: Int) = x * 2
```

4. 闭包
Kotlin语言中有三种闭包形式：全局函数、自嵌套函数、匿名函数体
```kotlin
fun main(args: Array<String>) {
    // 执行test闭包的内容
    test
}
// 定义一个比较测试闭包
val test = if (5 > 3) {
    println("yes")
} else {
    println("no")
}
```
- 为什么会设计闭包这种结构？
从上述的例子来说，我们可以看出来，其实定义一个函数就好了，为什么设计编程语言的人要设计闭包这么一个结构呢？这就得从作用域开始说起。变量的作用域无非就是两种：**全局变量**和**局部变量**。那么，如何在外部调取局部的变量呢？答案就是——闭包。

这里，我们给闭包下一个定义：闭包就是能够读取其他函数内部变量的函数

 闭包可以用在许多地方。它的最大用处有两个，一个是前面提到的可以读取函数内部的变量，另一个就是让这些变量的值始终保持在内存中。举例：

```Kotlin
/**
 * 计数统计
fun justCount():() -> Unit{
    var count = 0
    */
    return {
        println(count++)
    }
}


fun main(args: Array<String>) {

    val count = justCount()
    count()  // 输出结果：0
    count()  // 输出结果：1
    count()  // 输出结果：2
}
```
有没有发现闭包这点的好处，闭包就是在函数被创建的时候，存在的一个私有作用域，并且能够访问所有的父级作用域。每个功能模块我们都能够拆解到不同fun里，不同fun里的变量保持相互调用的可能性，相互独立还彼此不影响。我们可以函数式编程了！

```
广义上来说，在Kotlin语言之中，函数、条件语句、控制流语句、花括号逻辑块、Lambda表达式都可以称之为闭包，但通常情况下，我们所指的闭包都是在说Lambda表达式。
```

自执行闭包

自执行闭包就是在定义闭包的同时直接执行闭包，一般用于初始化上下文环境。 例如：
```kotlin
{ x: Int, y: Int ->
    println("${x + y}")
}(1, 3)
```
5. 函数let,with,run,apply,also

https://blog.csdn.net/u013064109/article/details/78786646

懒加载

https://blog.csdn.net/liyi1009365545/article/details/84236433
