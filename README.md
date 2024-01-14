
# Project4-Virtual_Memory_Management

## Test Sets

`waitpid ready_to_exit wait_locks`：启动进程，拿锁，等待，退出测试

`barrier`：屏障测试（三个进程都到达屏障才可有下一步动作）

`sema`：信号量测试（三个生产者，三个消费者，必须等待到进行消费，消费者才可以进入临界区消费）

`mbox`:  信箱测试（一个生产者传输字符，可以启动多个进程接受字符，本质在于写管程）

`multicore`：双核性能测试，希望能达到接近2的加速比

`affinity`:  绑核测试，希望能看到明显的速度差异

## Debug

P3同步操作的debug部分并不算很复杂，其中较为复杂的在于启动进程以及双核的相关debug

1. 注意到现在有大于16个进程，因此tasks和pcb数组需要扩容，可以扩容到32个
2. exec进行命令行参数传参时，容易出错，需要注意相关sp的设置
3. 关于shell进程，由于是单纯的软件，因此bug都非常容易定位，不过多赘述
4. 在同步相关操作的设计中，我所采用的设计都是mesa风格，所有的同步量都用自旋锁实现，包括互斥锁，屏障，信号量，信箱等。由于最开始我尝试过小锁的设计，因此可以很明显的在我的代码中看到小锁的痕迹。在代码中，我采用mesa风格，即对所有的临界区，我首先用配套的自旋锁将其保护起来，在代码的中途如果出现需要等待的情况，则调用do_block函数将其加入到对应的队列中，并且将这把小锁给释放掉。我认为这种风格可以让代码更加程式化和简洁，希望对做小锁的学弟学妹能有所帮助

#### 关于双核

在双核的设计过程中（这里只说大内核锁），有三个比较重要的点需要注意：

1. 对整个内核的上锁，我的方案是在保存上下文的地方上锁，在entry.S中可以看到
2. 关于双核的唤醒，我采用的方式时给所有的和发中断，但注意主核一点要将自己的中断信号给清除掉，不然会报错（也可用掩码进行设置）
3. current_running，我采用的方案是让其成为一个指针，每次进程修改时让其指针指向的地方进行修改。注意current_running是不能保存在栈中的，因为它代表的是当前哪个核在运行这段代码，可能同一段代码却是由两个核来运行的。因此current_running必须要即时获得

以上所述均是大内核锁的设计，双核的debug是较为痛苦的，有时候得用printl打印很多内容才能发现问题，gdb有时候也会出现从0核运行的位置跳到1核运行的位置，还需仔细进行甄别。关于细粒度锁的设计，就爱莫能助了。
