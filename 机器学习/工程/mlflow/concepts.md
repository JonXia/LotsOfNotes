1. 安装anaconda替换原有python2。[点击](https://blog.csdn.net/qq_36527339/article/details/81347353)
2. 更换Yum源
```bash
mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo_bak
```
```bash
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo
```
```shell
yum makecache
```
3. 克隆项目
```bash
git clone https://github.com/databricks/mlflow
```
里面有一些databricks写好的demo，可以随时运行查看
4. 启动Tracking UI
```bash
mlflow ui
```
默认是5000端口
5. 配Nginx，转发localhost/127.0.0.1的5000端口
6. 运行
```bash
mlflow pyfunc serve -m /Users/mlflow/mlflow-prototype/mlruns/0/7c1a0d5c42844dcdb8f5191146925174/artifacts/model -p 1234
```
启动服务，但是提升不要在生产环境中使用
7. 可以以Restapi的方式调参
```bash
curl -X POST -H "Content-Type:application/json; format=pandas-split" --data '{"columns":["alcohol", "chlorides", "citric acid", "density", "fixed acidity", "free sulfur dioxide", "pH", "residual sugar", "sulphates", "total sulfur dioxide", "volatile acidity"],"data":[[12.8, 0.029, 0.48, 0.98, 6.2, 29, 3.33, 1.2, 0.39, 75, 0.66]]}' http://127.0.0.1:1234/invocations
```

8. cdh配置
[点击1](https://www.jianshu.com/p/b9b8a6876402)
[点击2](https://www.jianshu.com/p/d2c524ec0d1b)
```bash
sudo systemctl start cloudera-scm-server
sudo tail -f /var/log/cloudera-scm-server/cloudera-scm-server.log
```

**注意：**
1. mlflow ui 不能在下载mlflow项目里直接打开，但在/root下，且由mlruns文件夹的地方运行才可以（不知道为什么，github上也有人提同样的issue，但没有解决）
2. 检查主机正确性时出现 “Cloudera 建议将 /proc/sys/vm/swappiness 设置为 0。当前设置为 30。” 的警告，进行如下设定
```bash
# vi /etc/sysctl.conf
vm.swappiness = 0
# sysctl –p
```
3. 检查主机正确性时出现 “已启用“透明大页面”，它可能会导致重大的性能问题。” 的警告，进行如下设定
```bash
echo never > /sys/kernel/mm/transparent_hugepage/enabled
echo never > /sys/kernel/mm/transparent_hugepage/defrag
```
