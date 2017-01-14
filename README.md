在学生时代，操作系统对于我而言是一个很神秘的东西，虽然从书本中了解了一些操作系统的基本原理， 但总缺少一些直观的认识。
硕士毕业那年，机缘巧合地看到了于渊写的《Orange'S一个操作系统的实现》这本奇书， 得以管窥一二。
感觉写一个操作系统应该是一件很酷的事情，而且也不是那么难，只要对计算机结构有清晰的认识， 就应该没有问题。
但是，X86这个架构对于我而言有点复杂了，我需要花费很多精力去了解各种各样的历史原因， 有点麻烦。
而一个嵌入式的系统，我可以自由的把控各个系统外设，必要时可以自定义，是一个很不错的起点。
于是，我决定写一个嵌入式的操作系统。

这个系列的文章从建立一个精简的可以正常编译的工程项目开始，深入到计算机和操作系统的各个角落。
我会逐渐的添加代码，操作处理器内核、驱动STM32的各种外设，最后将得到一个简单的操作系统。

选用的开发板是淘宝上买的银杏科技的iCore3，板上有一颗STM32F407IGT6的MCU，和一颗EP4CE10F17C8N的FPGA。
之所以选择这块板子是因为它有一颗FPGA，为后续的功能扩展提供了很多可能。

针对STM32，我们先在Windows环境下使用IDE入门，以后会在Linux下搭建一个完整的工具链。
使用的IDE是德国的KEIL公司开发的μVision，至于如何安装使用，如何建立工程文件，这里就不再赘述了。


系统时钟计算方法：
f(VCO clock) = f(PLL clock input) * PLLN / PLLM
f(PLL general clock output) = f(VCO clock) / PLLP
f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ

PLLI2S时钟计算方法：
f(VCO clock) = f(PLLI2S clock input) * PLLI2SN / PLLM
f(PLLI2S clock output) = f(VCO clock) / PLLI2SR

