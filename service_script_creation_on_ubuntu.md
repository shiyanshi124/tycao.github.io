ubuntu ����ű���д
===========
* ��Ҫ����
	* ���÷��������ű�
	* ���������ű�
	* ��������
### ���÷��������ű�
**��ubuntu 16.04��,�����Ŀ¼Ϊ`/lib/systemd/system`����������Ҫ����һ����Ϊ`caosx`�ķ��񣬾��ڴ�Ŀ¼�����һ���ļ���Ϊ`caosx.service`���ļ����ɡ�** �����ݰ�����<br />
```shell
[Unit]
Description=xxxxxxxxxxxxxxxx #�������� 
After=network.target #����ָ������������ǰ������
Documentation=  #�����ļ��ĵ�ַ��http://baidu.com/ ����ȱʡ

[Service]
# User=  �����û�  
# Group=  �����û���
ExecStart=  #������������������
# ExecStop=  ������ֹ�����ȱʡ


[Install] #������������������Լ��Ƿ񿪻�������
WantedBy=multi-user.target #�����񿪻������󣬻����ʲô�ļ��У�Ӱ������˳��
```

**���������﷨�����������Ǿ�һ�����ӣ���дһ���Զ���ķ���ű�caosx.service**�����������£�<br />
#### /lib/systemd/system/caosx.service
* ��������ű���
```shell
touch /lib/systemd/system/caosx.service
chmod 777 /lib/systemd/system/caosx.service
vi /lib/systemd/system/caosx.service
```

��`/lib/systemd/system/caosx.service`�ļ�������һ�����ݣ�<br />

```shell
[Unit]
Description=My Fucking Service	# ����
After=network.target

[Service]
ExecStart=/root/server_http	# ��Ҫ���е�·��
SuccessExitStatus=143	# �ɹ�ʱ���ص�״̬
TimeoutStopSec=10
Restart=on-failure	# ������dead���Զ�����
RestartSec=5	# 5�������


[Install]
WantedBy=multi-user.target
```
���ˣ�����ű��Ѿ�������ɣ�

:warning: **��Ҫע����ǣ� ��������������Ҫ�������·������ʽ����Ϊ�ڷ�������ʱ��������صĻ���������δ���ء�**


### �� `/lib/systemd/system` ·������Ӵ��ļ�������Ϊ`caosx.service`�󣬾Ϳ���ͨ�� `systemctl start caosx` �������������ˣ�<br />
!["service_script_01"](https://github.com/tycao/tycao.github.io/blob/master/shell_crawler/service_script_01 "service_script_01")<br />









***********
### �ο����ӣ�https://aak1247.coding.me/java-jar-as-service.html