# 项目说明

基于小熊派/金葫芦开发板的学习仓库。保存 2022~2023 年学期《嵌入式系统设计与开发》课程的学习代码和实验代码。

# WEEK08_GPIO_LIGHT

实现目标：通过 GPIO 轮询实现 LED 交替闪烁（绿、蓝）。

开发板：STM32L431RTC6（金葫芦）

# WEEK08_BEAR_GPIO_LIGHT

实现目标：通过 GPIO 轮询实现 S3、S4 按键控制 LED 的闪烁。

开发板：STM32L431RTC6（小熊派）

# WEEK08_BEAR_INTER_GPIO_LIGHT

实现目标：通过中断实现 S3、S4 按键控制 LED 灯的状态。

开发板：STM32L431RTC6（小熊派）

中断就是异常，中断的优先级高于 MCU 正在运行的正常程序，当一个中断产生时，MCU 会优先处理中断，如果处理成功就回到原来的程序中继续执行。

# WEEK09_UART

实现目标：通过轮询实现串口通信。

开发板：STM32L431RTC6（金葫芦）

# WEEK09_UART_INTER

实现目标：通过中断实现串口通信。

开发板：STM32L431RTC6（金葫芦）

1. 正常接收和转发数据；
2. 通过字符发送对 LED 进行控制：

   - 发送字符串 ON => LED 亮；
   - 发送字符串 OFF => LED 灭；
   - 发送字符串 SHAKING => LED 闪烁；

# WEEK11_ADC

开发板：STM32L431RTC6（金葫芦）

# WEEK11_DAC

开发板：STM32L431RTC6（小熊派）

# WEEK12_TIMER_FLASH

基于金葫芦实现读取/写入 FLASH 的功能。

# WEEK13_BEAR_SPI_I2C

读取光照传感器数据，控制小熊派开发板上的 LED（PC13），控制智慧农业板上的灯（PB9）。
