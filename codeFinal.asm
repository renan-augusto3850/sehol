extern GetStdHandle
extern WriteConsoleA
section .data
    message db "Hello, World!", 0

section .text
    global Start

Start:
    ; Set up registers for Windows API calls
    mov rdx, message    ; Load the address of the message
    call print_string  ; Call our custom print_string function

    ; Exit the program
    mov eax, 0          ; Return 0
    ret

print_string:
    ; Print a null-terminated string pointed to by RDX
    ; Uses Windows API WriteConsoleA
    push rdi            ; Preserve registers
    push rsi
    push rcx
    mov rsi, rdx        ; RSI = message address
    mov rdi, -11        ; STD_OUTPUT_HANDLE
    call GetStdHandle   ; Get handle to standard output
    mov rdi, rax        ; RDI = handle
    mov rcx, -1         ; Number of characters (-1 for null-terminated)
    call WriteConsoleA  ; Call Windows API function
    pop rcx             ; Restore registers
    pop rsi
    pop rdi
    ret

section .bss
    ; Reserve space for uninitialized data (not used in this example)
