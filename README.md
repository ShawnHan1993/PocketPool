# PocketPool 2.0
v2.0 传完了 更新如下：
1. 移除所有opencv相关的代码（2.0可以直接在UI主管电脑上运行 不用注释掉任何代码）
2. 保留Tracker类和startTracking函数， 函数里面留了一句设置击打速度（5，2）
3. 现在游戏主逻辑调用tracking函数是问询制，每隔50ms询问Tracker的tracking值是否为2，2代表track结束，1代表正在track。也可以改为单次调用取返回值，直接在游戏逻辑里面把timer关掉就好。
4. 清除原有杂乱的debug信息，现在debug信息会显示游戏所处的状态，哪个球进了哪个洞
