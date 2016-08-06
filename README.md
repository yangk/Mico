重要提示: 该版本仅提供相关人员内部测试使用，未经MXCHIP的许可严禁对外发布！

MICO
====

Mico-controller based Internet Connectivity Operation System


### 特性
* 为嵌入式设备设计的物联网开发软件平台
* 基于实时操作系统设计
* 支持各种微控制器平台
* 无线网络接入方案：Wi-Fi，蓝牙
* 内置云服务接入中间件和各种示例代码
* 优秀的低功耗控制功能
* 为物联网产品量产设计的应用程序框架
* 为应用开发提供的大量编程工具

### 目录结构
* Demos: 基于MiCO开发的各种示例程序
* include: MiCO核心功能接口
* mico: MiCO核心库,二进制文件和相关系统代码
* libraries: MiCO软件中间件，应用支持代码等
* Platform: 基于不同开发环境，硬件平台的特性文件
* Board: 板级定制代码
* Projects: 基于不同开发环境的的工程文件

### 使用方法
1. 首先您需要拥有一个MiCOKit开发套件：[MiCOKit-xxx](http://mico.io/wiki/doku.php?id=micokit_overview)<br />
2. 安装IAR workbench for ARM v7.3或者Realview MDK v5.xx版本
3. 使用USB线连接PC和MiCOKit，实现一个虚拟串口用于供电和输出调试信息, 驱动下载地址[VCP driver](http://www.ftdichip.com/Drivers/VCP.htm) 
4. 根据您使用的处理器，从project目录选择正确的工程文件，并打开
5. 根据使用的MiCOKit，在打开的软件工程中选择正确的工程配置目标参数:MiCOKit-xxx
6. 使用开发环境编译，下载并运行示例程序, 并从串口终端上获得程序的调试信息输出

### 在应用中使能Wi-Fi网络
1. 在mico_config.h中打开`<#define MICO_WLAN_CONNECTION_ENABLE>`和`<#define MICO_CONFIG_MODE CONFIG_MODE_EASYLINK>`
2. 在应用程序中开启MiCO核心数据管理`<core_data = mico_system_context_init( 0 )>`
3. 在应用程序中开启MiCO系统功能`<mico_system_init(core_data)>`
4. 编译，下载和运行代码，从设备的调试信息上可以看到进入EasyLink配网模式
5. 下载Wi-Fi网络配置移动端应用程序: [EasyLink](http://mico.io/wiki/doku.php?id=micoappdownload)
6. 在EasyLink应用程序中点击"+"按钮，进入网络配置界面
7. 输入SSID和密码，并点击“Fast mode”，启动EasyLink配网
8. 等待一段时间后，在配网界面下会出现新加入的网络设备

Notice: Internal use only


MICO
====
Mico-controller based Internet Connectivity Operation System


### Feathers
* Designed for embedded devices
* Based on a Real time operation system
* Support abundant MCUs
* Wi-Fi, bluetooth connectivity total solution
* Build-in protocols for cloud service
* State-of-the-art low power management
* Application framework for I.O.T product
* Rich tools and mobile APP to accelerate development

### Contents:
* Demos: Demos applications and application framework
* include: MiCO core APIs
* mico: MiCO core libraries, binary file and system codes
* libraries: MiCO middleware, support functions
* Platform: codes based in different IDE and hardware
* Board: BSP file, Hardware resources and configurations on different boards
* Projects: IAR workbench or Realview MDK projects
* libraries: Open source software libraries
* mico: MiCO's main functions

### How to use:
1. You should have a dev board [MiCOKit-xxx](http://mico.io/wiki/doku.php?id=micokit_overview)
2. Install IAR workbench for ARM v7.3 or higher or Realview MDK v5.xx
3. Connect USB cable to PC to have a virtual serial port, install [VCP driver](http://www.ftdichip.com/Drivers/VCP.htm) 
4. Open any demo project and select target: MiCOKit-xxx
5. Build, download and run MICO demos on MiCOKit, you may find logs display on serial port

### Enable Wi-Fi in application
1. Enble macro in mico_config.h: `<#define MICO_WLAN_CONNECTION_ENABLE>`和`<#define MICO_CONFIG_MODE CONFIG_MODE_EASYLINK>`
2. Initialize mico care data management:`<core_data = mico_system_context_init( 0 )>`
3. Initialize mico system functions`<mico_system_init(core_data)>`
4. Build, download and run,编译，device will enter EasyLink configuration mode
5. Download and Wi-Fi configuration APP: [EasyLink](http://mico.io/wiki/doku.php?id=micoappdownload)
6. Press "+" button on Easylink APP
7. Input ssid and password of the wlan that assoiated to iPhone, and start easylink
8. A new device appears on App after a while

