ANSI_CHARSET         EQU 0                      ; Constants
BLACKNESS            EQU 42h
CLIP_DEFAULT_PRECIS  EQU 0
CS_BYTEALIGNWINDOW   EQU 2000h
CS_HREDRAW           EQU 2
CS_VREDRAW           EQU 1
DEFAULT_PITCH        EQU 0
ES_AUTOHSCROLL       EQU 80h
ES_CENTER            EQU 1
FALSE                EQU 0
GRAY_BRUSH           EQU 2
IDC_ARROW            EQU 7F00h
IDI_APPLICATION      EQU 7F00h
IDNO                 EQU 7
IMAGE_CURSOR         EQU 2
IMAGE_ICON           EQU 1
LR_SHARED            EQU 8000h
MB_DEFBUTTON2        EQU 100h
MB_YESNO             EQU 4
NULL                 EQU 0
NULL_BRUSH           EQU 5
OPAQUE               EQU 2
PROOF_QUALITY        EQU 2
SM_CXFULLSCREEN      EQU 10h
SM_CYFULLSCREEN      EQU 11h
SS_CENTER            EQU 1
SS_NOTIFY            EQU 100h
SW_SHOWNORMAL        EQU 1
TRUE                 EQU 1
WM_CLOSE             EQU 10h
WM_COMMAND           EQU 111h
WM_CREATE            EQU 1
WM_CTLCOLOREDIT      EQU 133h
WM_CTLCOLORSTATIC    EQU 138h
WM_DESTROY           EQU 2
WM_PAINT             EQU 0Fh
WM_SETFONT           EQU 30h
OUT_DEFAULT_PRECIS   EQU 0
WS_CHILD             EQU 40000000h
WS_EX_COMPOSITED     EQU 2000000h
WS_OVERLAPPEDWINDOW  EQU 0CF0000h
BS_DEFPUSHBUTTON     EQU 0x00000001
WS_TABSTOP           EQU 10000h
WS_VISIBLE           EQU 10000000h
WM_CTLCOLORBTN       EQU 0x0135
purpleColor          EQU 0x800080

WindowWidth          EQU 500
WindowHeight         EQU 500

Static2ID         EQU 101
Edit1ID           EQU 102
Edit2ID           EQU 103
welcomeTextId            EQU 104
showButtonId         EQU 105
extern AdjustWindowRectEx                       ; Import external symbols
extern BeginPaint                               ; Windows API functions, not decorated
extern BitBlt
extern CreateFontA
extern CreateSolidBrush
extern CreateWindowExA
extern DefWindowProcA
extern DeleteObject
extern DestroyWindow
extern DispatchMessageA
extern EndPaint
extern ExitProcess
extern GetDlgCtrlID
extern GetStockObject
extern GetMessageA
extern GetModuleHandleA
extern GetSystemMetrics
extern InvalidateRect
extern IsDialogMessageA
extern LoadImageA
extern MessageBoxA
extern PostQuitMessage
extern RegisterClassExA
extern SendMessageA
extern GetSysColorBrush
extern SetBkColor
extern SetBkMode
extern SetTextColor
extern ShowWindow
extern TranslateMessage
extern UpdateWindow

global Start                                    ; Export symbols. The entry point

section .data                                   ; Initialized data segment
 Static1Colour    dd 0F0F0F0h                   ; Colour (0BBGGRRh)
 Static1ColourA   dd 020A0F0h
 Edit1TextColour  dd 0F590F5h
 Edit1BackColour  dd 0xFFFFFF
 Edit2TextColour  dd 0A56E3Bh
 Edit2BackColour  dd 0FEFE8Eh
 BackgroundColour dd 0xFFFFFF   
 WindowName       db "", 0
 ClassName        db "Window", 0
 SegoeUI          db "Segoe UI", 0
 StaticClass      db "STATIC", 0
 EditClass        db "EDIT", 0
 ExitText         db "Você quer sair?", 0
 
 welcomeTextText      db "Clique no botao:", 0 
 welcomeTextColour       dd 0x000000, 0 
 welcomeTextBackColour       dd 0xFFFFFF, 0 
 showButtonText         db "Clique em mim", 0 
 buttonClass             db "BUTTON", 0 

 showButtonExitText          db "Ola estou bem tambem esta?", 0 
