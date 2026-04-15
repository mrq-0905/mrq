/*===============================================
 * 文件名称：server.c
 * 描    述：服务器端 - 收到消息后会在服务器终端打印出来
 ================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0}; // 用于存放收到的消息
    char *response = "消息已收到";  // 回复给客户端的话

    // 1. 创建套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. 绑定地址
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. 监听
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("服务器启动，等待连接...\n");

    // 4. 接受连接
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("客户端已连接！\n");

    // 5. 通讯循环
    while (1) {
        // 清空缓冲区，防止乱码
        memset(buffer, 0, BUFFER_SIZE);

        // --- 接收数据 ---
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        
        // 如果客户端断开连接
        if (valread <= 0) {
            printf("客户端断开连接\n");
            break;
        }

        // --- 核心功能：在服务器终端打印收到的消息 ---
        // 这里就是你要求的“显示客户端发送的消息”
        printf(">>> 客户端发来了: %s\n", buffer); 

        // --- 回复数据 (可选) ---
        send(new_socket, response, strlen(response), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
