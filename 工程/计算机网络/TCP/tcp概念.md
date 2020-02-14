## 连接
三次握手：

```sql
        **LISTENING**
        A --> SYN=1 B
SYN_SENT
        A SYN=1 ACK-1 <-- B
                            SYN_RCVD
        A --> ACK=1 B

        **ESTABLISHED**
```
四次挥手：
```sql
        **ESTABLISHED**
        A --> FIN=1 B
FIN_WAIT_1
        A ACK=1 <-- B
FIN_WAIT_2                  CLOSE_WAIT
        A FIN=1 ACK=1  <-- B
TIME_WAIT                   LAST_ACK
        A --> ACK=1 B
        **CLOSED**
```