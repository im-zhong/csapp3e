我想到了一个非常有趣的lab
我先用有缺陷的gets函数实现一个简单的网络服务器，他需要一个密码，如果输入正确，就返回正确
如果输入错误，就返回错误。
但是这个密码被我放在文件系统里，路径什么的都告诉你
你需要写一个攻击程序，利用缓冲溢出，执行你特殊的代码，获得这个文件，从而得到密码
输入密码即可！
really fun