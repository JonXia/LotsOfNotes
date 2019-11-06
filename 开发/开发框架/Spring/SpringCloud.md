### Ribbon核心组件IRule

IRule：根据特定算法从服务列表中选取一个需要访问的服务

#### 七大方法

==IRule是一个接口，七大方法是其自带的落地实现类==

- RoundRobinRule：轮询（默认方法）
- RandomRule：随机
- AvailabilityFilteringRule：先过滤掉由于多次访问故障而处于断路器跳闸状态的服务，还有并发的连接数量超过阈值的服务，然后对剩余的服务进行轮询
- WeightedResponseTimeRule：根据平均响应时间计算服务的权重。统计信息不足时会按照轮询，统计信息足够会按照响应的时间选择服务
- RetryRule：正常时按照轮询选择服务，若过程中有服务出现故障，在轮询一定次数后依然故障，则会跳过故障的服务继续轮询。
- BestAvailableRule：先过滤掉由于多次访问故障而处于断路器跳闸状态的服务，然后选择一个并发量最小的服务
- ZoneAvoidanceRule：默认规则，符合判断server所在的区域的性能和server的可用性选择服务

#### 切换规则方法

只需在==配置类==中配置一个返回具体方法的bean即可

```java
@Bean
public IRule MyRule(){
        return new RandomRule();
    }
```

### 自定义Ribbon负载均衡算法

#### 配置及包位置

1. 自定义的Ribbon算法类不能放在主启动类所在的包及子报下（确切来说是不能放在@ComponentScan注解的包及子包下），否则会被全局应用到Ribbon服务中。应该把自定义算法类放在另外新建的包下，且这个类应该是为==配置类==。（其实与普通切换负载均衡规则类似，只不过是位置不同而已，普通的可以放在主启动类所在的包，自定义的要放在外面的包下）
2. 主启动类添加@RibbonClient(name = "微服务名",configuration = XXX.class)注解指定需要用到负载均衡的微服务名及自定义算法的class对象。

```java
@SpringBootApplication
@EnableEurekaClient
@RibbonClient(name = "MICROSERVICECLOUD-DEPT",configuration = MyRule.class)
public class Consumer80_APP {
    public static void main(String[] args) {
        SpringApplication.run(Consumer80_APP.class,args);
    }
}
```

####通过修改源代码获得自定义算法

目标：每个服务调用5次后再进行轮询（调用次数不是很对，懒得改了)

```java
package com.Rules;

import com.netflix.client.config.IClientConfig;
import com.netflix.loadbalancer.AbstractLoadBalancerRule;
import com.netflix.loadbalancer.ILoadBalancer;
import com.netflix.loadbalancer.RoundRobinRule;
import com.netflix.loadbalancer.Server;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.Configuration;

import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;


public class MyRule extends AbstractLoadBalancerRule {

    private AtomicInteger nextServerCyclicCounter;
    private static final boolean AVAILABLE_ONLY_SERVERS = true;
    private static final boolean ALL_SERVERS = false;
    private int total = 0;
    private int currentIndex = 0;

    private static Logger log = LoggerFactory.getLogger(RoundRobinRule.class);

    public MyRule() {
        nextServerCyclicCounter = new AtomicInteger(0);
    }

    public MyRule(ILoadBalancer lb) {
        this();
        setLoadBalancer(lb);
    }

    public Server choose(ILoadBalancer lb, Object key) {
        if (lb == null) {
            log.warn("no load balancer");
            return null;
        }

        Server server = null;
        int count = 0;
        while (server == null && count++ < 10) {
            List<Server> reachableServers = lb.getReachableServers();
            List<Server> allServers = lb.getAllServers();
            int upCount = reachableServers.size();
            int serverCount = allServers.size();

            if ((upCount == 0) || (serverCount == 0)) {
                log.warn("No up servers available from load balancer: " + lb);
                return null;
            }
            if (total > 5) {
                total = 0;
                int nextServerIndex = incrementAndGetModulo(serverCount);
                currentIndex = nextServerIndex;
                server = allServers.get(nextServerIndex);
            }else {
                if (currentIndex>=serverCount) {
                    currentIndex = 0;
                }
                server = allServers.get(currentIndex);
                total++;
            }


            if (server == null) {
                /* Transient. */
                Thread.yield();
                continue;
            }

            if (server.isAlive() && (server.isReadyToServe())) {
                return (server);
            }

            // Next.
            server = null;
        }

        if (count >= 10) {
            log.warn("No available alive servers after 10 tries from load balancer: "
                    + lb);
        }
        return server;
    }

    /**
     * Inspired by the implementation of {@link AtomicInteger#incrementAndGet()}.
     *
     * @param modulo The modulo to bound the value of the counter.
     * @return The next value.
     */
    private int incrementAndGetModulo(int modulo) {
        for (;;) {
            int current = nextServerCyclicCounter.get();
            int next = (current + 1) % modulo;
            if (nextServerCyclicCounter.compareAndSet(current, next))
                return next;
        }
    }


    public Server choose(Object key) {
        return choose(getLoadBalancer(), key);
    }


    public void initWithNiwsConfig(IClientConfig clientConfig) {
    }
}

```

