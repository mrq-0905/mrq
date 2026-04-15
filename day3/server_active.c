/*===============================================
 * 文件名称：server_active.c
 * 描    述：支持服务器端主动发送消息的 TCP 服务器 (多线程版)
 ================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h> // 必须：用于多线程

int client_socket_global = -1; // 全局变量：保存客户端套接字，方便发送线程使用

// --- 线程函数：负责接收客户端消息 ---
void* receive_thread(void* arg) {
    char buffer[1024] = {0};
    int sock = *(int*)arg;
    
    while(1) {
        bzero(buffer, 1024);
        int valread = read(sock, buffer, 1023);
        
        if (valread <= 0) {
            printf("\n[系统] 客户端断开连接\n");
            client_socket_global = -1; // 标记客户端已断开
            break;
        }
        
        printf("\n[客户端]: %s", buffer);
        printf("[我]: "); // 提示用户可以输入了
        fflush(stdout);  // 强制刷新输出缓冲区，确保提示语立即显示
    }
    return NULL;
}

// --- 线程函数：负责主动发送消息 ---
void* send_thread(void* arg) {
    char send_msg[1024];
    
    printf("[提示] 服务器已就绪，你可以直接输入文字并回车发送给客户端。\n");
    printf("[我]: ");
    fflush(stdout);

    while(1) {
        // 获取用户键盘输入
        if (fgets(send_msg, sizeof(send_msg), stdin) != NULL) {
            // 如果客户端没连接，提示一下
            if (client_socket_global == -1) {
                printf("[系统] 当前没有客户端连接，无法发送。\n");
                printf("[我]: ");
                continue;
            }

            // 发送数据
            // 注意：这里我们直接发送用户输入的字符串
            write(client_socket_global, send_msg, strlen(send_msg));
            
            // 重新打印提示符，保持界面整洁
            printf("[我]: ");
            fflush(stdout);
        }
    }
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t recv_tid, send_tid; // 线程ID

    // --- 1. 创建套接字 ---
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket failed");
        return -1;
    }

    // --- 2. 绑定 ---
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if(bind(server_fd, (struct sockaddr *)&address, addrlen) < 0){
        perror("Bind failed");
        return -1;
    }

    // --- 3. 监听 ---
    if(listen(server_fd, 3) < 0){
        perror("Listen failed");
        return -1;
    }
    printf("服务器启动，等待连接...\n");

    // --- 4. 接受连接 ---
    client_socket_global = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if(client_socket_global < 0) {
        perror("Accept failed");
        return -1;
    }
    
    printf("连接成功！IP: %s\n", inet_ntoa(address.sin_addr));

    // --- 5. 启动多线程 ---
    // 创建接收线程
    if(pthread_create(&recv_tid, NULL, receive_thread, (void*)&client_socket_global) != 0) {
        perror("Failed to create receive thread");
        return -1;
    }

    // 创建发送线程
    if(pthread_create(&send_tid, NULL, send_thread, NULL) != 0) {
        perror("Failed to create send thread");
        return -1;
    }

    // --- 6. 等待线程结束 (死循环保持主程序运行) ---
    pthread_join(recv_tid, NULL);
    pthread_join(send_tid, NULL);

    close(client_socket_global);
    close(server_fd);
    return 0;
}
