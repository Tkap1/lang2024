#ifdef __clang__
// #define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include <ShlObj.h>

#else

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
	#pragma clang diagnostic ignored "-Wreserved-identifier"
#endif

extern "C"
{

#define FD_SETSIZE      64
#define CONST               const
#define WINBASEAPI DECLSPEC_IMPORT
#define WINAPI      __stdcall
#define CALLBACK    __stdcall
#define VOID void
#define DECLSPEC_IMPORT __declspec(dllimport)
#define DECLSPEC_NORETURN   __declspec(noreturn)
#define WINUSERAPI DECLSPEC_IMPORT
#define APIENTRY    WINAPI
#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#define MB_CANCELTRYCONTINUE        0x00000006L
#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L
#define MB_USERICON                 0x00000080L
#define MB_ICONWARNING              MB_ICONEXCLAMATION
#define MB_ICONERROR                MB_ICONHAND
#define MB_ICONINFORMATION          MB_ICONASTERISK
#define MB_ICONSTOP                 MB_ICONHAND
#define MB_DEFBUTTON1               0x00000000L
#define MB_DEFBUTTON2               0x00000100L
#define MB_DEFBUTTON3               0x00000200L
#define MB_DEFBUTTON4               0x00000300L
#define MB_APPLMODAL                0x00000000L
#define MB_SYSTEMMODAL              0x00001000L
#define MB_TASKMODAL                0x00002000L
#define MB_HELP                     0x00004000L // Help Button
#define MB_NOFOCUS                  0x00008000L
#define MB_SETFOREGROUND            0x00010000L
#define MB_DEFAULT_DESKTOP_ONLY     0x00020000L
#define MB_TOPMOST                  0x00040000L
#define MB_RIGHT                    0x00080000L
#define MB_RTLREADING               0x00100000L
#define CF_TEXT             1
#define HGLOBAL     HANDLE
#define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
#define LANG_NEUTRAL                     0x00
#define SUBLANG_DEFAULT                             0x01    // user default
#define MAX_PATH          260
#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
#define MAKELANGID(p, s)       ((((WORD  )(s)) << 10) | (WORD  )(p))
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_READ                     (0x80000000L)
#define FILE_SHARE_READ                 0x00000001
#define FILE_ATTRIBUTE_NORMAL               0x00000080
#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5
#define STD_INPUT_HANDLE    ((DWORD)-10)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)
#define STD_ERROR_HANDLE    ((DWORD)-12)
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010
#define WINGDIAPI DECLSPEC_IMPORT
#define UNREFERENCED_PARAMETER(P)          (P)
#define MEM_COMMIT                      0x00001000
#define MEM_RESERVE                     0x00002000
#define PAGE_NOACCESS           0x01
#define PAGE_READONLY           0x02
#define PAGE_READWRITE          0x04
#define PAGE_WRITECOPY          0x08
#define PAGE_EXECUTE            0x10
#define PAGE_EXECUTE_READ       0x20
#define PAGE_EXECUTE_READWRITE  0x40
#define PAGE_EXECUTE_WRITECOPY  0x80
#define PAGE_GUARD             0x100
#define PAGE_NOCACHE           0x200
#define PAGE_WRITECOMBINE      0x400
#define MIDL_INTERFACE(x)   struct DECLSPEC_UUID(x) DECLSPEC_NOVTABLE
#define DECLSPEC_UUID(x)    __declspec(uuid(x))
#define DECLARE_INTERFACE_(iface, baseiface)            interface DECLSPEC_NOVTABLE iface : public baseiface
#define STDMETHOD(method)        virtual COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method)  virtual COM_DECLSPEC_NOTHROW type STDMETHODCALLTYPE method
#define DECLSPEC_NOVTABLE   __declspec(novtable)
#define THIS_
#define THIS                    void
#define PURE                    = 0
#define __STRUCT__ struct
#define interface __STRUCT__
#define STDMETHODCALLTYPE       __stdcall
#define BEGIN_INTERFACE
#define END_INTERFACE
#define REFIID const IID &
#define COM_DECLSPEC_NOTHROW DECLSPEC_NOTHROW
#define DECLSPEC_NOTHROW   __declspec(nothrow)
#define PM_NOREMOVE         0x0000
#define PM_REMOVE           0x0001
#define PM_NOYIELD          0x0002
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#define WM_NULL                         0x0000
#define WM_CREATE                       0x0001
#define WM_DESTROY                      0x0002
#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005
#define WM_ACTIVATE                     0x0006
#define     WA_INACTIVE     0
#define     WA_ACTIVE       1
#define     WA_CLICKACTIVE  2
#define WM_SETFOCUS                     0x0007
#define WM_KILLFOCUS                    0x0008
#define WM_ENABLE                       0x000A
#define WM_SETREDRAW                    0x000B
#define WM_SETTEXT                      0x000C
#define WM_GETTEXT                      0x000D
#define WM_GETTEXTLENGTH                0x000E
#define WM_PAINT                        0x000F
#define WM_CLOSE                        0x0010
#define WM_QUIT                         0x0012
#define WM_ERASEBKGND                   0x0014
#define WM_SYSCOLORCHANGE               0x0015
#define WM_SHOWWINDOW                   0x0018
#define WM_WININICHANGE                 0x001A
#define WM_DEVMODECHANGE                0x001B
#define WM_ACTIVATEAPP                  0x001C
#define WM_FONTCHANGE                   0x001D
#define WM_TIMECHANGE                   0x001E
#define WM_CANCELMODE                   0x001F
#define WM_SETCURSOR                    0x0020
#define WM_MOUSEACTIVATE                0x0021
#define WM_CHILDACTIVATE                0x0022
#define WM_QUEUESYNC                    0x0023
#define WM_GETMINMAXINFO                0x0024
#define S_OK                                   ((HRESULT)0L)
#define S_FALSE                                ((HRESULT)1L)
#define CREATE_NEW_CONSOLE                0x00000010
#define JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE          0x00002000
#define WM_CTLCOLORMSGBOX               0x0132
#define WM_CTLCOLOREDIT                 0x0133
#define WM_CTLCOLORLISTBOX              0x0134
#define WM_CTLCOLORBTN                  0x0135
#define WM_CTLCOLORDLG                  0x0136
#define WM_CTLCOLORSCROLLBAR            0x0137
#define WM_CTLCOLORSTATIC               0x0138
#define MN_GETHMENU                     0x01E1
#define WM_MOUSEFIRST                   0x0200
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
#define WM_MOUSEWHEEL                   0x020A
#define WM_XBUTTONDOWN                  0x020B
#define WM_XBUTTONUP                    0x020C
#define WM_XBUTTONDBLCLK                0x020D
#define WM_MOUSEHWHEEL                  0x020E
#define WHEEL_DELTA                     120
#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#define WHEEL_PAGESCROLL                (UINT_MAX)
#define GET_KEYSTATE_WPARAM(wParam)     (LOWORD(wParam))
#define GET_NCHITTEST_WPARAM(wParam)    ((short)LOWORD(wParam))
#define GET_XBUTTON_WPARAM(wParam)      (HIWORD(wParam))
#define XBUTTON1      0x0001
#define XBUTTON2      0x0002
#define WM_PARENTNOTIFY                 0x0210
#define WM_ENTERMENULOOP                0x0211
#define WM_EXITMENULOOP                 0x0212
#define WM_NEXTMENU                     0x0213
#define WM_SIZING                       0x0214
#define WM_CAPTURECHANGED               0x0215
#define WM_MOVING                       0x0216
#define WM_KEYFIRST                     0x0100
#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101
#define WM_CHAR                         0x0102
#define WM_DEADCHAR                     0x0103
#define WM_SYSKEYDOWN                   0x0104
#define WM_SYSKEYUP                     0x0105
#define WM_SYSCHAR                      0x0106
#define WM_SYSDEADCHAR                  0x0107
#define KF_EXTENDED       0x0100
#define KF_DLGMODE        0x0800
#define KF_MENUMODE       0x1000
#define KF_ALTDOWN        0x2000
#define KF_REPEAT         0x4000
#define KF_UP             0x8000
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */
#define VK_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define VK_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
#define VK_BACK           0x08
#define VK_TAB            0x09
#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D
#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14
#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_IME_ON         0x16
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19
#define VK_IME_OFF        0x1A
#define VK_ESCAPE         0x1B
#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F
#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F
#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D
#define VK_SLEEP          0x5F
#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87
#define VK_NAVIGATION_VIEW     0x88 // reserved
#define VK_NAVIGATION_MENU     0x89 // reserved
#define VK_NAVIGATION_UP       0x8A // reserved
#define VK_NAVIGATION_DOWN     0x8B // reserved
#define VK_NAVIGATION_LEFT     0x8C // reserved
#define VK_NAVIGATION_RIGHT    0x8D // reserved
#define VK_NAVIGATION_ACCEPT   0x8E // reserved
#define VK_NAVIGATION_CANCEL   0x8F // reserved
#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91
#define VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad
#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5
#define VK_BROWSER_BACK        0xA6
#define VK_BROWSER_FORWARD     0xA7
#define VK_BROWSER_REFRESH     0xA8
#define VK_BROWSER_STOP        0xA9
#define VK_BROWSER_SEARCH      0xAA
#define VK_BROWSER_FAVORITES   0xAB
#define VK_BROWSER_HOME        0xAC
#define VK_VOLUME_MUTE         0xAD
#define VK_VOLUME_DOWN         0xAE
#define VK_VOLUME_UP           0xAF
#define VK_MEDIA_NEXT_TRACK    0xB0
#define VK_MEDIA_PREV_TRACK    0xB1
#define VK_MEDIA_STOP          0xB2
#define VK_MEDIA_PLAY_PAUSE    0xB3
#define VK_LAUNCH_MAIL         0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1         0xB6
#define VK_LAUNCH_APP2         0xB7
#define VK_OEM_1          0xBA   // ';:' for US
#define VK_OEM_PLUS       0xBB   // '+' any country
#define VK_OEM_COMMA      0xBC   // ',' any country
#define VK_OEM_MINUS      0xBD   // '-' any country
#define VK_OEM_PERIOD     0xBE   // '.' any country
#define VK_OEM_2          0xBF   // '/?' for US
#define VK_OEM_3          0xC0   // '`~' for US
#define VK_GAMEPAD_A                         0xC3 // reserved
#define VK_GAMEPAD_B                         0xC4 // reserved
#define VK_GAMEPAD_X                         0xC5 // reserved
#define VK_GAMEPAD_Y                         0xC6 // reserved
#define VK_GAMEPAD_RIGHT_SHOULDER            0xC7 // reserved
#define VK_GAMEPAD_LEFT_SHOULDER             0xC8 // reserved
#define VK_GAMEPAD_LEFT_TRIGGER              0xC9 // reserved
#define VK_GAMEPAD_RIGHT_TRIGGER             0xCA // reserved
#define VK_GAMEPAD_DPAD_UP                   0xCB // reserved
#define VK_GAMEPAD_DPAD_DOWN                 0xCC // reserved
#define VK_GAMEPAD_DPAD_LEFT                 0xCD // reserved
#define VK_GAMEPAD_DPAD_RIGHT                0xCE // reserved
#define VK_GAMEPAD_MENU                      0xCF // reserved
#define VK_GAMEPAD_VIEW                      0xD0 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON    0xD1 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON   0xD2 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_UP        0xD3 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_DOWN      0xD4 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT     0xD5 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_LEFT      0xD6 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_UP       0xD7 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN     0xD8 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT    0xD9 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT     0xDA // reserved
#define VK_OEM_4          0xDB  //  '[{' for US
#define VK_OEM_5          0xDC  //  '\|' for US
#define VK_OEM_6          0xDD  //  ']}' for US
#define VK_OEM_7          0xDE  //  ''"' for US
#define VK_OEM_8          0xDF
#define VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define VK_ICO_HELP       0xE3  //  Help key on ICO
#define VK_ICO_00         0xE4  //  00 key on ICO
#define VK_PROCESSKEY     0xE5
#define VK_ICO_CLEAR      0xE6
#define VK_PACKET         0xE7
#define VK_OEM_RESET      0xE9
#define VK_OEM_JUMP       0xEA
#define VK_OEM_PA1        0xEB
#define VK_OEM_PA2        0xEC
#define VK_OEM_PA3        0xED
#define VK_OEM_WSCTRL     0xEE
#define VK_OEM_CUSEL      0xEF
#define VK_OEM_ATTN       0xF0
#define VK_OEM_FINISH     0xF1
#define VK_OEM_COPY       0xF2
#define VK_OEM_AUTO       0xF3
#define VK_OEM_ENLW       0xF4
#define VK_OEM_BACKTAB    0xF5
#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE
#define MAPVK_VK_TO_VSC     (0)
#define MAPVK_VSC_TO_VK     (1)
#define MAPVK_VK_TO_CHAR    (2)
#define MAPVK_VSC_TO_VK_EX  (3)
#define CS_VREDRAW          0x0001
#define CS_HREDRAW          0x0002
#define CS_DBLCLKS          0x0008
#define CS_OWNDC            0x0020
#define CS_CLASSDC          0x0040
#define CS_PARENTDC         0x0080
#define CS_NOCLOSE          0x0200
#define CS_SAVEBITS         0x0800
#define CS_BYTEALIGNCLIENT  0x1000
#define CS_BYTEALIGNWINDOW  0x2000
#define CS_GLOBALCLASS      0x4000
#define IDC_ARROW           MAKEINTRESOURCE(32512)
#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_TILED            WS_OVERLAPPED
#define WS_ICONIC           WS_MINIMIZE
#define WS_SIZEBOX          WS_THICKFRAME
#define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
									WS_CAPTION        | \
									WS_SYSMENU        | \
									WS_THICKFRAME     | \
									WS_MINIMIZEBOX    | \
									WS_MAXIMIZEBOX)