section .bss                                    ; Uninitialized data segment
 alignb 8
 hInstance        resq 1
 BackgroundBrush  resq 1
 Font             resq 1
 Static2          resq 1
 Edit1            resq 1
 Edit2            resq 1
 

 welcomeText            resq 1 

 showButton           resq 1 
section .text
 
                                   ; Code segment
Start:
 sub   RSP, 8                                   ; Align stack pointer to 16 bytes

 sub   RSP, 32                                  ; 32 bytes of shadow space
 xor   ECX, ECX
 call  GetModuleHandleA
 mov   qword [REL hInstance], RAX
 add   RSP, 32                                  ; Remove the 32 bytes

 call  WinMain

.Exit:
 xor   ECX, ECX
 call  ExitProcess

WinMain:
 push  RBP                                      ; Set up a stack frame
 mov   RBP, RSP
 sub   RSP, 160 + 64 + 32                       ; 160 bytes for local variables
                                                ; + 64 (8 * 8 byte) parameters 
                                                ; + 32 shadow space
                                                ; Kept to a multiple of 16 for API functions

%define Screen.Width       RBP - 160            ; 4 bytes
%define Screen.Height      RBP - 156            ; 4 bytes

%define ClientArea         RBP - 152            ; RECT structure. 16 bytes
%define ClientArea.left    RBP - 152            ; 4 bytes. Start on a 4 byte boundary
%define ClientArea.top     RBP - 148            ; 4 bytes
%define ClientArea.right   RBP - 144            ; 4 bytes
%define ClientArea.bottom  RBP - 140            ; 4 bytes. End on a 4 byte boundary

%define wc                 RBP - 136            ; WNDCLASSEX structure, 80 bytes
%define wc.cbSize          RBP - 136            ; 4 bytes. Start on an 8 byte boundary
%define wc.style           RBP - 132            ; 4 bytes
%define wc.lpfnWndProc     RBP - 128            ; 8 bytes
%define wc.cbClsExtra      RBP - 120            ; 4 bytes
%define wc.cbWndExtra      RBP - 116            ; 4 bytes
%define wc.hInstance       RBP - 112            ; 8 bytes
%define wc.hIcon           RBP - 104            ; 8 bytes
%define wc.hCursor         RBP - 96             ; 8 bytes
%define wc.hbrBackground   RBP - 88             ; 8 bytes
%define wc.lpszMenuName    RBP - 80             ; 8 bytes
%define wc.lpszClassName   RBP - 72             ; 8 bytes
%define wc.hIconSm         RBP - 64             ; 8 bytes. End on an 8 byte boundary

%define msg                RBP - 56             ; MSG structure, 48 bytes
%define msg.hwnd           RBP - 56             ; 8 bytes. Start on an 8 byte boundary
%define msg.message        RBP - 48             ; 4 bytes
%define msg.Padding1       RBP - 44             ; 4 bytes. Natural alignment padding
%define msg.wParam         RBP - 40             ; 8 bytes
%define msg.lParam         RBP - 32             ; 8 bytes
%define msg.time           RBP - 24             ; 4 bytes
%define msg.py.x           RBP - 20             ; 4 bytes
%define msg.pt.y           RBP - 16             ; 4 bytes
%define msg.Padding2       RBP - 12             ; 4 bytes. Structure length padding

