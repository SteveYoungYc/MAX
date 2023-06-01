#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

typedef struct PageAction {
    char* name;
    void (*Init)(void);
    void (*Exit)(void);
    void (*Run)(void* pParams);
    struct PageAction* ptNext;
} PageAction, *PPageAction;

void PageRegister(PPageAction ptPageAction);
void PagesRegister(void);
PPageAction Page(char* name);

#endif
