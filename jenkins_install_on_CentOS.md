CentOS7.4下安装Jenkins
===========
### history命令：
```shell
    1  yum search java
    2  yum install -y java-1.8.0-openjdk.x86_64
    3  java -version
    4  yum search jenkins
    5  sudo wget -O /etc/yum.repos.d/jenkins.repo http://pkg.jenkins-ci.org/redhat/jenkins.repo
    6  sudo rpm --import https://jenkins-ci.org/redhat/jenkins-ci.org.key
    7  sudo yum install jenkins
    8  systemctl status jenkins
    9  systemctl start jenkins
   10  systemctl status jenkins
```

### 参考链接：
* https://blog.csdn.net/zhiaini06/article/details/45287671
*******
*******





Shell爬取某问题下所有妹子的图片
========
### 参考链接：
* https://zhuanlan.zhihu.com/p/28258932

[自己拥有一台服务器可以做哪些很酷的事情？ - 娶不到静香的胖虎的回答 - 知乎](https://www.zhihu.com/question/40854395/answer/357230006) <br />


*******
*******