#define WS_POPUPWINDOW      (WS_POPUP          | \
									WS_BORDER         | \
									WS_SYSMENU)

#define WS_CHILDWINDOW      (WS_CHILD)
#define MAKEINTRESOURCEA(i) ((LPSTR)((ULONG_PTR)((WORD)(i))))
#define MAKEINTRESOURCEW(i) ((LPWSTR)((ULONG_PTR)((WORD)(i))))
#define CW_USEDEFAULT       ((int)0x80000000)
#define PFD_TYPE_RGBA        0
#define PFD_TYPE_COLORINDEX  1
#define PFD_MAIN_PLANE       0
#define PFD_OVERLAY_PLANE    1
#define PFD_UNDERLAY_PLANE   (-1)
#define PFD_DOUBLEBUFFER            0x00000001
#define PFD_STEREO                  0x00000002
#define PFD_DRAW_TO_WINDOW          0x00000004
#define PFD_DRAW_TO_BITMAP          0x00000008
#define PFD_SUPPORT_GDI             0x00000010
#define PFD_SUPPORT_OPENGL          0x00000020
#define PFD_GENERIC_FORMAT          0x00000040
#define PFD_NEED_PALETTE            0x00000080
#define PFD_NEED_SYSTEM_PALETTE     0x00000100
#define PFD_SWAP_EXCHANGE           0x00000200
#define PFD_SWAP_COPY               0x00000400
#define PFD_SWAP_LAYER_BUFFERS      0x00000800
#define PFD_GENERIC_ACCELERATED     0x00001000
#define PFD_SUPPORT_DIRECTDRAW      0x00002000
#define PFD_DIRECT3D_ACCELERATED    0x00004000
#define PFD_SUPPORT_COMPOSITION     0x00008000
#define PFD_DEPTH_DONTCARE          0x20000000
#define PFD_DOUBLEBUFFER_DONTCARE   0x40000000
#define PFD_STEREO_DONTCARE         0x80000000
#define MONITOR_DEFAULTTONULL       0x00000000
#define MONITOR_DEFAULTTOPRIMARY    0x00000001
#define MONITOR_DEFAULTTONEAREST    0x00000002
#define FILE_APPEND_DATA 4
#define ERROR               0
#define NULLREGION          1
#define SIMPLEREGION        2
#define COMPLEXREGION       3
#define RGN_ERROR ERROR
#define RGN_AND             1
#define RGN_OR              2
#define RGN_XOR             3
#define RGN_DIFF            4
#define RGN_COPY            5
#define RGN_MIN             RGN_AND
#define RGN_MAX             RGN_COPY
#define GW_HWNDFIRST        0
#define GW_HWNDLAST         1
#define GW_HWNDNEXT         2
#define GW_HWNDPREV         3
#define GW_OWNER            4
#define GW_CHILD            5
#define GetNextWindow(hWnd, wCmd) GetWindow(hWnd, wCmd)
#define WS_EX_TOPMOST           0x00000008L
#define WS_EX_LAYERED           0x00080000
#define GWL_EXSTYLE         (-20)
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define LWA_ALPHA             0x00000002
#define SWP_NOSIZE          0x0001
#define SWP_NOMOVE          0x0002
#define SWP_NOZORDER        0x0004
#define SWP_NOREDRAW        0x0008
#define SWP_NOACTIVATE      0x0010
#define SWP_FRAMECHANGED    0x0020  /* The frame changed: send WM_NCCALCSIZE */
#define SWP_SHOWWINDOW      0x0040
#define SWP_HIDEWINDOW      0x0080
#define SWP_NOCOPYBITS      0x0100
#define SWP_NOOWNERZORDER   0x0200  /* Don't do owner Z ordering */
#define SWP_NOSENDCHANGING  0x0400  /* Don't send WM_WINDOWPOSCHANGING */
#define SWP_DRAWFRAME       SWP_FRAMECHANGED
#define SWP_NOREPOSITION    SWP_NOOWNERZORDER
#define HWND_TOP        ((HWND)0)
#define HWND_BOTTOM     ((HWND)1)
#define HWND_TOPMOST    ((HWND)-1)
#define HWND_NOTOPMOST  ((HWND)-2)
#define IDI_APPLICATION     MAKEINTRESOURCE(32512)
#define IDI_HAND            MAKEINTRESOURCE(32513)
#define IDI_QUESTION        MAKEINTRESOURCE(32514)
#define IDI_EXCLAMATION     MAKEINTRESOURCE(32515)
#define IDI_ASTERISK        MAKEINTRESOURCE(32516)
#define IMAGE_BITMAP        0
#define IMAGE_ICON          1
#define IMAGE_CURSOR        2
#define CSIDL_INTERNET_CACHE            0x0020
#define CSIDL_COOKIES                   0x0021
#define CSIDL_HISTORY                   0x0022
#define CSIDL_COMMON_APPDATA            0x0023        // All Users\Application Data
#define CSIDL_WINDOWS                   0x0024        // GetWindowsDirectory()
#define CSIDL_SYSTEM                    0x0025        // GetSystemDirectory()
#define CSIDL_PROGRAM_FILES             0x0026        // C:\Program Files
#define CSIDL_MYPICTURES                0x0027        // C:\Program Files\My Pictures
#define EXTERN_C       extern "C"
#define DECLSPEC_IMPORT __declspec(dllimport)
#define SHFOLDERAPI           EXTERN_C DECLSPEC_IMPORT HRESULT STDAPICALLTYPE
#define STDAPICALLTYPE          __stdcall
#define STDAPI                  EXTERN_C HRESULT STDAPICALLTYPE
#define REFKNOWNFOLDERID const KNOWNFOLDERID &
#define KF_FLAG_DEFAULT        0x00000000
#define WINOLEAPI        EXTERN_C DECLSPEC_IMPORT HRESULT STDAPICALLTYPE
#define WINOLEAPI_(type) EXTERN_C DECLSPEC_IMPORT type STDAPICALLTYPE
#define CP_UTF8                   65001       // UTF-8 translation

