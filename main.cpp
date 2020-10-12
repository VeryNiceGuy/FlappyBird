
#include "AppDelegate.h"
#include "cocos2d.h"

#define WIN32_LEAN_AND_MEAN    
#include <windows.h>
#include <tchar.h>
#include "platform/CCStdC.h"

USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