%define hWnd               RBP - 8              ; 8 bytes

 mov   ECX, dword [REL BackgroundColour]
 call  CreateSolidBrush                         ; Create a brush for the window backgound
 mov   qword [REL BackgroundBrush], RAX

 mov   dword [wc.cbSize], 80                    ; [RBP - 136]
 mov   dword [wc.style], CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNWINDOW  ; [RBP - 132]
 lea   RAX, [REL WndProc]
 mov   qword [wc.lpfnWndProc], RAX              ; [RBP - 128]
 mov   dword [wc.cbClsExtra], NULL              ; [RBP - 120]
 mov   dword [wc.cbWndExtra], NULL              ; [RBP - 116]
 mov   RAX, qword [REL hInstance]
 mov   qword [wc.hInstance], RAX                ; [RBP - 112]

 xor   ECX, ECX
 mov   EDX, IDI_APPLICATION
 mov   R8D, IMAGE_ICON
 xor   R9D, R9D
 mov   qword [RSP + 4 * 8], NULL
 mov   qword [RSP + 5 * 8], LR_SHARED
 call  LoadImageA                               ; Large program icon
 mov   qword [wc.hIcon], RAX                    ; [RBP - 104]

 xor   ECX, ECX
 mov   EDX, IDC_ARROW
 mov   R8D, IMAGE_CURSOR
 xor   R9D, R9D
 mov   qword [RSP + 4 * 8], NULL
 mov   qword [RSP + 5 * 8], LR_SHARED
 call  LoadImageA                               ; Cursor
 mov   qword [wc.hCursor], RAX                  ; [RBP - 96]

 mov   RAX, qword [REL BackgroundBrush]
 mov   qword [wc.hbrBackground], RAX            ; [RBP - 88]
 mov   qword [wc.lpszMenuName], NULL            ; [RBP - 80]
 lea   RAX, [REL ClassName]
 mov   qword [wc.lpszClassName], RAX            ; [RBP - 72]

 xor   ECX, ECX
 mov   EDX, IDI_APPLICATION
 mov   R8D, IMAGE_ICON
 xor   R9D, R9D
 mov   qword [RSP + 4 * 8], NULL
 mov   qword [RSP + 5 * 8], LR_SHARED
 call  LoadImageA                               ; Small program icon
 mov   qword [wc.hIconSm], RAX                  ; [RBP - 64]

 lea   RCX, [wc]                                ; [RBP - 136]
 call  RegisterClassExA

 mov   ECX, SM_CXFULLSCREEN
 call  GetSystemMetrics                         ; Get the current screen width
 mov   dword [Screen.Width], EAX                ; [RBP - 160]

 mov   ECX, SM_CYFULLSCREEN
 call  GetSystemMetrics                         ; Get the current screen height
 mov   dword [Screen.Height], EAX               ; [RBP - 156]

 mov   dword [ClientArea.left], 0               ; [RBP - 152]
 mov   dword [ClientArea.top], 0                ; [RBP - 148]
 mov   dword [ClientArea.right], WindowWidth    ; [RBP - 144]
 mov   dword [ClientArea.bottom], WindowHeight  ; [RBP - 140]

 lea   RCX, [ClientArea]                        ; [RBP - 152]
 mov   EDX, WS_OVERLAPPEDWINDOW                 ; Style
 xor   R8D, R8D
 mov   R9D, WS_EX_COMPOSITED                    ; Extended style
 call  AdjustWindowRectEx                       ; Get window size for the desired client size
                                                ; Size is returned in ClientArea
 mov   EAX, dword [ClientArea.top]              ; [RBP - 148]
 sub   dword [ClientArea.bottom], EAX           ; New Height = ClientArea.bottom - ClientArea.top

 mov   EAX, dword [ClientArea.left]             ; [RBP - 152]
 sub   dword [ClientArea.right], EAX            ; New Width = ClientArea.right - ClientArea.left

 mov   ECX, WS_EX_COMPOSITED
 lea   RDX, [REL ClassName]
 lea   R8, [REL WindowName]
 mov   R9D, WS_OVERLAPPEDWINDOW

 xor   R10D, R10D
 mov   EAX, dword [Screen.Width]                ; [RBP - 160]
 sub   EAX, dword [ClientArea.right]            ; Corrected window width. [RBP - 144]
 cmovs EAX, R10D                                ; Clamp to 0 (left) if negative
 shr   EAX, 1                                   ; EAX = (Screen.Width - window height) / 2
 mov   dword [RSP + 4 * 8], EAX                 ; X position, now centred

 mov   EAX, dword [Screen.Height]               ; [RBP - 156]
 sub   EAX, dword [ClientArea.bottom]           ; Corrected window height. [RBP - 140]
 cmovs EAX, R10D                                ; Clamp to 0 (top) if negative
 shr   EAX, 1                                   ; EAX = (Screen.Height - window height) / 2
 mov   dword [RSP + 5 * 8], EAX                 ; Y position, now centred

 mov   EAX, dword [ClientArea.right]            ; [RBP - 144]
 mov   dword [RSP + 6 * 8], EAX                 ; Width

 mov   EAX, dword [ClientArea.bottom]           ; [RBP - 140]
 mov   dword [RSP + 7 * 8], EAX                 ; Height

 mov   qword [RSP + 8 * 8], NULL
 mov   qword [RSP + 9 * 8], NULL
 mov   RAX, qword [REL hInstance]
 mov   qword [RSP + 10 * 8], RAX
 mov   qword [RSP + 11 * 8], NULL
 call  CreateWindowExA
 mov   qword [hWnd], RAX                        ; [RBP - 8]

 mov   RCX, qword [hWnd]                        ; [RBP - 8]
 mov   EDX, SW_SHOWNORMAL
 call  ShowWindow

 mov   RCX, qword [hWnd]                        ; [RBP - 8]
 call  UpdateWindow
 

