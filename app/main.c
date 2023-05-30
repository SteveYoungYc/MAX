#include <disp_manager.h>
#include <font_manager.h>
#include <page_manager.h>
#include <input_manager.h>

int main(int argc, char** argv) {
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
