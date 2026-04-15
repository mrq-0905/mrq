/*===============================================
 * 文件名称：client.c
 * 描    述：TCP客户端 - 用于连接刚才的服务器
 ================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // 服务器IP地址
#define SERVER_PORT 8080      // 服务器端口号

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char send_msg[1024];

    // 1. 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket 创建错误 \n");
        return -1;
    }

    // 2. 设置服务器地址结构
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // 3. 将 IP 地址转换为二进制形式
    if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("\n 无效的地址 / 地址不支持 \n");
        return -1;
    }

    // 4. 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n 连接服务器失败 (请确认服务器已启动) \n");
        return -1;
    }

    printf("已连接到服务器！(输入 'exit' 退出客户端)\n");

    // 5. 通讯循环
    while(1) {
        printf("请输入发送给服务器的消息: ");
        
        // 读取用户输入
        fgets(send_msg, sizeof(send_msg), stdin);
        
        // 去掉输入末尾的换行符
        send_msg[strcspn(send_msg, "\n")] = 0;

        // 检查是否退出
        if (strcmp(send_msg, "exit") == 0) {
            printf("正在断开连接...\n");
            break;
        }

        // 发送数据
        send(sock, send_msg, strlen(send_msg), 0);

        // 接收服务器回复
        int valread = read(sock, buffer, 1024);
        if (valread > 0) {
            printf("收到服务器回复: %s\n", buffer);
            bzero(buffer, 1024); // 清空缓冲区
        }
    }

    // 6. 关闭连接
    close(sock);
    return 0;
}