#define STANDARD_RIGHTS_REQUIRED         (0x000F0000L)
#define SYNCHRONIZE                      (0x00100000L)
#define SEMAPHORE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0x3)
#define INFINITE            0xFFFFFFFF  // Infinite timeout

#ifdef __clang__
#define _Null_terminated_
#endif

#define DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
EXTERN_C const GUID name


#ifndef FALSE
	#define FALSE               0
#endif

#ifndef TRUE
	#define TRUE                1
#endif

#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.
#define COMMON_LVB_LEADING_BYTE    0x0100 // Leading Byte of DBCS
#define COMMON_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
#define COMMON_LVB_GRID_HORIZONTAL 0x0400 // DBCS: Grid attribute: top horizontal.
#define COMMON_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
#define COMMON_LVB_GRID_RVERTICAL  0x1000 // DBCS: Grid attribute: right vertical.
#define COMMON_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
#define COMMON_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.
#define COMMON_LVB_SBCSDBCS        0x0300 // SBCS or DBCS flag.

#define STATUS_WAIT_0                           ((DWORD   )0x00000000L)
#define WAIT_OBJECT_0       ((STATUS_WAIT_0 ) + 0 )


typedef enum _JOBOBJECTINFOCLASS {
	JobObjectBasicAccountingInformation = 1,
	JobObjectBasicLimitInformation,
	JobObjectBasicProcessIdList,
	JobObjectBasicUIRestrictions,
	JobObjectSecurityLimitInformation,  // deprecated
	JobObjectEndOfJobTimeInformation,
	JobObjectAssociateCompletionPortInformation,
	JobObjectBasicAndIoAccountingInformation,
	JobObjectExtendedLimitInformation,
	JobObjectJobSetInformation,
	JobObjectGroupInformation,
	JobObjectNotificationLimitInformation,
	JobObjectLimitViolationInformation,
	JobObjectGroupInformationEx,
	JobObjectCpuRateControlInformation,
	JobObjectCompletionFilter,
	JobObjectCompletionCounter,
	JobObjectReserved1Information = 18,
	JobObjectReserved2Information,
	JobObjectReserved3Information,
	JobObjectReserved4Information,
	JobObjectReserved5Information,
	JobObjectReserved6Information,
	JobObjectReserved7Information,
	JobObjectReserved8Information,
	JobObjectReserved9Information,
	JobObjectReserved10Information,
	JobObjectReserved11Information,
	JobObjectReserved12Information,
	JobObjectReserved13Information,
	JobObjectReserved14Information = 31,
	JobObjectNetRateControlInformation,
	JobObjectNotificationLimitInformation2,
	JobObjectLimitViolationInformation2,
	JobObjectCreateSilo,
	JobObjectSiloBasicInformation,
	JobObjectReserved15Information = 37,
	JobObjectReserved16Information = 38,
	JobObjectReserved17Information = 39,
	JobObjectReserved18Information = 40,
	JobObjectReserved19Information = 41,
	JobObjectReserved20Information = 42,
	JobObjectReserved21Information = 43,
	JobObjectReserved22Information = 44,
	JobObjectReserved23Information = 45,
	JobObjectReserved24Information = 46,
	JobObjectReserved25Information = 47,
	MaxJobObjectInfoClass
} JOBOBJECTINFOCLASS;

DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HGLRC);          // OpenGL
DECLARE_HANDLE(HICON);
DECLARE_HANDLE(HMENU);
DECLARE_HANDLE(HCURSOR);
DECLARE_HANDLE(HBRUSH);
DECLARE_HANDLE(HMONITOR);
DECLARE_HANDLE(HRGN);


#ifdef UNICODE
	#define MAKEINTRESOURCE  MAKEINTRESOURCEW
	#define LoadIcon  LoadIconW
#else // UNICODE
	#define LoadIcon  LoadIconA
	#define MessageBox  MessageBoxA
	#define FormatMessage  FormatMessageA
	#define FindFirstFile  FindFirstFileA
	#define FindNextFile  FindNextFileA
	#define CreateFile  CreateFileA
	#define PeekMessage  PeekMessageA
	#define DispatchMessage  DispatchMessageA
	#define CreateProcess  CreateProcessA
	#define CreateJobObject  CreateJobObjectA
	#define CopyFile  CopyFileA
	#define MapVirtualKey  MapVirtualKeyA
	#define DefWindowProc  DefWindowProcA
	#define OutputDebugString  OutputDebugStringA
	#define GetModuleHandle  GetModuleHandleA
	#define MAKEINTRESOURCE  MAKEINTRESOURCEA
	#define LoadCursor  LoadCursorA
	#define RegisterClass  RegisterClassA
	#define CreateWindowEx  CreateWindowExA
	#define GetMonitorInfo  GetMonitorInfoA
#endif // NOT UNICODE



