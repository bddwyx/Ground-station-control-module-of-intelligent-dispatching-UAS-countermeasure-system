# Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system

# 智能调度无人机地面站控制模块

## 总述 Description
A control system for ground stations that serve as the central processing unit of UAS contermeasurement.

一个为无人空中系统集群的中央处理单元设计的地面站控制系统。

For version v1.0, we adopted Arduino Uno temporarily. Nucleo L432 based on STM32 L4 series may be adopted for further improvement for finer control and more complicated functions.

在版本1.0中，我们暂时采用Arduino Uno作为主控。为实现更精细的控制与更复杂的功能，后续可能采用基于STM32 L系列的Nucleo L432。

## 通讯协议 Communication

### 下位机控制指令 Control commands on lower computer(MCU)

The lower computer, that is, the monitoring system of the ground station based on Doppler radar, provides interfaces for yaw angle control, pitch angle control, stalling, and automatic manual switching.

下位机，即基于多普勒雷达的地面站的监测系统，提供了偏航角控制，俯仰角控制，停转与自动手动切换等接口。

The lower computer is divided into automatic cruise mode and manual mode. In automatic cruise mode, the radar will continuously scan 360 full range and return readings; in manual mode, if no control command is sent, the radar will stop at the last position and continue to detect until the control command is entered into automatic cruise mode or executed Other instructions.

下位机分为自动巡航模式和手动模式。自动巡航模式下，雷达会不间断地360全范围扫描，返回读数；手动模式下，若不发送控制指令，雷达会停在上一次的位置持续探测，直到收到控制指令进入自动巡航模式或执行其他指令。

The lower computer adopts UART protocol, 1+8+0+1, Baud Rate 38400.

下位机采用UART协议，1+8+0+1，波特率38400。

| 数据范围 |  0-19  | 20-21 |   22   |   23   |
| :-----  | :----: | :----: | :----: | :----: |
| 功能    | 雷达偏航角控制 | 雷达俯仰角控制 | 停转指令 | 自动巡航 |
| 备注    | 对360°20均分 | 分为高低两档 |  | 任何其他指令将进入手动模式 |


### 上位机显示指令 Display commands for host computer(PC)

Each data packet of the display command of the host computer is three bytes, the first two bytes are the coordinates of the display screen, and one bit is the radial velocity measured by the Doppler radar.

上位机的显示指令的每个数据包为三个字节，前两个字节为显示屏显示坐标，有一位为多普勒雷达测量得到的径向速度。

| 数据序号 | 0 | 1 | 2 |
| :-----  | :----: | :----: | :----: |
| 类型    | 8位整形 | 8位整形 | 8位无符号整形 |
|   描述  | 显示坐标x | 显示坐标y | 径向速度绝对值 |
|   备注  | 应当为正值 | 应当为负值 |   |

### 四旋翼控制指令 Quadrotor's commands

The quadrotor control command packet is six bytes, which is received by the SPI protocol through the NRF24L01 2.4GHz wireless module. The data at the transmitting end can use UART protocol, 1+8+0+1, Baud Rate 115200.

四旋翼的控制指令包为六个字节，通过NRF24L01这款2.4GHz无线模块采用SPI协议接收。发射端的数据可采用UART协议,1+8+0+1，波特率115200。 

| 数据序号 | 0 | 1 | 2 | 3 | 4 | 5 |
| :-----  | :----: | :----: | :----: | :----: | :----: | :----: |
| 类型    | 8位无符号整形 | 8位无符号整形 | 8位无符号整形 | 8位无符号整形 | 8位无符号整形 | 8位无符号整形 |
|   描述  | 油门大小 | 偏航角控制 | 滚转角控制 | 俯仰角控制 | 功能按键 | 校验和 |
|   备注  | 升降控制 |  运动方向  |  左右平移 |  前后平移  | 模式切换 | 前五位数据的和 |

## 函数接口 API

## 全局变量 Global Variables

## 效果图 Renderings
