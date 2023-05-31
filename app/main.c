#include <disp_manager.h>
#include <font_manager.h>
#include <input_manager.h>
#include <page_manager.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // 子进程执行客户端代码
        execlp("./client", "client", "127.0.0.1", NULL);
        fprintf(stderr, "Exec failed\n");
        return 1;
    } else {
        int error;

        /* 初始化显示系统 */
        DisplayInit();
        SelectDefaultDisplay("fb");
        InitDefaultDisplay();

        /* 初始化输入系统 */
        InputManagerInit();

        /* 初始化文字系统 */
        FontsRegister();

        error = SelectAndInitFont("default_font", NULL);
        if (error) {
            printf("SelectAndInitFont err\n");
            return -1;
        }
        PagesRegister();
        Page("main")->Run(NULL);
        InputManagerExit();
        return 0;
    }
}