typedef char CHAR;
typedef short SHORT;
typedef __int64 LONG64, *PLONG64;
typedef long LONG;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;
typedef unsigned int    u_int;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL	             *PBOOL;
typedef BOOL                *LPBOOL;
typedef BYTE	             *PBYTE;
typedef BYTE                *LPBYTE;
typedef int	              *PINT;
typedef int                 *LPINT;
typedef WORD	             *PWORD;
typedef WORD                *LPWORD;
typedef long                *LPLONG;
typedef DWORD	            *PDWORD;
typedef DWORD               *LPDWORD;
typedef void                *LPVOID;
typedef CONST void          *LPCVOID;
typedef CONST CHAR *LPCSTR, *PCSTR;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;
typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;
typedef INT_PTR (WINAPI *PROC)();
typedef __int64 LONGLONG;
typedef void *HANDLE;
typedef CHAR *NPSTR, *LPSTR, *PSTR;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
typedef WCHAR *NWPSTR, *LPWSTR, *PWSTR;
typedef HANDLE              HLOCAL;
typedef void *PVOID;
typedef HINSTANCE HMODULE;      /* HMODULEs can be used in place of HINSTANCEs */
typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;
typedef ULONG_PTR SIZE_T, *PSIZE_T;
typedef long HRESULT;
typedef INT_PTR (WINAPI *FARPROC)();
typedef unsigned long ULONG;
typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
typedef unsigned __int64 ULONGLONG;
typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef WORD                ATOM;   //BUGBUG - might want to remove this from minwin
typedef void* HGDIOBJ;
typedef DWORD   COLORREF;
typedef CONST WCHAR *LPCWCH, *PCWCH;
typedef CONST CHAR *LPCCH, *PCCH;
typedef DWORD (WINAPI *PTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

typedef CONST WCHAR *LPCWSTR, *PCWSTR;

typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef union _LARGE_INTEGER {
	struct {
		DWORD LowPart;
		LONG HighPart;
	} DUMMYSTRUCTNAME;
	struct {
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER;
typedef LARGE_INTEGER *PLARGE_INTEGER;

typedef struct _WIN32_FIND_DATAA {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	DWORD dwReserved0;
	DWORD dwReserved1;
	CHAR   cFileName[ MAX_PATH ];
	CHAR   cAlternateFileName[ 14 ];
#ifdef _MAC
	DWORD dwFileType;
	DWORD dwCreatorType;
	WORD  wFinderFlags;
#endif
} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;

typedef struct _SECURITY_ATTRIBUTES {
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _OVERLAPPED {
	ULONG_PTR Internal;
	ULONG_PTR InternalHigh;
	union {
		struct {
				DWORD Offset;
				DWORD OffsetHigh;
		} DUMMYSTRUCTNAME;
		PVOID Pointer;
	} DUMMYUNIONNAME;

	HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef struct _PROCESS_INFORMATION {
	HANDLE hProcess;
	HANDLE hThread;
	DWORD dwProcessId;
	DWORD dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;

typedef struct _SYSTEM_INFO {
  union {
    DWORD dwOemId;
    struct {
      WORD wProcessorArchitecture;
      WORD wReserved;
    } DUMMYSTRUCTNAME;
  } DUMMYUNIONNAME;
  DWORD     dwPageSize;
  LPVOID    lpMinimumApplicationAddress;
  LPVOID    lpMaximumApplicationAddress;
  DWORD_PTR dwActiveProcessorMask;
  DWORD     dwNumberOfProcessors;
  DWORD     dwProcessorType;
  DWORD     dwAllocationGranularity;
  WORD      wProcessorLevel;
  WORD      wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;

typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} GUID;
typedef GUID IID;

typedef GUID KNOWNFOLDERID;
DEFINE_KNOWN_FOLDER(FOLDERID_RoamingAppData, 0x3EB685DB, 0x65F9, 0x4CF6, 0xA0, 0x3A, 0xE3, 0xEF, 0x65, 0x72, 0x9F, 0x3D);
DEFINE_KNOWN_FOLDER(FOLDERID_SavedGames,          0x4c5c32ff, 0xbb9d, 0x43b0, 0xb5, 0xb4, 0x2d, 0x72, 0xe5, 0x4e, 0xaa, 0xa4);

typedef struct tagPOINT
{
	LONG  x;
	LONG  y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;


typedef struct tagMSG {
	HWND        hwnd;
	UINT        message;
	WPARAM      wParam;
	LPARAM      lParam;
	DWORD       time;
	POINT       pt;
#ifdef _MAC
	DWORD       lPrivate;
#endif
} MSG, *PMSG, *NPMSG,  *LPMSG;

typedef struct _STARTUPINFOA {
	DWORD   cb;
	LPSTR   lpReserved;
	LPSTR   lpDesktop;
	LPSTR   lpTitle;
	DWORD   dwX;
	DWORD   dwY;
	DWORD   dwXSize;
	DWORD   dwYSize;
	DWORD   dwXCountChars;
	DWORD   dwYCountChars;
	DWORD   dwFillAttribute;
	DWORD   dwFlags;
	WORD    wShowWindow;
	WORD    cbReserved2;
	LPBYTE  lpReserved2;
	HANDLE  hStdInput;
	HANDLE  hStdOutput;
	HANDLE  hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;

#ifdef UNICODE

#else // UNICODE
	typedef STARTUPINFOA STARTUPINFO;
#endif // NOT UNICODE

typedef struct _IO_COUNTERS {
	ULONGLONG  ReadOperationCount;
	ULONGLONG  WriteOperationCount;
	ULONGLONG  OtherOperationCount;
	ULONGLONG ReadTransferCount;
	ULONGLONG WriteTransferCount;
	ULONGLONG OtherTransferCount;
} IO_COUNTERS;
typedef IO_COUNTERS *PIO_COUNTERS;

typedef struct _JOBOBJECT_BASIC_LIMIT_INFORMATION {
	LARGE_INTEGER PerProcessUserTimeLimit;
	LARGE_INTEGER PerJobUserTimeLimit;
	DWORD LimitFlags;
	SIZE_T MinimumWorkingSetSize;
	SIZE_T MaximumWorkingSetSize;
	DWORD ActiveProcessLimit;
	ULONG_PTR Affinity;
	DWORD PriorityClass;
	DWORD SchedulingClass;
} JOBOBJECT_BASIC_LIMIT_INFORMATION, *PJOBOBJECT_BASIC_LIMIT_INFORMATION;

typedef struct _JOBOBJECT_EXTENDED_LIMIT_INFORMATION {
	JOBOBJECT_BASIC_LIMIT_INFORMATION BasicLimitInformation;
	IO_COUNTERS IoInfo;
	SIZE_T ProcessMemoryLimit;
	SIZE_T JobMemoryLimit;
	SIZE_T PeakProcessMemoryUsed;
	SIZE_T PeakJobMemoryUsed;
} JOBOBJECT_EXTENDED_LIMIT_INFORMATION, *PJOBOBJECT_EXTENDED_LIMIT_INFORMATION;

typedef struct tagWNDCLASSA {
	UINT        style;
	WNDPROC     lpfnWndProc;
	int         cbClsExtra;
	int         cbWndExtra;
	HINSTANCE   hInstance;
	HICON       hIcon;
	HCURSOR     hCursor;
	HBRUSH      hbrBackground;
	LPCSTR      lpszMenuName;
	LPCSTR      lpszClassName;
} WNDCLASSA, *PWNDCLASSA, *NPWNDCLASSA, *LPWNDCLASSA;
#ifdef UNICODE
typedef WNDCLASSW WNDCLASS;
typedef PWNDCLASSW PWNDCLASS;
typedef NPWNDCLASSW NPWNDCLASS;
typedef LPWNDCLASSW LPWNDCLASS;
#else
typedef WNDCLASSA WNDCLASS;
typedef PWNDCLASSA PWNDCLASS;
typedef NPWNDCLASSA NPWNDCLASS;
typedef LPWNDCLASSA LPWNDCLASS;
#endif // UNICODE

typedef struct tagPIXELFORMATDESCRIPTOR
{
	WORD  nSize;
	WORD  nVersion;
	DWORD dwFlags;
	BYTE  iPixelType;
	BYTE  cColorBits;
	BYTE  cRedBits;
	BYTE  cRedShift;
	BYTE  cGreenBits;
	BYTE  cGreenShift;
	BYTE  cBlueBits;
	BYTE  cBlueShift;
	BYTE  cAlphaBits;
	BYTE  cAlphaShift;
	BYTE  cAccumBits;
	BYTE  cAccumRedBits;
	BYTE  cAccumGreenBits;
	BYTE  cAccumBlueBits;
	BYTE  cAccumAlphaBits;
	BYTE  cDepthBits;
	BYTE  cStencilBits;
	BYTE  cAuxBuffers;
	BYTE  iLayerType;
	BYTE  bReserved;
	DWORD dwLayerMask;
	DWORD dwVisibleMask;
	DWORD dwDamageMask;
} PIXELFORMATDESCRIPTOR, *PPIXELFORMATDESCRIPTOR, *LPPIXELFORMATDESCRIPTOR;

typedef struct tagRECT
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

typedef struct tagMONITORINFO
{
	DWORD   cbSize;
	RECT    rcMonitor;
	RECT    rcWork;
	DWORD   dwFlags;
} MONITORINFO, *LPMONITORINFO;


extern "C++"
	{
		MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
		IUnknown
		{
		public:
				BEGIN_INTERFACE
				virtual HRESULT STDMETHODCALLTYPE QueryInterface(
					/* [in] */ REFIID riid,
					/* [iid_is][out] */ void**ppvObject) = 0;

				virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

				virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

				template<class Q>
				HRESULT

				STDMETHODCALLTYPE

				QueryInterface(Q** pp)
				{
					return QueryInterface(__uuidof(Q), (void **)pp);
				}

				END_INTERFACE
		};
	} // extern C++


// @Hack(tkap, 17/07/2022): Iunknown is a crazy interface thing that I don't understand, but since we only need a pointer, we can hack it
typedef IUnknown *LPUNKNOWN;


WINBASEAPI BOOL WINAPI IsDebuggerPresent(VOID);
WINUSERAPI int WINAPI MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
WINBASEAPI BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);
WINBASEAPI BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount);
WINUSERAPI BOOL WINAPI OpenClipboard(HWND hWndNewOwner);
WINUSERAPI HANDLE WINAPI GetClipboardData(UINT uFormat);
WINBASEAPI LPVOID WINAPI GlobalLock(HGLOBAL hMem);
WINBASEAPI BOOL WINAPI GlobalUnlock(HGLOBAL hMem);
WINUSERAPI BOOL WINAPI CloseClipboard(VOID);
WINBASEAPI DWORD WINAPI GetLastError(VOID);
WINBASEAPI DWORD WINAPI FormatMessageA(DWORD dwFlags, LPCVOID lpSource,DWORD dwMessageId,DWORD dwLanguageId, LPSTR lpBuffer, DWORD nSize, va_list *Arguments);
WINBASEAPI DWORD WINAPI FormatMessageW(DWORD dwFlags, LPCVOID lpSource,DWORD dwMessageId,DWORD dwLanguageId, LPWSTR lpBuffer, DWORD nSize, va_list *Arguments);
WINBASEAPI HLOCAL WINAPI LocalFree(HLOCAL hMem);
WINBASEAPI HANDLE WINAPI FindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
// WINBASEAPI HANDLE WINAPI FindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
WINBASEAPI BOOL WINAPI FindClose(HANDLE hFindFile);
WINBASEAPI DECLSPEC_NORETURN VOID WINAPI ExitProcess(UINT uExitCode);
WINBASEAPI DWORD WINAPI GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);
WINBASEAPI BOOL WINAPI CloseHandle(HANDLE hObject);
WINBASEAPI BOOL WINAPI FindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
WINGDIAPI PROC  WINAPI wglGetProcAddress(LPCSTR);
WINUSERAPI VOID WINAPI PostQuitMessage(int nExitCode);
WINUSERAPI BOOL WINAPI SetCursorPos(int X, int Y);
WINBASEAPI LONG WINAPI CompareFileTime(CONST FILETIME* lpFileTime1, CONST FILETIME* lpFileTime2);
WINBASEAPI VOID WINAPI Sleep(DWORD dwMilliseconds);
WINBASEAPI LPVOID WINAPI VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
WINBASEAPI HMODULE WINAPI LoadLibraryA(LPCSTR lpLibFileName);
WINBASEAPI FARPROC WINAPI GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
WINUSERAPI BOOL WINAPI PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
WINUSERAPI BOOL WINAPI TranslateMessage(CONST MSG *lpMsg);
WINUSERAPI LRESULT WINAPI DispatchMessageA(CONST MSG *lpMsg);
WINUSERAPI BOOL WINAPI GetCursorPos(LPPOINT lpPoint);
WINUSERAPI BOOL WINAPI ScreenToClient(HWND hWnd, LPPOINT lpPoint);
WINUSERAPI int WINAPI ShowCursor(BOOL bShow);
WINBASEAPI HANDLE WINAPI CreateJobObjectA(LPSECURITY_ATTRIBUTES lpJobAttributes, LPCSTR lpName);
WINBASEAPI BOOL WINAPI AssignProcessToJobObject(HANDLE hJob, HANDLE hProcess);
WINBASEAPI BOOL WINAPI SetInformationJobObject(HANDLE hJob, JOBOBJECTINFOCLASS JobObjectInformationClass, LPVOID lpJobObjectInformation, DWORD cbJobObjectInformationLength);
WINBASEAPI BOOL WINAPI TerminateProcess(HANDLE hProcess, UINT uExitCode);
WINGDIAPI BOOL  WINAPI SwapBuffers(HDC);
WINBASEAPI BOOL WINAPI FreeLibrary(HMODULE hLibModule);
WINBASEAPI BOOL WINAPI CopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);
WINUSERAPI UINT WINAPI MapVirtualKeyA(UINT uCode, UINT uMapType);
WINUSERAPI LRESULT WINAPI DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WINBASEAPI VOID WINAPI OutputDebugStringA(LPCSTR lpOutputString);
WINBASEAPI HMODULE WINAPI GetModuleHandleA(LPCSTR lpModuleName);
WINUSERAPI HCURSOR WINAPI LoadCursorA(HINSTANCE hInstance, LPCSTR lpCursorName);
WINUSERAPI ATOM WINAPI RegisterClassA(CONST WNDCLASSA *lpWndClass);
WINUSERAPI HDC WINAPI GetDC(HWND hWnd);
WINGDIAPI int  WINAPI ChoosePixelFormat(HDC hdc,CONST PIXELFORMATDESCRIPTOR *ppfd);
WINGDIAPI BOOL  WINAPI SetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd);
WINGDIAPI HGLRC WINAPI wglCreateContext(HDC);
WINGDIAPI BOOL  WINAPI wglMakeCurrent(HDC, HGLRC);
WINGDIAPI BOOL  WINAPI wglDeleteContext(HGLRC);
WINUSERAPI int WINAPI ReleaseDC(HWND hWnd, HDC hDC);
WINUSERAPI BOOL WINAPI DestroyWindow(HWND hWnd);
WINUSERAPI HMONITOR WINAPI MonitorFromWindow(HWND hwnd, DWORD dwFlags);
WINUSERAPI BOOL WINAPI GetMonitorInfoA(HMONITOR hMonitor, LPMONITORINFO lpmi);
WINUSERAPI BOOL WINAPI SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
WINGDIAPI int  WINAPI DescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
WINUSERAPI HWND WINAPI FindWindowA(LPCSTR lpClassName, LPCSTR lpWindowName);
WINUSERAPI BOOL WINAPI SetForegroundWindow(HWND hWnd);
SHFOLDERAPI SHGetFolderPathA(HWND hwnd, int csidl, HANDLE hToken, DWORD dwFlags, LPSTR pszPath);
STDAPI SHGetKnownFolderPath(REFKNOWNFOLDERID rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);
WINOLEAPI_(void) CoTaskMemFree(LPVOID pv);
BOOL DeleteFileA(LPCSTR lpFileName);

BOOL GetExitCodeProcess(HANDLE  hProcess, LPDWORD lpExitCode);

int
WINAPI
WideCharToMultiByte(
    UINT CodePage,
    DWORD dwFlags,
    LPCWCH lpWideCharStr,
    int cchWideChar,
    LPSTR lpMultiByteStr,
    int cbMultiByte,
    LPCCH lpDefaultChar,
    LPBOOL lpUsedDefaultChar
    );

WINBASEAPI
int
WINAPI
MultiByteToWideChar(
    UINT CodePage,
    DWORD dwFlags,
    LPCCH lpMultiByteStr,
    int cbMultiByte,
    LPWSTR lpWideCharStr,
    int cchWideChar
    );

WINBASEAPI
HANDLE
WINAPI
CreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    );

// @Note(tkap, 13/08/2022): Slopy here. I'm just copying the function header from windows documentation, rather than going into Visual Studio and finding
// the header. We'll see if it works...
BOOL GetClientRect(
  HWND   hWnd,
  LPRECT lpRect
);

DWORD WaitForSingleObjectEx(HANDLE hHandle, DWORD  dwMilliseconds, BOOL   bAlertable);
HANDLE CreateSemaphoreExA(
  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
  LONG                  lInitialCount,
  LONG                  lMaximumCount,
  LPCSTR                lpName,
  DWORD                 dwFlags,
  DWORD                 dwDesiredAccess
);

BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG   lReleaseCount, LPLONG lpPreviousCount);
DWORD GetCurrentThreadId();

#define InterlockedIncrement(...) _InterlockedIncrement(__VA_ARGS__)
#define InterlockedDecrement(...) _InterlockedDecrement(__VA_ARGS__)
#define InterlockedExchange(...) _InterlockedExchange(__VA_ARGS__)
#define InterlockedAdd(...) _InterlockedAdd(__VA_ARGS__)
#define InterlockedAdd64(...) _InlineInterlockedAdd64(__VA_ARGS__)
#define InterlockedCompareExchange(...) _InterlockedCompareExchange(__VA_ARGS__)
#define InterlockedCompareExchange64 _InterlockedCompareExchange64
__int64 _InterlockedCompareExchange64(__int64 volatile * _Destination, __int64 _Exchange, __int64 _Comparand);
LONG InterlockedCompareExchange(LONG volatile *Destination, LONG ExChange, LONG Comperand);
LONG _InterlockedIncrement(LONG volatile *Addend);
LONG _InterlockedDecrement(LONG volatile *Addend);
LONG _InterlockedExchange(LONG volatile *Target, LONG Value);
LONG _InterlockedAdd(LONG volatile *Target, LONG Value);

static LONG64
_InlineInterlockedAdd64(LONG64 volatile *Addend, LONG64 Value)
{
	LONG64 Old;

	do {
			Old = *Addend;
	} while (InterlockedCompareExchange64(Addend,
																				Old + Value,
																				Old) != Old);

	return Old + Value;
}

void GetSystemInfo(LPSYSTEM_INFO lpSystemInfo);

HANDLE CreateThread(
	LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE  lpStartAddress,
	LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId
);

BOOL PathFileExistsA(
  LPCSTR pszPath
);

BOOL PathFileExistsW(
  LPCWSTR pszPath
);

WINBASEAPI
BOOL
WINAPI
SetConsoleTitleA(
	LPCSTR lpConsoleTitle
);

HWND GetActiveWindow();

WINBASEAPI
BOOL
WINAPI
CreateDirectoryW(
    LPCWSTR lpPathName,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );

BOOL GetWindowRect(
  HWND   hWnd,
  LPRECT lpRect
);

HRGN CreateRectRgnIndirect(
  const RECT *lprect
);

HWND GetTopWindow(
  HWND hWnd
);

BOOL IsWindowVisible(
  HWND hWnd
);

int CombineRgn(
  HRGN hrgnDst,
  HRGN hrgnSrc1,
  HRGN hrgnSrc2,
  int  iMode
);

BOOL DeleteObject(
  HGDIOBJ ho
);

HWND GetWindow(
  HWND hWnd,
  UINT uCmd
);

WINUSERAPI
LONG_PTR
WINAPI
GetWindowLongPtrA(
    HWND hWnd,
    int nIndex);

WINUSERAPI
LONG_PTR
WINAPI
SetWindowLongPtrA(
    HWND hWnd,
    int nIndex,
    LONG_PTR dwNewLong);

WINUSERAPI HWND WINAPI CreateWindowExA(
	DWORD dwExStyle,
	LPCSTR lpClassName,
	LPCSTR lpWindowName,
	DWORD dwStyle,
	int X,
	int Y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam
);

WINBASEAPI BOOL WINAPI CreateProcessA(
	LPCSTR lpApplicationName,
	LPSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCSTR lpCurrentDirectory,
	LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation
);

WINBASEAPI HANDLE WINAPI CreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
);

