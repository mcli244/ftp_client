ftp client
---

### FTP服务器
- 控制端口：21
- 数据端口：20

### FTP连接模式
- 主动模式：服务端从20端口主动向客户端发起连接。
    - 客户端发送PORT命令，向服务器发送一个端口号，供服务器的20端口来连接。例如：客户端，命令端口为12345，向服务器发送的数据端口为12345 + 1 = 12346

- 被动模式：服务端在指定范围内的某各端口被动等待客户，被动发起连接。
    - 客户端发送PASV命令，服务器收到后回应一个端口号给客户端连接。

### 从FTP服务器下载文件的基本流程如下

1. 建立TCP连接，该协议默认使用21端口，当然可以指定其它端口，取决于服务器的配置。

2. 连接成功之后，服务器会发送一行欢迎文字，例如：220 welcome.
    其中左边的数字220表示就绪状态，220后面有一个空格，空格后面是提示文字。
    在解析命令应答的时候，只需要获取前面的数字即可。

3. 收到欢迎信息后，就要开始登陆了，先用USER命令发送用户名，服务器返回331状态。
    然后再用PASS命令发送登陆密码，服务器返回530表示密码错误，返回230表示密码正确。
    发送：USER anonymous
    接收：331 Anonymous login ok, send your complete email address as your password
    发送：PASS anonymous
    接收：230 Anonymous access granted, restrictions apply

4. 登陆成功之后，再发送一条TYPE I命令，进入二进制模式，这样获取文件数据的时候，就会以二进制字节流发送。
    避免以ASCII码格式发送文件数据。

5. 获取文件长度
   发送：SIZE /path/filename
   失败：550 /path/filename: No such file or directory
   成功：213 [filesize]
   返回[filesize]是十进制数字，表示该文件在大小，字节为单位

6. 下载文件
   下载文件前，先发送PASV命令，进入被动模式，这样FTP服务器就会开放一个新的端口，用于接收文件数据。
   客户端成功连接到这个数据端口后，再发送RETR命令请求下载文件，这时文件数据就会从新的端口发送过来，文件传输完毕，服务器自动关闭数据端口。
   发送：PASV
   接收：227 Entering Passive Mode (145,24,145,107,207,235).
   后面括号内的5个数字，分别表示IP地址和端口号，端口号分为高8位和低8位，高8位在前
   这里的例子表示IP地址为145.24.145.107，端口号为53227(计算公式：207 * 256 + 235)。

   发送：RETR /path/filename
   接收：150 Opening BINARY mode data connection for /path/filename (54 bytes)
   >>>从数据端口接收文件数据
   接收：226 Transfer complete
 

7. 上传文件

  上传文件与下载文件类似，也是发送PASV命令，获取到数据端口，然后发送STOR命令请求上传文件。
  不同的是上传文件是往这个数据端口写文件数据，写完数据后，客户端主动断开与数据端口的TCP连接，服务器会返回一条上传成功的状态。
  发送：PASV
  接收：227 Entering Passive Mode (145,24,145,107,207,235).
  发送：STOR /path/filename
  接收：150 Opening BINARY mode data connection for /path/filename
   >>>往数据端口写数据
  接收：226 Transfer complete
————————————————
版权声明：本文为CSDN博主「星沉地动」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq446252221/article/details/89383888



### FTP命令字

|  命令   | 描述  |
|  :----  | :----  |
| ABOR					|		中断数据连接程序
| ACCT \<account>		|		系统特权帐号
| ALLO \<bytes>			|		为服务器上的文件存储器分配字节
| APPE \<filename>		|			添加文件到服务器同名文件
| CDUP \<dir path>		|			改变服务器上的父目录
| CWD \<dir path>		|			改变服务器上的工作目录
| DELE \<filename>		|			删除服务器上的指定文件
| HELP \<command>		|			返回指定命令信息
| LIST \<name>			|			如果是文件名列出文件信息，如果是目录则列出文件列表
| MODE \<mode>			|			传输模式（S=流模式，B=块模式，C=压缩模式）
| MKD \<directory>		|			在服务器上建立指定目录
| NLST \<directory>		|		列出指定目录内容
| NOOP					|		无动作，除了来自服务器上的承认
| PASS \<password>		|			系统登录密码
| PASV					|		请求服务器等待数据连接
| PORT \<address>		|			IP 地址和两字节的端口 ID
| PWD					|			显示当前工作目录
| QUIT					|		从 FTP 服务器上退出登录
| REIN					|		重新初始化登录状态连接
| REST \<offset>			|		由特定偏移量重启文件传递
| RETR \<filename>		|			从服务器上找回（复制）文件
| RMD  \<directory>		|			在服务器上删除指定目录
| RNFR \<old path>		|			对旧路径重命名
| RNTO \<new path>		|			对新路径重命名
| SITE \<params>			|		由服务器提供的站点特殊参数
| SMNT \<pathname>		|			挂载指定文件结构
| STAT \<directory>		|		在当前程序或目录上返回信息
| STOR \<filename>		|			储存（复制）文件到服务器上
| STOU \<filename>		|			储存文件到服务器名称上
| STRU \<type>			|			数据结构（F=文件，R=记录，P=页面）
| SYST				|		返回服务器使用的操作系统
| TYPE \<data type>		|		数据类型（A=ASCII，E=EBCDIC，I=binary）
| USER \<username>		|			系统登录的用户名


### FTP命令字
|  响应代码   | 解释说明  |
|  :----  | :----  |
| 110	| 新文件指示器上的重启标记
| 120	| 服务器准备就绪的时间（分钟数）
| 125	| 打开数据连接，开始传输
| 150	| 打开连接
| 200	| 成功
| 202	| 命令没有执行
| 211	| 系统状态回复
| 212	| 目录状态回复
| 213	| 文件状态回复
| 214	| 帮助信息回复
| 215	| 系统类型回复
| 220	| 服务就绪
| 221	| 退出网络
| 225	| 打开数据连接
| 226	| 结束数据连接
| 227	| 进入被动模式（IP 地址、ID 端口）
| 230	| 登录因特网
| 250	| 文件行为完成
| 257	| 路径名建立
| 331	| 要求密码
| 332	| 要求帐号
| 350	| 文件行为暂停
| 421	| 服务关闭
| 425	| 无法打开数据连接
| 426	| 结束连接
| 450	| 文件不可用
| 451	| 遇到本地错误
| 452	| 磁盘空间不足
| 500	| 无效命令
| 501	| 错误参数
| 502	| 命令没有执行
| 503	| 错误指令序列
| 504	| 无效命令参数
| 530	| 未登录网络
| 532	| 存储文件需要帐号
| 550	| 文件不可用
| 551	| 不知道的页类型
| 552	| 超过存储分配
| 553	| 文件名不允许