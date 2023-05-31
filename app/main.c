#include <disp_manager.h>
#include <font_manager.h>
#include <input_manager.h>
#include <page_manager.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process executes the client code
        execlp("./client", "client", "127.0.0.1", NULL);
        fprintf(stderr, "Exec failed\n");
        return 1;
    } else {
        int error;

        /* Initializing display system */
        DisplayInit();
        SelectDefaultDisplay("fb");
        InitDefaultDisplay();

        /* Initializing input system */
        InputManagerInit();

        /* Initializing font system */
        FontsRegister();

        error = SelectAndInitFont("default_font", NULL);
        if (error) {
            printf("SelectAndInitFont err\n");
            return -1;
        }
        PagesRegister();
        Page("main")->Run(NULL);
        InputManagerExit();

        // Sending SIGTERM signal to terminate the child process
        kill(pid, SIGTERM);

        // Waiting for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        return 0;
    }
}