WINBASEAPI BOOL WINAPI WriteFile(
	HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
);

WINBASEAPI BOOL WINAPI ReadFile(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
);

WINUSERAPI
BOOL
WINAPI
SetLayeredWindowAttributes(
    HWND hwnd,
    COLORREF crKey,
    BYTE bAlpha,
    DWORD dwFlags);

WINUSERAPI
HICON
WINAPI
LoadIconA(
    HINSTANCE hInstance,
    LPCSTR lpIconName);

WINUSERAPI
HANDLE
WINAPI
LoadImageA(
    HINSTANCE hInst,
    LPCSTR name,
    UINT type,
    int cx,
    int cy,
    UINT fuLoad);

WINBASEAPI
BOOL
WINAPI
SetConsoleTextAttribute(
    HANDLE hConsoleOutput,
    WORD wAttributes
    );

WINBASEAPI
HANDLE
WINAPI
GetStdHandle(
    DWORD nStdHandle
    );

WINBASEAPI
DWORD
WINAPI
WaitForSingleObject(
    HANDLE hHandle,
    DWORD dwMilliseconds
    );

WINBASEAPI
BOOL
WINAPI
ReleaseMutex(
    HANDLE hMutex
    );

WINBASEAPI
HANDLE
WINAPI
CreateMutexA(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    BOOL bInitialOwner,
    LPCSTR lpName
    );

