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
5. 配Nginx记得转发localhost/127.0.0.1的5000端口
