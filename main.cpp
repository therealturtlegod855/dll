#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winrt/base.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.Notifications.h>
#include <roapi.h>

using namespace winrt;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::UI::Notifications;

#pragma comment(lib, "windowsapp.lib")

// Callback for toast activation (buttons clicked)
void CALLBACK ToastActivated(const char* args) {
    // MinGW app can poll this or use shared memory/WM_COPYDATA for results
    OutputDebugStringA(args);  // Log button args for debugging
}

extern "C" {
    __declspec(dllexport) BOOL ShowToast(const char* title, const char* body, const char* button1, const char* button1Arg) {
        init_apartment();
        
        XmlDocument doc;
        doc.LoadXml(LR"(<toast>
            <visual>
                <binding template="ToastGeneric">
                    <text>" + std::wstring(title, title + strlen(title)) + L"</text>
                    <text>" + std::wstring(body, body + strlen(body)) + L"</text>
                </binding>
            </visual>
            <actions>
                <action content="OK" arguments="action=ok"/>
                <action content=")" + std::wstring(button1, button1 + strlen(button1)) + L"\" arguments=\"" + std::wstring(button1Arg, button1Arg + strlen(button1Arg)) + L"\"/>
            </actions>
        </toast>)");
        
        ToastNotification toast{doc};
        auto notifier = ToastNotificationManager::CreateToastNotifier();
        notifier.Show(toast);
        return TRUE;
    }
    
    __declspec(dllexport) BOOL InitToast() {
        return SUCCEEDED(RoInitialize(RO_INIT_MULTITHREADED));
    }
}