#ifdef UNICODE
#define CreateMutex  CreateMutexW
#else
#define CreateMutex  CreateMutexA
#endif // !UNICODE

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		DIRECT SOUND START		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

#define DSSCL_PRIORITY              0x00000002
#define DSBCAPS_PRIMARYBUFFER       0x00000001
#define WAVE_FORMAT_PCM     1
#define DSBCAPS_GLOBALFOCUS         0x00008000
#define DSBPLAY_LOOPING             0x00000001
#define DS_OK                           S_OK

typedef struct tWAVEFORMATEX
{
	WORD        wFormatTag;         /* format type */
	WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
	DWORD       nSamplesPerSec;     /* sample rate */
	DWORD       nAvgBytesPerSec;    /* for buffer estimation */
	WORD        nBlockAlign;        /* block size of data */
	WORD        wBitsPerSample;     /* number of bits per sample of mono data */
	WORD        cbSize;             /* the count in bytes of the size of */
												/* extra information (after cbSize) */
} WAVEFORMATEX, *PWAVEFORMATEX, *NPWAVEFORMATEX, *LPWAVEFORMATEX;

typedef struct _DSBUFFERDESC
{
	DWORD           dwSize;
	DWORD           dwFlags;
	DWORD           dwBufferBytes;
	DWORD           dwReserved;
	LPWAVEFORMATEX  lpwfxFormat;
// #if DIRECTSOUND_VERSION >= 0x0700
	GUID            guid3DAlgorithm;
// #endif
} DSBUFFERDESC, *LPDSBUFFERDESC;