.MessageLoop:
 lea   RCX, [msg]                               ; [RBP - 56]
 xor   EDX, EDX
 xor   R8D, R8D
 xor   R9D, R9D
 call  GetMessageA
 cmp   RAX, 0
 je    .Done

 mov   RCX, qword [hWnd]                        ; [RBP - 8]
 lea   RDX, [msg]                               ; [RBP - 56]
 call  IsDialogMessageA                         ; For keyboard strokes
 cmp   RAX, 0
 jne   .MessageLoop                             ; Skip TranslateMessage and DispatchMessageA

 lea   RCX, [msg]                               ; [RBP - 56]
 call  TranslateMessage

 lea   RCX, [msg]                               ; [RBP - 56]
 call  DispatchMessageA
 jmp   .MessageLoop

.Done:
 xor   EAX, EAX
 mov   RSP, RBP                                 ; Remove the stack frame
 pop   RBP
 ret

WndProc:
 push  RBP                                      ; Set up a stack frame
 mov   RBP, RSP

 sub   RSP, 80 + 80 + 32                        ; 80 bytes for local variables
                                                ; + 80 (10 * 8 byte) parameters
                                                ; + 32 shadow space
                                                ; Kept to a multiple of 16 for API functions

%define hWnd                RBP + 16            ; Location of the shadow space setup by
%define uMsg                RBP + 24            ; the calling function
%define wParam              RBP + 32
%define lParam              RBP + 40

%define ps                  RBP - 80            ; PAINTSTRUCT structure. 72 bytes
%define ps.hdc              RBP - 80            ; 8 bytes. Start on an 8 byte boundary
%define ps.fErase           RBP - 72            ; 4 bytes
%define ps.rcPaint.left     RBP - 68            ; 4 bytes
%define ps.rcPaint.top      RBP - 64            ; 4 bytes
%define ps.rcPaint.right    RBP - 60            ; 4 bytes
%define ps.rcPaint.bottom   RBP - 56            ; 4 bytes
%define ps.Restore          RBP - 52            ; 4 bytes
%define ps.fIncUpdate       RBP - 48            ; 4 bytes
%define ps.rgbReserved      RBP - 44            ; 32 bytes
%define ps.Padding          RBP - 12            ; 4 bytes. Structure length padding

%define hdc                 RBP - 8             ; 8 bytes

 mov   qword [hWnd], RCX                        ; Free up RCX RDX R8 R9 by spilling the
 mov   qword [uMsg], RDX                        ; 4 passed parameters to the shadow space
 mov   qword [wParam], R8                       ; We can now access these parameters by name
 mov   qword [lParam], R9

 cmp   qword [uMsg], WM_CLOSE                   ; [RBP + 24]
 je    WMCLOSE

 cmp   qword [uMsg], WM_COMMAND                 ; [RBP + 24]
 je    WMCOMMAND

 cmp   qword [uMsg], WM_CREATE                  ; [RBP + 24]
 je    WMCREATE

 cmp   qword [uMsg], WM_CTLCOLOREDIT            ; [RBP + 24]
 je    WMCTLCOLOREDIT

 cmp   qword [uMsg], WM_CTLCOLORSTATIC          ; [RBP + 24]
 je    WMCTLCOLORSTATIC

 cmp   qword [uMsg], WM_DESTROY                 ; [RBP + 24]
 je    WMDESTROY

 cmp   qword [uMsg], WM_PAINT                   ; [RBP + 24]
 je    WMPAINT


DefaultMessage:
 mov   RCX, qword [hWnd]                        ; [RBP + 16]
 mov   RDX, qword [uMsg]                        ; [RBP + 24]
 mov   R8, qword [wParam]                       ; [RBP + 32]
 mov   R9, qword [lParam]                       ; [RBP + 40]
 call  DefWindowProcA
 jmp   Return

