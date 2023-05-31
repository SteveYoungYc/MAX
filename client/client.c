#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

/* socket
 * connect
 * send/recv
 */

#define SERVER_PORT 8888

int main(int argc, char** argv) {
    int iSocketClient;
    struct sockaddr_in tSocketServerAddr;

    int i;
    int iRet;
    unsigned char ucSendBuf[1000];
    int iSendLen;
    int iAddrLen;

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
        i = (i + 1) % 10;
        for (int j = 0; j < 10; j++) {
            ucSendBuf[j] = i + '0';
        }

        iAddrLen = sizeof(struct sockaddr);
        iSendLen = sendto(iSocketClient, ucSendBuf, strlen(ucSendBuf), 0,
                          (const struct sockaddr*)&tSocketServerAddr, iAddrLen);

        if (iSendLen <= 0) {
            close(iSocketClient);
            return -1;
        }

        sleep(1);
    }

    return 0;
}
