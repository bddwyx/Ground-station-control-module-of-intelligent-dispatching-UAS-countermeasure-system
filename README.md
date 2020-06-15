# Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system

# 智能调度无人机地面站控制模块

## 总述 Description
A control system for ground stations that serve as the central processing unit of UAS contermeasurement.

一个为无人空中系统集群的中央处理单元设计的地面站控制系统。

For version v1.0, we adopted Arduino Uno temporarily. Nucleo L432 based on STM32 L4 series may be adopted for further improvement for finer control and more complicated functions.

在版本1.0中，我们暂时采用Arduino Uno作为主控。为实现更精细的控制与更复杂的功能，后续可能采用基于STM32 L系列的Nucleo L432。

## 通讯协议 Communication

### 下位机控制指令 Control commands on lower computer(MCU)

```mermaid
graph LR
A(上位机) -->B(雷达)
```

| 数据范围 |  0-19  | 20-21 |   22   |   23   |
| :-----  | :----: | :----: | :----: | :----: |
| 功能    | 雷达偏航角控制 | 雷达俯仰角控制 | | |
| 备注    | 对360°20均分 | 分为高低两档 | | |

### 上位机显示指令 Display commands for host computer(PC)

## 函数接口 API

## 全局变量 Global Variables

## 效果图 Renderings