WMCLOSE:
 mov   RCX, qword [hWnd]                        ; [RBP + 16]
 lea   RDX, [REL ExitText]
 lea   R8, [REL WindowName]
 mov   R9D, MB_YESNO | MB_DEFBUTTON2
 call  MessageBoxA

 cmp   RAX, IDNO
 je    Return.WM_Processed

 mov   RCX, qword [hWnd]                        ; [RBP + 16]
 call  DestroyWindow                            ; Send a WM_DESTROY message
 jmp   Return.WM_Processed

WMCOMMAND:
 mov   RAX, qword [wParam]                      ; RAX = ID. [RBP + 32]

 cmp   AX, welcomeTextId
 je    .welcomeText




 cmp AX, showButtonId
 je  .showButton
.welcomeText:
 mov   EAX, dword [REL welcomeTextColour]
 mov   ECX, dword [REL welcomeTextBackColour]
 mov   dword [REL welcomeTextColour], ECX
 mov   dword [REL welcomeTextBackColour], EAX          ; Swap colours

 mov   RCX, qword [lParam]                      ; Static1 handle. [RBP + 40]
 xor   EDX, EDX
 mov   R8D, TRUE
 call  InvalidateRect                           ; Redraw control
 jmp   Return.WM_Processed
.showButton: 

 mov   RCX, qword [hWnd]                        ; [RBP + 16]
 lea   RDX, [REL showButtonExitText]
 lea   R8, [REL WindowName]
 mov   R9D, MB_YESNO | MB_DEFBUTTON2
 call  MessageBoxA

 cmp   RAX, IDNO
 je    Return.WM_Processed

 mov   RCX, qword [hWnd]                        ; [RBP + 16]           
 jmp   Return.WM_Processed
 
WMCREATE:
 
 
 xor   ECX, ECX
 lea   RDX, [REL StaticClass]
 lea   R8, [REL welcomeTextText]                          ; Default text
 mov   R9D, WS_CHILD | WS_VISIBLE | SS_NOTIFY | SS_CENTER
 mov   qword [RSP + 4 * 8], 120                 ; X
 mov   qword [RSP + 5 * 8], 10                  ; Y
 mov   qword [RSP + 6 * 8], 400                 ; Width
 mov   qword [RSP + 7 * 8], 20                  ; Height
 mov   RAX, qword [hWnd]                        ; [RBP + 16]
 mov   qword [RSP + 8 * 8], RAX 
 mov   qword [RSP + 9 * 8], welcomeTextId 
 mov   RAX, qword [REL hInstance] 
 mov   qword [RSP + 10 * 8], RAX
 mov   qword [RSP + 11 * 8], NULL
 call  CreateWindowExA
 mov   qword [REL welcomeText], RAX

 mov   ECX, 20                                  ; Size
 xor   EDX, EDX
 xor   R8D, R8D 
 xor   R9D, R9D
 mov   qword [RSP + 4 * 8], 400                 ; Weight
 mov   qword [RSP + 5 * 8], NULL
 mov   qword [RSP + 6 * 8], NULL
 mov   qword [RSP + 7 * 8], NULL
 mov   qword [RSP + 8 * 8], ANSI_CHARSET
 mov   qword [RSP + 9 * 8], OUT_DEFAULT_PRECIS
 mov   qword [RSP + 10 * 8], CLIP_DEFAULT_PRECIS
 mov   qword [RSP + 11 * 8], PROOF_QUALITY
 mov   qword [RSP + 12 * 8], DEFAULT_PITCH
 lea   RAX, [REL SegoeUI]
 mov   qword [RSP + 13 * 8], RAX
 call  CreateFontA
 mov   qword [REL Font], RAX

 mov   RCX, qword [REL welcomeText]
 mov   EDX, WM_SETFONT
 mov   R8, qword [REL Font]
 xor   R9D, R9D
 call  SendMessageA
 xor     rcx, rcx
 lea     rdx, [REL buttonClass]    ; lpClassName
 lea     r8, [REL showButtonText]          ; lpWindowName
 mov     r9d, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON    ; Styles
 mov     qword [rsp + 4*8], 10          ; X
 mov     qword [rsp + 5*8], 10          ; Y
 mov     qword [rsp + 6*8], 100         ; Width
 mov     qword [rsp + 7*8], 100          ; Height
 mov     rax, qword [hWnd]              ; hWnd (identificador da janela)
 mov     qword [rsp + 8*8], rax
 mov     qword [rsp + 9*8], showButtonId   ; Suponha que você tenha definido showButtonId
 mov     rax, qword [REL hInstance]
 mov     qword [rsp + 10*8], rax
 mov     qword [rsp + 11*8], NULL
 call    CreateWindowExA

 mov   RCX, qword [REL buttonClass]
 mov   EDX, WM_SETFONT
 mov   R8, qword [REL Font]
 xor   R9D, R9D
 call  SendMessageA

 jmp   Return.WM_Processed
 
