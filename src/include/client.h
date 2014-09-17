
int communication_createSocket(const char* hostnm, int portnm);
int communication_closeSocket(void);
int communication_sendRecv(char* sendBuf, int sendBufSize, char* recvBuf, int recvBufSize);

