# 项目说明

基于小熊派/金葫芦开发板的学习仓库，保存 2022 年秋季学期学习《嵌入式系统设计与开发》课程的学习代码和实验代码。

## 文件命名规则

带有 BEAR 表示小熊派的工程项目，其余的基本都是金葫芦的工程项目。

# WEEK08_GPIO_LIGHT

实现目标：通过 GPIO 轮询实现 LED 交替闪烁（绿、蓝）。

开发板：STM32L431RTC6（金葫芦）

实验结果：认识 GPIO，一个基本的输入输出，可以获取引脚上的状态或设置引脚所对应设备的状态。

实验备注：2022 年秋季学期第八周实验一第一部分。CubeMX 设置 LED 的引脚（P8、P9）

# WEEK08_BEAR_GPIO_LIGHT

实现目标：通过 GPIO 轮询实现 S3、S4 按键控制 LED 的闪烁。

开发板：STM32L431RTC6（小熊派）

实验结果：认识 GPIO，一个基本的输入输出，可以获取引脚上的状态或设置引脚所对应设备的状态。

实验备注：2022 年秋季学期第八周实验一第二部分。CubeMX 设置 S3（PB2）、S4（PB3）、LED（PC13）的引脚。

# WEEK08_BEAR_INTER_GPIO_LIGHT

实现目标：通过中断实现 S3、S4 按键控制 LED 灯的状态。

开发板：STM32L431RTC6（小熊派）

实验结果：中断就是异常，中断的优先级高于 MCU 正在运行的正常程序，当一个中断产生时，MCU 会优先处理中断，如果处理成功就回到原来的程序中继续执行。

实验备注：2022 年秋季学期第八周实验一第三部分。CubeMX 设置 S3（PB2）、S4（PB3）、LED（PC13）的引脚。可以在 main.c 中些中断回调函数，也可以在 stm32l4xx_it.c 中写中断代码。

> 中断 <=> 异常。每次都对这种区别不大的名词刨根问底的话，我认为这对初学者来说是非常浪费时间的行为，与其去理解名词之间的本质区别，不如多花点时间去探究为什么、怎么做。

# WEEK09_UART

实现目标：通过轮询实现串口通信。

开发板：STM32L431RTC6（金葫芦）

实验结果：通过串口调试助手，PC 发送数据，MCU 能够收到并转发消息给 PC。

实验备注：2022 年秋季学期第九周实验二第一部分。使用 HAL_UART_Receive 和 HAL_UART_Transmit 函数。

# WEEK09_UART_INTER

实现目标：通过中断实现串口通信。

开发板：STM32L431RTC6（金葫芦）

实验结果：发送或接收，MCU 亮蓝灯（PB9）

实验备注：2022 年秋季学期第九周实验二第二部分。
