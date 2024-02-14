#pragma once

#define MAIN_BOOL
#ifdef MAIN_BOOL
    #define bool int
    enum {false, true};
#endif //MAIN_BOOL

#define MAIN_DEBUG
#ifdef MAIN_DEBUG
    #include <stdio.h>
    #define LOG fprintf(stderr, "LOG %s %s %s %d %s()\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__);
    #define LOGS(msg) fprintf(stderr, "LOG %s %s %s %d %s() : %s\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, msg);
    #define LOGSN(msg, num) fprintf(stderr, "LOG %s %s %s %d %s() : %s%d\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, msg, num);
    #define LOGIF(flag) if (flag) fprintf(stderr, "LOG %s %s %s %d %s()\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__);
    #define LOGSIF(flag, msg) if (flag) fprintf(stderr, "LOG %s %s %s %d %s() : %s\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, msg);
    #define LOGSNIF(flag, msg, num) if (flag) fprintf(stderr, "LOG %s %s %s %d %s() : %s%d\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, msg, num);
    #define LOGIFN(flag) if (!flag) fprintf(stderr, "LOG %s %s %s %d %s()\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__);
    #define LOGSIFN(flag, msg) if (!flag) fprintf(stderr, "LOG %s %s %s %d %s() : %s\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, msg);
    #define LOGSNIFN(flag, msg, num) if (!flag) fprintf(stderr, "LOG %s %s %s %d %s() : %s%d\n", __DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, msg, num);
#else
    #define LOG
    #define LOGS(msg)
    #define LOGSN(msg, num)
    #define LOGIF(flag)
    #define LOGSIF(flag, msg)
    #define LOGSNIF(flag, msg, num)
    #define LOGIFN(flag)
    #define LOGSIFN(flag, msg)
    #define LOGSNIFN(flag, msg, num)
#endif //MAIN_DEBUG

