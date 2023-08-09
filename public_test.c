#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define SERVER_CMD_PORT 21
#define SERVER_DAT_PORT 20
#define SERVER_IP   "127.0.0.1"

int main(int argc, char **argv)
{
	printf("ftp client test\n");
	int s_fd = -1;
    uint8_t buffer[1024];
    int recode = 0;
    char msg[256];
	
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(s_fd < 0)
    {
        perror("socket");
        return -1;
    }
    // fcntl(s_fd, F_SETFL, O_NONBLOCK);                //将socket设为非阻塞

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_CMD_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);

    if(connect(s_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("connect");
        return -1;
    }
    
    if (recv(s_fd, buffer, sizeof(buffer), 0) < 0) {
        perror("Read failed");
        return -1;
    }
    printf("buffer:%s\n", buffer);
    
    // 用户登陆
    // 发送：USER anonymous
    // 接收：331 Anonymous login ok, send your complete email address as your password
    // 发送：PASS anonymous
    // 接收：230 Anonymous access granted, restrictions apply

    strcpy(buffer, "USER lxj\r\n");
    printf("buffer[%ld]:%s\n", strlen(buffer), buffer);
    send(s_fd, buffer, strlen(buffer), 0);

    if (recv(s_fd, buffer, sizeof(buffer), 0) < 0) {
        perror("Read failed");
        return -1;
    }
    printf("buffer:%s\n", buffer);


    strcpy(buffer, "PASS ..\r\n");
    send(s_fd, buffer, strlen(buffer), 0);

    if (recv(s_fd, buffer, sizeof(buffer), 0) < 0) {
        perror("Read failed");
        return -1;
    }
    printf("buffer:%s\n", buffer);

    strcpy(buffer, "PASV \r\n");
    send(s_fd, buffer, strlen(buffer), 0);

    if (recv(s_fd, buffer, sizeof(buffer), 0) < 0) {
        perror("Read failed");
        return -1;
    }
    printf("buffer:%s\n", buffer);
    // 提取数据端口的IP和端口号
    char *p = strstr(buffer, "(");
    uint8_t data_ip[4];
    uint8_t data_port[2];

    sscanf(p, "(%d,%d,%d,%d,%d,%d", &data_ip[0], &data_ip[1], &data_ip[2], &data_ip[3], &data_port[0], &data_port[1]);

    printf("date info:[%d.%d.%d.%d %d]\n", data_ip[0], data_ip[1], data_ip[2], data_ip[3], data_port[0]<<8|data_port[1]);

    int dat_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(dat_fd < 0)
    {
        perror("socket");
        return -1;
    }
    // fcntl(s_fd, F_SETFL, O_NONBLOCK);                //将socket设为非阻塞

    struct sockaddr_in dat_address;
    dat_address.sin_family = AF_INET;
    dat_address.sin_port = htons(data_port[0]*256 + data_port[1]);
    dat_address.sin_addr.s_addr = inet_addr(SERVER_IP);

    if(connect(dat_fd, (struct sockaddr *)&dat_address, sizeof(dat_address)) < 0)
    {
        perror("connect");
        return -1;
    }
    
    // if (recv(dat_fd, buffer, sizeof(buffer), 0) < 0) {
    //     perror("Read failed");
    //     return -1;
    // }
    // printf("buffer:%s\n", buffer);

    strcpy(buffer, "LIST ./\r\n");
    send(s_fd, buffer, strlen(buffer), 0);

    if (recv(dat_fd, buffer, sizeof(buffer), 0) < 0) {
        perror("Read failed");
        return -1;
    }
    printf("buffer:%s\n", buffer);

    strcpy(buffer, "QUIT \r\n");
    send(s_fd, buffer, strlen(buffer), 0);

    if (recv(s_fd, buffer, sizeof(buffer), 0) < 0) {
        perror("Read failed");
        return -1;
    }
    printf("buffer:%s\n", buffer);

    close(s_fd);

	return 0;
}
