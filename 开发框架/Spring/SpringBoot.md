## 使用spring.profiles.active来分区配置

很多时候，我们项目在开发环境和生成环境的环境配置是不一样的，例如，数据库配置，在开发的时候，我们一般用测试数据库，而在生产环境的时候，我们是用正式的数据，这时候，我们可以利用profile在不同的环境下配置用不同的配置文件或者不同的配置

spring boot允许你通过命名约定按照一定的格式(application-{profile}.properties)来定义多个配置文件，然后通过在application.properyies通过spring.profiles.active来具体激活一个或者多个配置文件，如果没有没有指定任何profile的配置文件的话，spring boot默认会启动application-default.properties。

profile的配置文件可以按照application.properyies的放置位置一样，放于以下四个位置，

1. 当前目录的 “/config”的子目录下
2. 当前目录下
3. classpath根目录的“/config”包下
4. classpath的根目录下

在这里我们就定义俩个profile文件，application-cus1.properties和application-cus2.properties，并在俩个文件中都分别写上变量cusvar=cus1和cusvar=cus2

我们在application.properties也写上，并把profile切换到application-cus1.properties的配置文件

```properties
cusvar=cus3
spring.profiles.active=cus1
```

可以通过这样来测试

```java
@RestController
@RequestMapping("/task")
public class TaskController {

    @RequestMapping(value = {"/",""})
    public String hellTask(@Value("${cusvar}")String cusvar ){

        return "hello task !! myage is " + cusvar;
    }

}
```

在这里可以看到spring.profiles.active激活的profile不同，打印出来的结果也不一样。

还有一种叠加profile的方法

```properties
spring.profiles: testdb
spring.profiles.include: proddb,prodmq
```

## 多环境配置

在Spring Boot中多环境配置文件名需要满足`application-{profile}.properties`的格式，其中`{profile}`对应你的环境标识，比如：

> application-dev.properties：开发环境
>
> application-test.properties：测试环境
>
> application-prod.properties：生产环境

至于哪个具体的配置文件会被加载，需要在`application.properties`文件中通过`spring.profiles.active`属性来设置，其值对应`{profile}`值。
如：`spring.profiles.active=test`就会加载`application-test.properties`配置文件内容

针对各环境新建不同的配置文件`application-dev.properties`、`application-test.properties`、`application-prod.properties`
​     在这三个文件均都设置不同的`server.port`属性，如：dev环境设置为8080，test环境设置为9090，prod环境设置为80
​     application.properties中设置`spring.profiles.active=dev`，就是说默认以dev环境设置

测试不同配置的加载：
​     执行java -jar xxx.jar，可以观察到服务端口被设置为8080，也就是默认的开发环境（dev），执行java -jar xxx.jar --spring.profiles.active=test，可以观察到服务端口被设置为9090，也就是测试环境的配置（test）

执行java -jar xxx.jar --spring.profiles.active=prod，可以观察到服务端口被设置为80，也就是生产环境的配置（prod）

按照上面的实验，可以如下总结多环境的配置思路：
   application.properties中配置通用内容，并设置spring.profiles.active=dev，以开发环境为默认配置
   application-{profile}.properties中配置各个环境不同的内容
通过命令行方式去激活不同环境的配置。

### 高级应用

```java
/**
 * 发送邮件接口.
 */
public interface EmailService {
    /**发送邮件*/
    publicvoid send();
}
发送邮件的具体实现（dev-开发环境的代码）：
@Service
@Profile("dev") //开发环境的时候.
public class DevEmailServiceImpl implements EmailService{
 
    @Override
    publicvoid send() {
       System.out.println("DevEmailServiceImpl.send().开发环境不执行邮件的发送.");
    }
}
```

