# uboot201404_for_tiny210


# uboot启动流程分析，针对S5PV210

# BL1阶段，SPL，u-boot-spl.bin

# 1、首先运行arch/arm/cpu/armv7/start.S 里面的_start函数，进行异常向量表设置，然后跳转到reset复位处理函数，设置处理器SVC模式，关闭IRQ和FIQ中断。设置cp15协处理器的SCTRL寄存器V(bit13)为0，
#	 设置异常向量表在0x00000000-0x0000001C，设置异常向量表地址为_start。跳转到cpu_init_cp15初始化协处理器，清除TLB，关闭cache，关闭MMU，如果没有定义CONFIG_SYS_ICACHE_OFF则打开icache。
#	 继续执行cpu_init_crit，跳转到board/samsung/tiny210/lowlevel_init.S里面的lowlevel_init，进行时钟初始化，DDR初始化。
# 2、跳转到arch\arm\lib\crt0.S里面的_main，调用copy_bl2_to_ram把BL2从SD卡或是nand flash拷贝到SDRAM的0x20000000，然后跳转到SDRAM开始运行BL2。


# BL2阶段，u-boot.bin

# 1、还是运行arch/arm/cpu/armv7/start.S 里面的_start函数，然后是reset，再次设置异常向量表地址，设置cp15，跳转到cpu_init_crit，调用lowlevel_init，但没做任何时事情，因为BL1已经做了初始化，
# 	 然后跳转到_main，设置sp，预留空间给全局变量gd，gd在sp之上，gd指针保存在r9里面，跳转到board_init_f执行。
# 2、执行arch\arm\lib\board.c里面的board_init_f，清空gd，设置gd里面变量mon_len为uboot大小，执行init_sequence里面的初始化序列函数，初始化定时器，串口，环境变量，设置dram banks等等。
#	 继续gd的初始化，如果定义了宏CONFIG_SYS_MEM_TOP_HIDE则预留CONFIG_SYS_MEM_TOP_HIDE ram隐藏空间，如果定义了CONFIG_LOGBUFFER，则预留LOGBUFF_RESERVE空间给kernel logbuffer，设置TLB大小和空间，如果定义了
# 	 CONFIG_LCD则预留fb，设置TOTAL_MALLOC_LEN大小为malloc空间，预留空间给bd，预留空间给gd，得到addr_sp为栈指针，addr为uboot起始地址。设置波特率，设置dram地址地址和大小，设置uboot重定位地址为addr，
#	 起始栈指针为addr_sp，设置重定位偏移，把gg数据由当前r9处拷贝到新的gd空间。
# 3、设置新的gd地址到r9，计算重定位后here的地址保存在lr，这样在执行relocate_code进行uboot重定位之后返回时，直接跳转到重定位后的uboot继续执行。设置r0为重定位地址，调用relocate_code进行uboot重定位。
# 4、执行arch\arm\lib\relocater.S里面的relocate_code进行uboot重定位。uboot中ld使用-pie而cc没有使用-fPIC或-fPIE,目标文件中就不会生成GOT。首先拷贝__image_copy_start和__image_copy_end之间的代码到
#	 重定位地址空间，在调整.rel.dyn段里面的地址，对__rel_dyn_start和__rel_dyn_end之间类型为R_ARM_RELATIVE（23）的entry加上偏移进行调整。
# 5、重定位代码后清零bss段，设置r0为r9，即r0指向gd，r1为重定位地址，然后调用board_init_r。
# 6、执行arch\arm\lib\board.c里面的board_init_r继续初始化，设置gd标志为GD_FLG_RELOC，使能ceche，调用board\samsung\tiny210\tiny210.c里面的board_init初始化sromc设置机器类型和启动参数地址，
#	 调用serial_initialize注册串口设备，初始化malloc，初始化nand flash，执行env_relocate设置环境变量，设置中断，初始化网卡，最后跳转到main_loop死循环，在main_loop中会调用process_boot_delay检测启动
#	 阶段有没有键按下，如果有就进入uboot命令行，如果没有就默认加载kernel，如果加载失败也会进入uboot命令行。
#
########################################################################################################################################################################################################
#
# 	 对于代码重定位的一些说明
#
#	 需要注意的是，在uboot的整个relocate_code中rel.dyn不仅没有拷贝，
#	 也没有修改，修改只是针对rel.dyn中值+offset为地址上的值！查阅网上资料，compiler在cc时加入-fPIC或-fPIE选项，会在目标文件中生成GOT（global offset table），将需要relocate的值存放在GOT中，
#	 函数尾部的	Label来存储GOT的offset以及其中变量的offset，变量寻址首先根据尾部Label相对寻址找到GOT地址，以及变量地址在GOT中的位置，从而确定变量地址，这样对于目标文件统一修改GOT中的值，
#	 就修改了变量地址的offset，完成了relocation。ld时加入-pie选项，就会将GOT并入到rel.dyn段中，uboot在relocate_code中统一根据rel.dyn段修改需要relocation的数值。uboot中ld使用-pie而cc没有使用-fPIC或-fPIE,
#    目标文件中就不会生成GOT，函数中寻址还是在尾部Label中直接存储变量的绝对地址，但这个Label同样存在rel.dyn中，uboot根据rel.dyn段修改Label上的值，就完成了relocation。
#    这样不仅节省了每个目标文件的GOT段，而且不需要去相对寻址GOT，直接修改函数尾部Label所存储的变量地址就可以啦！
#
#	 参考博文：http://blog.csdn.net/skyflying2012/article/details/37660265
#			   http://blog.csdn.net/caiyuqing2001/article/details/7328994
########################################################################################################################################################################################################