## Feign负载均衡

Feign是一个声明式WebService客户端，使用方法时定义一个接口并在上面添加注解即可。Feign支持可拔插式的编码器和解码器。Spring Cloud对Feign进行了封装，使其支持SpringMVC和HttpMessageConverters。Feign可以与Eureka和Ribbon组合使用以支持负载均衡。

[Feign源码]: https://github.com/OpenFeign/Feign

### 使用案例

1. 新建Feign模块，加入依赖（其实跟80消费者差不多，主要是多了Feign依赖）

```xml
    <dependencies>
        <dependency>
            <groupId>com.XXX</groupId>
            <artifactId>microservice-api</artifactId>
            <version>${project.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-feign</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-eureka</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-ribbon</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-config</artifactId>
        </dependency>
        <!--热部署-->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>springloaded</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-devtools</artifactId>
        </dependency>
    </dependencies>
```

1. 因为Feign开发其实是面向接口编程，所以Feign接口可以放在api模块中供各模块使用，所以要在api模块中添加Feign依赖
2. 在api中编写接口，接口上添加@FeignClient注解，并通过value指定作用的微服务名

```java
@FeignClient(value = "MICROSERVICECLOUD-DEPT")
public interface DeptClientService {

    @PostMapping("/dept")
    public boolean addDept(Dept dept);

    @GetMapping("/dept")
    public List<Dept> findAll();

    @GetMapping("/dept/{id}")
    public Dept findById(@PathVariable("id")Integer id);
}

```

1. 在Feign模块中编写Controller，并注入FeignClient接口，直接调用service接口中的方法即可（因为声明Feign接口时已经指定过微服务，所以访问时会正确地找到微服务）

```java
@RestController
@RequestMapping("/consumer")
public class ConsumerDeptController {
    @Autowired
    private DeptClientService service;

    @PostMapping("/dept")
    public boolean addDept(Dept dept){
        return service.addDept(dept);
    }

    @GetMapping("/dept")
    public List<Dept> findAll(){
        return service.findAll();
    }

    @GetMapping("/dept/{id}")
    public Dept findById(@PathVariable("id")Integer id){
        return service.findById(id);
    }
}
```

1. 修改Feign模块的主启动类，加入@EnableFeignClients注解和@ComponentScan注解（主要是扫描api中声明的接口）

```java
@SpringBootApplication
@EnableEurekaClient
@EnableFeignClients(basePackages = {"com.XXX"})
@ComponentScan("com.XXX")
public class Consumer80Feign_APP {
    public static void main(String[] args) {
        SpringApplication.run(Consumer80Feign_APP.class,args);
    }
}
```

1. 启动后访问，即会按照轮询的方式调用provider集群

### 总结

- Feign通过接口方法调用REST服务，在Eureka中查找对应的服务
- Feign集成了Ribbon技术，所以也支持负载均衡（轮询）

## Hystrix断路器

### 分布式系统面临的问题

#### 扇出

多个微服务互相调用的时候，如果A调用B、C，而B、C又继续调用其他微服务，这就是扇出（像一把扇子一样慢慢打开。

#### 服务雪崩

- 删除过程中，如果某一个环节的服务出现故障或连接超时，就会导致前面的服务占用越来越多的资源，进而引起系统崩溃，就是“雪崩效应”。
- 对于高流量的应用来说，单一的后端依赖会导致服务器所有的资源都在几秒钟内饱和。比失败更糟糕的是，这些应用程序还可能导致服务之间的延迟增加，备份队列，线程和其他系统资源紧张，导致整个系统发生更多的级联故障。这些都表示需要==对故障和延迟进行隔离和管理==，以便单个依赖关系的失败，不能取消整个应用程序或系统。

### Hystrix介绍

- Hystrix是一个用于处理分布式系统延迟和容错的开源库。分布式系统中，依赖避免不了调用失败，比如超时，异常等。Hystrix能保证在出现问题的时候，不会导致整体服务失败，避免级联故障，以提高分布式系统的弹性。
- Hystrix类似一个“断路器”，当系统中异常发生时，断路器给调用返回一个符合预期的，可处理的FallBack，这样就可以避免长时间无响应或抛出异常，使故障不能再系统中蔓延，造成雪崩。

#### 服务熔断

- 熔断机制的注解是@HystrixCommand
- 熔断机制是应对雪崩效应的一种==链路保护机制==，一般存在于服务端
- 当扇出链路的某个服务出现故障或响应超时，会进行==服务降级==，进而==熔断该节点的服务调用==，快速返回“错误”的相应信息。、
- Hystrix的熔断存在阈值，缺省是5秒内20次调用失败就会触发
- Hystrix源码：[Hystrix源码]: https://github.com/Netflix/Hystrix