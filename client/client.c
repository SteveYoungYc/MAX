#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

#define SERVER_PORT 8888
#define POS_NUM 100

/* socket
 * connect
 * send/recv
 */

typedef struct {
    float x;
    float y;
} Position;

void generate_sample_data(Position* position, int time_step) {
    float t = time_step * 1;  // 时间步长
    float scale = 2;          // 控制轨迹的尺度

    position->x = t * scale;
    position->y = t * scale;
}


int main(int argc, char** argv) {
    int iSocketClient;
    struct sockaddr_in tSocketServerAddr;

    int i;
    unsigned char *ucSendBuf;
    int iSendLen;
    int iAddrLen;

    Position samples[POS_NUM];

    for (int i = 0; i < POS_NUM; i++) {
        generate_sample_data(&samples[i], i);
    }

    // 打印样例数据
    // for (int i = 0; i < POS_NUM; i++) {
    //     printf("Sample %d: Position = (%f, %f)\n", i+1, samples[i].x, samples[i].y);
    // }

    // while (1) {
    //     i = (i + 1) % POS_NUM;
    //     ucSendBuf = (unsigned char *)(samples + i);
    //     printf("x: %f, y: %f, sizeof(): %lu\n", *(float*)ucSendBuf, *(float*)(ucSendBuf + sizeof(float)), sizeof(Position));
    //     for (int i = 0; i < sizeof(Position); i++) {
    //         printf("%d ", ucSendBuf[i]);
    //     }
    //     printf("\n");
    //     usleep(100000);
    // }

    if (argc != 2) {
        printf("Usage:\n");
        printf("%s <server_ip>\n", argv[0]);
        return -1;
    }

    iSocketClient = socket(AF_INET, SOCK_DGRAM, 0);

    tSocketServerAddr.sin_family = AF_INET;
    tSocketServerAddr.sin_port = htons(SERVER_PORT); /* host to net, short */
    // tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    if (0 == inet_aton(argv[1], &tSocketServerAddr.sin_addr)) {
        printf("invalid server_ip\n");
        return -1;
    }
    memset(tSocketServerAddr.sin_zero, 0, 8);

    while (1) {
        i = (i + 1) % POS_NUM;
        ucSendBuf = (char *)(samples + i);
        for (int i = 0; i < sizeof(Position); i++) {
            ucSendBuf[i]++;
        }
        iAddrLen = sizeof(struct sockaddr);
        iSendLen = sendto(iSocketClient, ucSendBuf, sizeof(Position), 0,
                          (const struct sockaddr*)&tSocketServerAddr, iAddrLen);

        // printf("x: %f, y: %f\n", *(float*)ucSendBuf, *(float*)(ucSendBuf + sizeof(float)));
        // for (int i = 0; i < sizeof(Position); i++) {
        //     printf("%d ", ucSendBuf[i]);
        // }
        // printf("\n");
        if (iSendLen <= 0) {
            close(iSocketClient);
            return -1;
        }
        for (int i = 0; i < sizeof(Position); i++) {
            ucSendBuf[i]--;
        }
        usleep(1000000);
    }

    return 0;
}