WMCTLCOLOREDIT:                                 ; For colouring edit controls
 mov   RCX, qword [lParam]                      ; [RBP + 40]
 call  GetDlgCtrlID                             ; RAX = ID

 cmp   RAX, Edit1ID
 je    .Edit1

 cmp   RAX, Edit2ID
 je    .Edit2

.Default:
 mov   ECX, NULL_BRUSH
 call  GetStockObject                           ; Return a brush
 jmp   Return

.Edit1:
 mov   RCX, qword [wParam]                      ; [RBP + 32]
 mov   EDX, dword [REL Edit1TextColour]
 call  SetTextColor

 mov   RCX, qword [wParam]                      ; [RBP + 32]
 mov   EDX, OPAQUE
 call  SetBkMode

 mov   RCX, qword [wParam]                      ; [RBP + 32]
 mov   EDX, dword [REL Edit1BackColour]
 call  SetBkColor

 mov   ECX, NULL_BRUSH
 call  GetStockObject                           ; Return a brush
 jmp   Return

.Edit2:
 mov   RCX, qword [wParam]                      ; [RBP + 32]
 mov   EDX, dword [REL Edit2TextColour]
 call  SetTextColor

 mov   RCX, qword [wParam]                      ; [RBP + 32]
 mov   EDX, OPAQUE
 call  SetBkMode

 mov   RCX, qword [wParam]                      ; [RBP + 32]
 mov   EDX, dword [REL Edit2BackColour]
 call  SetBkColor

 mov   ECX, NULL_BRUSH
 call  GetStockObject                           ; Return a brush
 jmp   Return

WMCTLCOLORSTATIC:                               ; For colouring static controls
 mov   RCX, qword [lParam]                      ; [RBP + 40]
 call  GetDlgCtrlID                             ; RAX = ID

 cmp   RAX, welcomeTextId
 je    .welcomeText

.Default:
 mov   ECX, NULL_BRUSH
 call  GetStockObject                           ; Return a brush
 jmp   Return


.welcomeText:
 mov   RCX, qword [wParam]                      ; [RBP + 32]
 mov   EDX, dword [REL welcomeTextColour]
 call  SetTextColor

 mov   RCX, qword [wParam]                      ; [RBP + 32]
 mov   EDX, OPAQUE
 call  SetBkMode

 mov   RCX, qword [wParam]                      ; [RBP + 32]
 mov   EDX, [REL welcomeTextBackColour]
 call  SetBkColor

 mov   ECX, NULL_BRUSH
 call  GetStockObject                           ; Return a brush
 jmp   Return
WMDESTROY:
 mov   RCX, qword [REL BackgroundBrush]
 call  DeleteObject

 mov   RCX, qword [REL Font]
 call  DeleteObject

 xor   ECX, ECX
 call  PostQuitMessage
 jmp   Return.WM_Processed

WMPAINT:
 mov   RCX, qword [hWnd]                        ; [RBP + 16]
 lea   RDX, [ps]                                ; [RBP - 80]
 call  BeginPaint
 mov   qword [hdc], RAX                         ; [RBP - 8]                              ; Blit a black rectangle

 mov   RCX, qword [hWnd]                        ; [RBP + 16]
 lea   RDX, [ps]                                ; [RBP - 80]
 call  EndPaint

Return.WM_Processed:
 xor   EAX, EAX                                 ; WM_ has been processed, return 0

Return:
 mov   RSP, RBP                                 ; Remove the stack frame
 pop   RBP
 ret