typedef struct _DSCAPS
{
	DWORD           dwSize;
	DWORD           dwFlags;
	DWORD           dwMinSecondarySampleRate;
	DWORD           dwMaxSecondarySampleRate;
	DWORD           dwPrimaryBuffers;
	DWORD           dwMaxHwMixingAllBuffers;
	DWORD           dwMaxHwMixingStaticBuffers;
	DWORD           dwMaxHwMixingStreamingBuffers;
	DWORD           dwFreeHwMixingAllBuffers;
	DWORD           dwFreeHwMixingStaticBuffers;
	DWORD           dwFreeHwMixingStreamingBuffers;
	DWORD           dwMaxHw3DAllBuffers;
	DWORD           dwMaxHw3DStaticBuffers;
	DWORD           dwMaxHw3DStreamingBuffers;
	DWORD           dwFreeHw3DAllBuffers;
	DWORD           dwFreeHw3DStaticBuffers;
	DWORD           dwFreeHw3DStreamingBuffers;
	DWORD           dwTotalHwMemBytes;
	DWORD           dwFreeHwMemBytes;
	DWORD           dwMaxContigFreeHwMemBytes;
	DWORD           dwUnlockTransferRateHwBuffers;
	DWORD           dwPlayCpuOverheadSwBuffers;
	DWORD           dwReserved1;
	DWORD           dwReserved2;
} DSCAPS, *LPDSCAPS;

typedef struct _DSBCAPS
{
	DWORD           dwSize;
	DWORD           dwFlags;
	DWORD           dwBufferBytes;
	DWORD           dwUnlockTransferRate;
	DWORD           dwPlayCpuOverhead;
} DSBCAPS, *LPDSBCAPS;



typedef struct IDirectSoundBuffer           *LPDIRECTSOUNDBUFFER;
typedef struct IDirectSound                 *LPDIRECTSOUND;
typedef const GUID *LPCGUID;
typedef const DSBUFFERDESC *LPCDSBUFFERDESC;
typedef const WAVEFORMATEX *LPCWAVEFORMATEX;

#define INTERFACE IDirectSound
DECLARE_INTERFACE_(IDirectSound, IUnknown)
{
	// IUnknown methods
	STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
	STDMETHOD_(ULONG,Release)       (THIS) PURE;

	// IDirectSound methods
	STDMETHOD(CreateSoundBuffer)    (THIS_ LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOuter) PURE;
	STDMETHOD(GetCaps)              (THIS_ LPDSCAPS pDSCaps) PURE;
	STDMETHOD(DuplicateSoundBuffer) (THIS_ LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate) PURE;
	STDMETHOD(SetCooperativeLevel)  (THIS_ HWND hwnd, DWORD dwLevel) PURE;
	STDMETHOD(Compact)              (THIS) PURE;
	STDMETHOD(GetSpeakerConfig)     (THIS_ LPDWORD pdwSpeakerConfig) PURE;
	STDMETHOD(SetSpeakerConfig)     (THIS_ DWORD dwSpeakerConfig) PURE;
	STDMETHOD(Initialize)           (THIS_ LPCGUID pcGuidDevice) PURE;
};

#define IDirectSound_QueryInterface(p,a,b)       IUnknown_QueryInterface(p,a,b)
#define IDirectSound_AddRef(p)                   IUnknown_AddRef(p)
#define IDirectSound_Release(p)                  IUnknown_Release(p)

#define IDirectSound_CreateSoundBuffer(p,a,b,c)  (p)->CreateSoundBuffer(a,b,c)
#define IDirectSound_GetCaps(p,a)                (p)->GetCaps(a)
#define IDirectSound_DuplicateSoundBuffer(p,a,b) (p)->DuplicateSoundBuffer(a,b)
#define IDirectSound_SetCooperativeLevel(p,a,b)  (p)->SetCooperativeLevel(a,b)
#define IDirectSound_Compact(p)                  (p)->Compact()
#define IDirectSound_GetSpeakerConfig(p,a)       (p)->GetSpeakerConfig(a)
#define IDirectSound_SetSpeakerConfig(p,b)       (p)->SetSpeakerConfig(b)
#define IDirectSound_Initialize(p,a)             (p)->Initialize(a)

#undef INTERFACE

#define INTERFACE IDirectSoundBuffer

DECLARE_INTERFACE_(IDirectSoundBuffer, IUnknown)
{
	// IUnknown methods
	STDMETHOD(QueryInterface)       (THIS_  REFIID, LPVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
	STDMETHOD_(ULONG,Release)       (THIS) PURE;

	// IDirectSoundBuffer methods
	STDMETHOD(GetCaps)              (THIS_  LPDSBCAPS pDSBufferCaps) PURE;
	STDMETHOD(GetCurrentPosition)   (THIS_  LPDWORD pdwCurrentPlayCursor,  LPDWORD pdwCurrentWriteCursor) PURE;
	STDMETHOD(GetFormat)            (THIS_ LPWAVEFORMATEX pwfxFormat, DWORD dwSizeAllocated,  LPDWORD pdwSizeWritten) PURE;
	STDMETHOD(GetVolume)            (THIS_  LPLONG plVolume) PURE;
	STDMETHOD(GetPan)               (THIS_  LPLONG plPan) PURE;
	STDMETHOD(GetFrequency)         (THIS_  LPDWORD pdwFrequency) PURE;
	STDMETHOD(GetStatus)            (THIS_  LPDWORD pdwStatus) PURE;
	STDMETHOD(Initialize)           (THIS_  LPDIRECTSOUND pDirectSound,  LPCDSBUFFERDESC pcDSBufferDesc) PURE;
	STDMETHOD(Lock)                 (THIS_ DWORD dwOffset, DWORD dwBytes,
														LPVOID *ppvAudioPtr1,  LPDWORD pdwAudioBytes1,
														LPVOID *ppvAudioPtr2,  LPDWORD pdwAudioBytes2, DWORD dwFlags) PURE;
	STDMETHOD(Play)                 (THIS_ DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags) PURE;
	STDMETHOD(SetCurrentPosition)   (THIS_ DWORD dwNewPosition) PURE;
	STDMETHOD(SetFormat)            (THIS_  LPCWAVEFORMATEX pcfxFormat) PURE;
	STDMETHOD(SetVolume)            (THIS_ LONG lVolume) PURE;
	STDMETHOD(SetPan)               (THIS_ LONG lPan) PURE;
	STDMETHOD(SetFrequency)         (THIS_ DWORD dwFrequency) PURE;
	STDMETHOD(Stop)                 (THIS) PURE;
	STDMETHOD(Unlock)               (THIS_ LPVOID pvAudioPtr1, DWORD dwAudioBytes1,
														LPVOID pvAudioPtr2, DWORD dwAudioBytes2) PURE;
	STDMETHOD(Restore)              (THIS) PURE;
};

#undef INTERFACE

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		DIRECT SOUND END		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		WINSOCK2 START		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
typedef UINT_PTR        SOCKET;


typedef struct fd_set {
		u_int fd_count;               /* how many are SET? */
		SOCKET  fd_array[FD_SETSIZE];   /* an array of SOCKETs */
} fd_set;

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		WINSOCK2 END		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif