[(23条消息) 详细介绍如何在ubuntu20.04中安装ROS系统，以及安装过程中出现的常见错误的解决方法，填坑！！！_慕羽★的博客-CSDN博客_ubuntu20.04安装教程](https://blog.csdn.net/qq_44339029/article/details/108919545)



上面教程走到 “sudo rosdep init” 因为网络被墙就进行不了，可以通过下方 “鱼香ROS” 的一键安装 “rosdepc”

[(23条消息) 鱼香ROS网站上线|一行代码安装ROS/ROS2/解决rosdep问题|小鱼脚本_生活不止眼前的苟且-CSDN博客_鱼香ros网站](https://blog.csdn.net/qq_27865227/article/details/120277420)

安装好 “rosdepc” 后再用

```c
sudo rosdepc init
```

```c
rosdepc update
```

后面继续上面的教程走

