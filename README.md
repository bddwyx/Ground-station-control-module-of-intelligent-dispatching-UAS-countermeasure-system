# Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system

# 智能调度无人机地面站控制模块

## 总述 Description
A control system for ground stations that serve as the central processing unit of UAS contermeasurement.

一个为无人空中系统集群的中央处理单元设计的地面站控制系统。

For version v1.0, we adopted Arduino Uno temporarily. Nucleo L432 based on STM32 L4 series may be adopted for further improvement for finer control and more complicated functions.

在版本1.0中，我们暂时采用Arduino Uno作为主控。为实现更精细的控制与更复杂的功能，后续可能采用基于STM32 L系列的Nucleo L432。

![整体设计渲染图](https://github.com/bddwyx/Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system/blob/master/%E4%B8%8A%E5%9C%BA%E5%9B%BE1.JPG "整体设计渲染图")

## 无人机反制概述 Overview of UAS countermeasure

With the development of UAS industry, the potential safety hazard of some UASs flying illegally in some specific areas is becoming increasingly prominent and it is urgent to strengthen the management of civil UAS equipment.

随着无人机行业的发展,部分无人机的违规飞行对一些特定地区带来的安全隐患日益突出，加强对民用无人机设备的管理已成为迫切需求。

In the rectification of the illegal flight behavior of UAS, UAS counter control is a very effective measure.

而在对无人机违规飞行行为的整治中，无人机反制是一种十分有效的措施。

The so-called UAS counter control refers to the use of counter equipment to force the long-distance and low altitude UAS to fall, then make UAS return to the operator or descend steadily in a short time, so as to realize the effective control of UAS flying in "control area".

所谓无人机反制，指的就是利用反制设备，在短时间内迫使远距离低空无人机跌落、返回操作者处或平稳下降，进而实现对“管制区”飞行无人机的有效管控。

For UAS countermeasures, the traditional countermeasures are usually electromagnetic interference gun countermeasures, that is, by transmitting high-energy electromagnetic waves of 2.4GHz or 5.8GHz frequency band to directionally interfere with the specific control signal of UAS, or through GPS suppression jamming and deceptive jamming to affect the positioning of UAS, so as to force UAS to land, and achieve the purpose of UAS countermeasures in the end.

对于无人机反制，传统的反制措施通常为电磁干扰枪反制，即通过发射高能的2.4GHz或5.8GHz等频段的电磁波来定向干扰无人机的特定控制信号，或通过GPS的压制式干扰和欺骗式干扰对无人机的定位产生影响，以迫使无人机降落，从而达到无人机反制的目的。

However, the traditional electromagnetic interference countermeasures have security risks and limitations of application scenarios. Because the UAV without location information has a certain probability of crash, and the high altitude crash UAV will cause a great security threat to the people below, and due to the limitation of its interference mode, the equipment is difficult to take effective countermeasures against the UAV designed for specific purposes, so the countermeasures are not fully suitable for the security work in crowded or important places.

然而，传统的电磁干扰反制方式存在安全隐患与应用场景的限制。因为失去位置信息的无人机具有一定的坠毁概率，高处坠毁的无人机将会对下方的人群造成极大的安全威胁，并且由于其干扰方式的限定，设备难以对出于特定目的设计的无人机采取有效的反制措施，所以该反制方式并不完全适合人群密集或重要地点的安防工作。

Moreover, in some special occasions, such as airports, the interference signals sent by the counter equipment may affect the normal landing and taking off of airport aircraft.

而且，在一些特定场合，比如机场等，反制设备发出的干扰信号可能影响机场飞机的正常起落。

In addition to the above countermeasures against electromagnetic interference guns, countries around the world have also tried other countermeasures.

除了上述提到的电磁干扰枪反制，世界各国也尝试了其他的反制措施。

For example, in 2016, the Dutch police have trained eagles to carry out capture type UAV countermeasures, and achieved certain results in practice; the Japanese government has also established a "UAV hunting team" to capture some non-standard flying UAVs through the way of netting; Wang Hongyu, associate professor of Shanghai Jiaotong University, also put forward the concept of Net Countermeasures of UAV and prototype are produced, which are reflected in CCTV's "smart man" program.

例如，在2016年，荷兰警方已经通过训练老鹰的方式进行捕获式无人机反制，并在实践中取得了一定的成效；日本政府也曾通过成立“无人机捕猎小队”的方式通过拉网的方式捕获一些不规范飞行的无人机；上海交通大学的王红雨副教授也提出了无人机网捕式反制的概念并制作出样机，并在央视《机智过人》节目中有所体现。

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
| 功能    | 雷达偏航角yaw控制 | 雷达俯仰角pitch控制 | 停转stop指令 | 自动巡航auto |
| 备注    | 对360°进行20均分 | 分为高upper低horizontal两档 |  | 任何其他指令将进入手动manual模式 |

The PID of the system is shown in the figure below.

系统的PID效果图如下图所示。

![PID效果图](https://github.com/bddwyx/Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system/blob/master/PID%E6%95%88%E6%9E%9C%E5%9B%BE.jpg)
![PID效果图2](https://github.com/bddwyx/Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system/blob/master/PID%E6%95%88%E6%9E%9C%E5%9B%BE2.png)

### 上位机显示指令 Display commands for host computer(PC)

Each data packet of the display command of the host computer is three bytes, the first two bytes are the coordinates of the display screen, and one bit is the radial velocity measured by the Doppler radar.

上位机的显示指令的每个数据包为三个字节，前两个字节为显示屏显示坐标，有一位为多普勒雷达测量得到的径向速度。

| 数据序号 | 0 | 1 | 2 |
| :-----  | :----: | :----: | :----: |
| 类型    | 8位整形int8_t | 8位整形int8_t | 8位无符号整形uint8_t |
|   描述  | 显示坐标x | 显示坐标y | 径向速度$v_r$绝对值 |
|   备注  | 应当为正值pos | 应当为负值neg |   |

### 四旋翼控制指令 Quadrotor's commands

The quadrotor control command packet is six bytes, which is received by the SPI protocol through the NRF24L01 2.4GHz wireless module. The data at the transmitting end can use UART protocol, 1+8+0+1, Baud Rate 115200.

四旋翼的控制指令包为六个字节，通过NRF24L01这款2.4GHz无线模块采用SPI协议接收。发射端的数据可采用UART协议,1+8+0+1，波特率115200。 

| 数据序号 | 0 | 1 | 2 | 3 | 4 | 5 |
| :-----  | :----: | :----: | :----: | :----: | :----: | :----: |
| 类型    | 8位无符号整形uint8_t | 8位无符号整形uint8_t | 8位无符号整形uint8_t | 8位无符号整形uint8_t | 8位无符号整形uint8_t | 8位无符号整形uint8_t |
|   描述  | 油门大小acc | 偏航角控制yaw | 滚转角控制roll | 俯仰角控制pitch | 功能按键key | 校验和sum |
|   备注  | 升降控制 |  运动方向  |  左右平移 |  前后平移  | 模式切换 | 前5位数据的和 |

## 函数接口 API

* int read_position(void）

描述 description：The raw data of the angle reading of the magnetic encoder is read through the IIC protocol. 通过IIC协议读取磁编码器的角度读数的原始数据。   
参数 parameter：   
返回值 return value：Type uint12, which divides the angle range of 0-360 degrees into 4096 parts, and evenly maps to 0-4095. 类型uint12，将0-360度的角度范围分为4096份，均匀映射到0-4095。   

* int  move_to_yaw(int)

描述 description：Point thr radar at the desired position. 使雷达转动到对应角度。   
参数 parameter：uint8, valid input shall be in range 0 to 19. uint8 ，合法的输入值需要在0-19之间。   
返回值 return value：return 1 when a input is valid and a successful move is done, return 0 if the parameter is invalid. 输入合法时返回1，无效输入则返回-1，并且不修改雷达目标角度。   

* void stop_and_read(void)

描述 description：print the velocity value radar detected while stop it。 使得雷达停在当前角度测量并打印出雷达读数。   
参数 parameter：   
返回值 return value：   

## 全局变量 Global Variables

## 效果图 Renderings

![设计渲染图](https://github.com/bddwyx/Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system/blob/master/%E6%B8%B2%E6%9F%933.JPG)
![设计渲染图](https://github.com/bddwyx/Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system/blob/master/%E6%B8%B2%E6%9F%934.JPG)
![设计渲染图](https://github.com/bddwyx/Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system/blob/master/%E4%BF%AF%E8%A7%86%E5%9B%BE%E6%B8%B2%E6%9F%93.JPG)
![爆炸渲染图](https://github.com/bddwyx/Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system/blob/master/%E7%88%86%E7%82%B8%E6%B8%B2%E6%9F%933.JPG)
![爆炸渲染图](https://github.com/bddwyx/Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system/blob/master/%E7%88%86%E7%82%B8%E6%B8%B2%E6%9F%932.JPG)
![爆炸渲染图](https://github.com/bddwyx/Ground-station-control-module-of-intelligent-dispatching-UAS-countermeasure-system/blob/master/%E4%BF%AF%E8%A7%86%E7%88%86%E7%82%B8%E5%9B%BE%E6%B8%B2%E6%9F%93.JPG)
