�ڰ������ϴ�Լ���shadowsocks
======
* ������
	* CentOS 7.4, 64bit�� ������������ȣ��� shadowsocks<br />
	![centos_version](https://github.com/tycao/tycao.github.io/blob/master/cloud_aliyun_src/centos_version.png "centos_version")<br />
* ���ȣ���Ҫ��һ̨�����Ʒ������������������������ȵ�CentOS 7.4
* ������Ʒ�����֮���������������װ `python-pip` �� `shadowsocks`:<br />
```shell
yum install python-pip
pip install shadowsocks
```
* ����������ɺ󣬰�װ�������<br />
```shell
{
    "server":"0.0.0.0",
    "server_port":443,
    "local_address":"127.0.0.1",
    "local_port":1080,
    "password":"12345",
    "timeout":300,
    "method":"aes-256-cfb",
    "fast_open":false,
    "workers":5
}
```
!["shadows_json"](https://github.com/tycao/tycao.github.io/blob/master/cloud_aliyun_src/shadowsocks_json.png "shadows_json")<br />

:warning: <br /> 
* ע���޸� password Ϊ�Լ�������
* workers ��ʾ�����Ľ������� 

* ����shadowsocks����:<br />
```shell
shadowsocks����: ssserver -c /etc/shadowsocks.json -d start 
shadowsocks����: ssserver -c /etc/shadowsocks.json -d restart 
shadowsocks�ر�: ssserver -c /etc/shadowsocks.json -d stop
```

****
ʹ�ñ���Shdowsocks�ͻ���, ���Ӱ����Ʒ�����������
[shadowsocks android �汾](https://github.com/shadowsocks/shadowsocks-android/releases)<br />
[shadowsocks windows �汾](https://github.com/shadowsocks/shadowsocks-windows/releases)<br />
shadowsocks iPhone, iPad�汾,����ȥApp store������ �� firstwingy