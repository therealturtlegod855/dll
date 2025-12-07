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
        init_apartment();
        
        auto doc = XmlDocument();
        std::wstring xml = L"<toast>"
            L"<visual>"
            L"  <binding template=\"ToastGeneric\">"
            L"    <text>" + winrt::to_hstring(title) + L"</text>"
            L"    <text>" + winrt::to_hstring(body) + L"</text>"
            L"  </binding>"
            L"</visual>"
            L"<actions>"
            L"  <action content=\"OK\" arguments=\"action=ok\"/>"
            L"</actions>"
            L"</toast>";
        
        doc.LoadXml(xml);
        ToastNotification toast(doc);
        auto notifier = ToastNotificationManager::CreateToastNotifier();
        notifier.Show(toast);
        return TRUE;
    }
    
    __declspec(dllexport) BOOL WINAPI InitToast() {
        return SUCCEEDED(RoInitialize(RO_INIT_MULTITHREADED));
    }
}
