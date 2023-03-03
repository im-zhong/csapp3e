# main.o 和 prog 里面的函数区别
1. `objdump -dx main.o`
`
0000000000000000 <main>:
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   48 83 ec 10             sub    $0x10,%rsp
   c:   be 02 00 00 00          mov    $0x2,%esi
  11:   48 8d 05 [00 00 00 00]    lea    0x0(%rip),%rax        # 18 <main+0x18>
                        14: R_X86_64_PC32       array-0x4
  18:   48 89 c7                mov    %rax,%rdi
  1b:   e8 [00 00 00 00]          call   20 <main+0x20>
                        1c: R_X86_64_PLT32      sum-0x4
  20:   89 45 fc                mov    %eax,-0x4(%rbp)
  23:   8b 45 fc                mov    -0x4(%rbp),%eax
  26:   c9                      leave
  27:   c3                      ret
`
2. `objdump -d prog`
`
0000000000001129 <main>:
    1129:       f3 0f 1e fa             endbr64
    112d:       55                      push   %rbp
    112e:       48 89 e5                mov    %rsp,%rbp
    1131:       48 83 ec 10             sub    $0x10,%rsp
    1135:       be 02 00 00 00          mov    $0x2,%esi
    113a:       48 8d 05 [cf 2e 00 00]    lea    0x2ecf(%rip),%rax        # 4010 <array>, 这个地址也是pc relative 1141 + 2ecf = 4010
    1141:       48 89 c7                mov    %rax,%rdi
    1144:       e8 [08 00 00 00]          call   1151 <sum>
    1149:       89 45 fc                mov    %eax,-0x4(%rbp)
    114c:       8b 45 fc                mov    -0x4(%rbp),%eax
    114f:       c9                      leave
    1150:       c3                      ret
`
1. 首先我们可以非常明显的看到 再重定位之前main函数的起始地址是零，call指令的目标地址是零；在重定位之后，main函数的起始地址编了，call指令的目标地址也被填充好了
2. 使用 objdump -D prog 可以看到array的地址
0000000000004010 <array>:
    4010:       01 00                   add    %eax,(%rax)
    4012:       00 00                   add    %al,(%rax)
    4014:       02 00                   add    (%rax),%al
        ...