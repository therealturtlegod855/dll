#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winrt/base.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.Notifications.h>
#include <roapi.h>

using namespace winrt;
using namespace winrt::Windows::Data::Xml::Dom;
using namespace winrt::Windows::UI::Notifications;

#pragma comment(lib, "windowsapp.lib")
#pragma comment(lib, "runtimeobject.lib")

extern "C" {
    __declspec(dllexport) BOOL WINAPI ShowToast(const char* title, const char* body) {
        try {
            init_apartment();
            
            auto doc = XmlDocument();
            hstring xml_content = L"<toast><visual><binding template=\"ToastGeneric\">"
                                L"<text fill=\"{textPrimary}\">" + to_hstring(title) + L"</text>"
                                L"<text fill=\"{textSecondary}\">" + to_hstring(body) + L"</text>"
                                L"</binding></visual>"
                                L"<actions><action content=\"OK\" arguments=\"action=ok\"/></actions></toast>";
            
            doc.LoadXml(xml_content);
            ToastNotification toast(doc);
            auto notifier = ToastNotificationManager::CreateToastNotifier();
            notifier.Show(toast);
            return TRUE;
        } catch (...) {
            return FALSE;
        }
    }
    
    __declspec(dllexport) BOOL WINAPI InitToast() {
        return SUCCEEDED(RoInitialize(RO_INIT_MULTITHREADED));
    }
}
