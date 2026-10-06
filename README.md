# F103_Fan_PID

基于 STM32F103C8T6 的闭环调速与自动摇头智能风扇控制系统。

## 项目简介

本项目以 STM32F103C8T6 为核心控制器，设计并实现了一套具备多挡位风速调节、编码器闭环 PID 调速、SG90 舵机摆头以及 OLED 实时参数显示的智能桌面风扇系统。同时支持通过串口对接 VOFA+ 上位机进行 PID 参数调谐与波形监控。

## 功能特性

- **多挡位调节**:
  - 支持 0~3 挡多级风速切换与一键启停。
- **PID 闭环速度控制**:
  - 通过定时器编码器接口精确采集电机实时转速（Cur Speed）。
  - 内置位置式/增量式 PID 算法动态调节电机 PWM 占空比，抗负载扰动能力强，转速稳定。
- **SG90 舵机摆头交互**:
  - 控制风扇左右往复摆头，支持随时开启/关闭摇头功能。
- **OLED UI 实时监测**:
  - 0.96 寸 I2C OLED 屏显示当前挡位（Level）、摇头状态（Round ON/OFF）、PID 增益参数（Kp/Ki/Kd）以及目标转速与实际转速。
- **VOFA+ 串口调试**:
  - 串口输出转速与控制量，方便使用上位机绘制响应曲线，快速完成 PID 参数整定。

## 硬件与外设

- **主控芯片**: STM32F103C8T6 (ARM Cortex-M3, 72MHz)
- **电机与驱动**: 直流电机 + TB6612FNG 驱动模块
- **测速反馈**: AB 相正交编码器 (TIM2/TIM3 编码器模式)
- **舵机模块**: SG90 模拟舵机 (TIM PWM 驱动)
- **显示外设**: 0.96 寸 I2C OLED 屏幕
- **上位机接口**: USART1 串口通信

## 目录结构

- `Core/`: 系统时钟配置、外设底层初始化 (`gpio.c`, `tim.c`, `i2c.c`, `usart.c`) 与主循环
- `MiddleDriver/`:
  - `PID.c / .h`: PID 控制算法实现
  - `Encoder.c / .h`: 编码器测速与脉冲计数
  - `motor.c / .h`: 直流电机方向与 PWM 驱动
  - `SG09.c / .h`: 舵机角度及摆头逻辑控制
  - `Menu.c / .h`: OLED 菜单界面与按键状态机
  - `OLED.c / .h`: 屏幕绘制与字符显示
  - `VOFA.c / .h`: VOFA+ 数据帧封装与串口输出
- `Drivers/`: STM32F1xx HAL 库与 CMSIS 固件
- `MDK-ARM/`: Keil MDK 工程文件 (`Test0.uvprojx`)

## 开发环境

- **IDE**: Keil MDK-ARM (v5 及以上)
- **配置工具**: STM32CubeMX (`Test0.ioc`)
- **工程文件**: `MDK-ARM/Test0.uvprojx`
