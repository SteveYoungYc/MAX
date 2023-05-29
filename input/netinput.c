#include <arpa/inet.h>
#include <input_manager.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

/* socket
 * bind
 * sendto/recvfrom
 */

#define SERVER_PORT 8888

static int g_iSocketServer;
char ucRecvBuf[INPUT_BUFF_SIZE];

static int NetinputGetInputEvent(PInputEvent ptInputEvent) {
    struct sockaddr_in tSocketClientAddr;
    int iRecvLen;
    unsigned int iAddrLen = sizeof(struct sockaddr);

    iRecvLen = recvfrom(g_iSocketServer, ucRecvBuf, INPUT_BUFF_SIZE - 1, 0, (struct sockaddr*)&tSocketClientAddr, &iAddrLen);
    if (iRecvLen > 0) {
        ucRecvBuf[iRecvLen] = '\0';
        ptInputEvent->iType = INPUT_TYPE_NET;
        gettimeofday(&ptInputEvent->tTime, NULL);
        strncpy(ptInputEvent->data.net.str, ucRecvBuf, INPUT_BUFF_SIZE);
        ptInputEvent->data.net.str[INPUT_BUFF_SIZE - 1] = '\0';
        return 0;
    }
    return -1;
}

static int NetinputDeviceInit(void) {
    struct sockaddr_in tSocketServerAddr;
    int iRet;

    g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == g_iSocketServer) {
        printf("socket error!\n");
        return -1;
    }

    tSocketServerAddr.sin_family = AF_INET;
    tSocketServerAddr.sin_port = htons(SERVER_PORT); /* host to net, short */
    tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(tSocketServerAddr.sin_zero, 0, 8);

    iRet = bind(g_iSocketServer, (const struct sockaddr*)&tSocketServerAddr, sizeof(struct sockaddr));
    if (-1 == iRet) {
        printf("bind error!\n");
        return -1;
    }

    return 0;
}

static int NetinputDeviceExit(void) {
    close(g_iSocketServer);
    return 0;
}

static InputOpr g_tNetinputDev = {
    .name = "network",
    .GetInputEvent = NetinputGetInputEvent,
    .DeviceInit = NetinputDeviceInit,
    .DeviceExit = NetinputDeviceExit,
};

void NetinputRegister() {
    RegisterInput(&g_tNetinputDev);
}
