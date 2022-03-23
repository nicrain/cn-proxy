# Shadowsocks Manager 搭建 Trojan 节点

## 开启 BBR 加速

``` bash
uname -r # 查看内核版本
echo "net.core.default_qdisc=fq" >> /etc/sysctl.conf
echo "net.ipv4.tcp_congestion_control=bbr" >> /etc/sysctl.conf
sysctl -p
sysctl net.ipv4.tcp_available_congestion_control
lsmod | grep bbr
reboot # 重启(可选)
```

## 开启端口

``` bash
ufw allow 80 
ufw allow 443 
ufw allow 4001
ufw status

oracle ubuntu only
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -P OUTPUT ACCEPT
iptables -F
```

## 安装证书

``` bash
apt update -y
apt install certbot
certbot certonly --standalone --agree-tos -n -d jp.trojanhub.xyz -m duyuanchao.me@gmail.com
```

## 安装 MariaDB 数据库

``` bash
apt install mariadb-server mariadb-client -y # 安装服务端和客户端
systemctl status mariadb # 查看运行状态
sudo systemctl start mariadb # 启动
sudo systemctl enable mariadb # 开机自启动
sudo mysql_secure_installation # 安全设置

use mysql;
update user set plugin='' where User='root';
flush privileges;
exit
```

创建数据库表

``` sql
mysql -uroot -p
create database trojan;
use trojan;
CREATE TABLE users (
      id INT UNSIGNED NOT NULL,                   
      password CHAR(56) NOT NULL UNIQUE,            
      quota BIGINT NOT NULL DEFAULT -1,
      download BIGINT UNSIGNED NOT NULL DEFAULT 0,  
     upload BIGINT UNSIGNED NOT NULL DEFAULT 0,    
      PRIMARY KEY (id),
     INDEX (password)
);
exit
```

## 安装 trojan

``` bash
bash -c "$(wget -O- https://raw.githubusercontent.com/trojan-gfw/trojan-quickstart/master/trojan-quickstart.sh)"
```

## 运行 trojan

``` bash
cat <<EOF >/usr/local/etc/trojan/config.json
{
    "run_type": "server",
    "local_addr": "0.0.0.0",
    "local_port": 443,
    "remote_addr": "127.0.0.1",
    "remote_port": 80,
    "password": [
        "password1",
        "password2"
    ],
    "log_level": 1,
    "ssl": {
        "cert": "/etc/letsencrypt/live/jp.trojanhub.xyz/fullchain.pem",
        "key": "/etc/letsencrypt/live/jp.trojanhub.xyz/privkey.pem",
        "key_password": "",
        "cipher": "ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:DHE-RSA-AES128-GCM-SHA256:DHE-RSA-AES256-GCM-SHA384",
        "cipher_tls13": "TLS_AES_128_GCM_SHA256:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_256_GCM_SHA384",
        "prefer_server_cipher": true,
        "alpn": [
            "http/1.1"
        ],
        "alpn_port_override": {
            "h2": 81
        },
        "reuse_session": true,
        "session_ticket": false,
        "session_timeout": 600,
        "plain_http_response": "",
        "curves": "",
        "dhparam": ""
    },
    "tcp": {
        "prefer_ipv4": false,
        "no_delay": true,
        "keep_alive": true,
        "reuse_port": false,
        "fast_open": false,
        "fast_open_qlen": 20
    },
    "mysql": {
        "enabled": true,
        "server_addr": "127.0.0.1",
        "server_port": 3306,
        "database": "trojan",
        "username": "root",
        "password": "AAaa112233",
        "cafile": ""
    }
}
EOF
```

相关命令操作

``` bash
systemctl status trojan
systemctl start trojan
systemctl restart trojan
systemctl enable trojan
```



## 安装 ssmgr-trojan-client

``` bash
apt install nodejs npm -y
npm -v
npm i -g ssmgr-trojan-client
screen -S trojan -dm ssmgr-trojan-client -l 0.0.0.0:4001 -k AAaa112233 --db-type mysql --db-address localhost:3306 --db-name trojan --db-user root --db-password AAaa112233
```

