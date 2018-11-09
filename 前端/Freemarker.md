FreeMarker

- 后端代码demo
      package cn.xsj.demo;

      import java.io.File;
      import java.io.FileWriter;
      import java.io.IOException;
      import java.io.Writer;
      import java.util.ArrayList;
      import java.util.HashMap;
      import java.util.List;
      import java.util.Map;

      import freemarker.template.Configuration;
      import freemarker.template.Template;
      import freemarker.template.TemplateException;

      public class Test {
      	public static void main(String[] args) throws IOException, TemplateException {

      		//1.获得当前版本、创建一个配置对象
      		Configuration c=new Configuration(Configuration.getVersion());
      		//2.设置模板所在的目录
      		c.setDirectoryForTemplateLoading(new File("D:\\j1ee\\freemarkerDemo\\src\\main\\resources"));
      		//3.设置字符集
      		c.setDefaultEncoding("utf-8");
      		//4.获取模板对象
      		Template t=c.getTemplate("test.ftl");
      		//5.创建数据模型
      		Map map=new HashMap();
      		map.put("name", "真帅");
      		map.put("message", "zsszs");
      		map.put("success", false);

      		List goodsList=new ArrayList();
      		Map goods1=new HashMap();
      		goods1.put("name", "苹果");
      		goods1.put("price", 5.8);
      		Map goods2=new HashMap();
      		goods2.put("name", "香蕉");
      		goods2.put("price", 2.5);
      		Map goods3=new HashMap();
      		goods3.put("name", "橘子");
      		goods3.put("price", 3.2);
      		goodsList.add(goods1);
      		goodsList.add(goods2);
      		goodsList.add(goods3);
      		map.put("goodsList", goodsList);


      		//6.创建一个输出流对象

      		Writer out=new FileWriter("d:\\test.html");

      		//7.输出
      		t.process(map, out);

      		//8.关闭
      		out.close();
      	}
      }




1. 指令
   <#assign>用于定义一个变量
       <#assign linkman="周先生">
       联系人：${linkman}
   <#include "head.ftl">用于模板文件的嵌套
   <#if >
   在模板文件上添加
   <#if success=true>
     1
   <#else>
    2
   </#if>
   在代码中赋值
   map.put("success", true);
   <#list >

   在模板文件上添加
   <#list goodsList as goods>
     {goods_index+1} 商品名称： {goods.name} 价格：${goods.price}<br>
   </#list>
   在后端代码中添加
            List goodsList=new ArrayList();
            Map goods1=new HashMap();
            goods1.put("name", "苹果");
            goods1.put("price", 5.8);
            Map goods2=new HashMap();
            goods2.put("name", "香蕉");
            goods2.put("price", 2.5);
            Map goods3=new HashMap();
            goods3.put("name", "橘子");
            goods3.put("price", 3.2);
            goodsList.add(goods1);
            goodsList.add(goods2);
            goodsList.add(goods3);

   map.put("goodsList", goodsList);
2. 内建函数
   - size
     我们通常要得到某个集合的大小
     我们使用size函数来实现，代码如下：
     共  ${goodsList?size}  条记录
   - eval
     转换JSON字符串为对象
         <#assign text="{'bank':'工商银行','account':'10101920201920212'}" />
         <#assign data=text?eval />

     开户行：{data.bank}  账号：{data.account}
   - date/time/datetime/日期格式化
     在模板文件中添加：
     当前日期：${today?date} <br>
     当前时间：${today?time} <br>
     当前日期+时间：${today?datetime} <br>
     日期格式化：  ${today?string("
     yyyy
     年MM月")}
     在后端代码中添加：
     dataModel.put("today", new Date());

   - c
     数字转换为字符串
     模板文件中添加
     累计积分：{point} 累计积分：{point?c}
     后端代码中添加
     map.put("point",123456789);


3. 空值处理运算符
   - 判断某变量是否存在
     <#if aaa??>
       aaa变量存在
     <#else>
       aaa变量不存在
     </#if>
   - 设定缺失变量的默认值
     ${aaa!'-'}
     当aaa为null则返回！后边的内容-
4. 算术运算符
   Free marker支持所有的算术运算，具体和java一样
   但是
   <>号和标签符号会混，所以
   ">="可以用"gte"替代
   "<="可以用"lte"替代
   或者使用括号确定优先级
   <#if (x>y)>
    ...
   </#if>
