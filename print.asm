printf@plt-0x10:
 push   QWORD PTR [rip+0x2fca]        # 403ff0 <_GLOBAL_OFFSET_TABLE_+0x8>
 jmp    QWORD PTR [rip+0x2fcc]        # 403ff8 <_GLOBAL_OFFSET_TABLE_+0x10>
 nop    DWORD PTR [rax+0x0]
main:
 push   rbp
 mov    rbp,rsp
 mov    edi,0x402004
 mov    eax,0x0
 call   401030 <printf@plt>
 mov    eax,0x0
 pop    rbp
 ret