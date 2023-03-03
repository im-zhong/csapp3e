// 2023/3/3
// zhangzhong
// todo: 这个值得研究 这怎么会内存出错呢???

// p -> x
extern void x();

void p() { x(); }

int main() { p(); }
