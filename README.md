# 基于 ARM-Linux 的智能家居系统

基于 i.MX6ULL 平台的嵌入式 Linux 智能家居项目，涵盖多种传感器/执行器的字符设备驱动开发（I2C、GPIO、中断子系统）、设备树配置、QT 5.12 交叉编译移植与LCD触摸屏应用开发，以及蓝牙远程控制。

## 硬件平台

| 项目 | 说明 |
|------|------|
| 开发板 | 百问网 IMX6ULL Pro（i.MX6ULL Cortex-A7, 512MB DDR3L, 4GB eMMC） |
| 内核版本 | Linux 4.9.88 |
| 驱动编译工具链 | Linaro GCC 6.2.1 (arm-linux-gnueabihf) |
| QT 版本 | Qt 5.12.8（Buildroot 交叉编译） |
| QT 编译工具链 | arm-buildroot-linux-gnueabihf-gcc 7.5.0 |

## 驱动模块
| 模块 | 涉及子系统 | 关键技术点 |
|------|-----------|-----------|
| AP3216C | I2C 子系统 | i2c_client/i2c_driver 注册，I2C 读写时序，设备树节点配置 |
| DHT11 | GPIO 子系统 | 单总线时序（微秒级 GPIO 翻转），数据校验，字符设备接口 |
| LED/蜂鸣器/继电器 | GPIO 子系统 | 设备树多节点解析，GPIO 输出控制，统一字符设备框架 |
| SR501 | GPIO + 中断 | 中断申请与处理，异步通知机制，等待队列 |

基于平台总线驱动模型 (platform_driver)和I2C总线驱动模型，通过Pinctrl配置设备树在驱动中获取硬件资源（GPIO 编号、中断号等），使用 `file_operations` 向用户空间提供访问接口。

## 应用层

### QT 智能家居界面 (`app/qt_smarthome/`)

交叉编译 Qt 5.12.8 至 ARM 平台并部署到开发板 LCD 触摸屏。主要功能：

- 实时显示各传感器数据（温湿度、光照、人体感应、气体浓度）
- 触摸屏控制 LED、蜂鸣器、继电器（门锁）
- MQ135 气体传感器通过设备树配置 ADC 节点，应用层直接读取
- 独立 UART 收发线程处理蓝牙数据，避免阻塞 UI

### 蓝牙远程控制 (`app/uart_hc05/`)

基于 UART 串口与 HC-05 蓝牙模块通信，手机端通过蓝牙串口发送指令远程控制家居设备。

## 目录结构
```

├── drivers/                # Linux 设备驱动 
│   ├── ap3216/             # AP3216C — I2C 环境光/距离传感器 
│   ├── dht11/              # DHT11 — GPIO 温湿度传感器 
│   ├── ledbeepjdq/         # LED/蜂鸣器/继电器 — GPIO 控制 
│   └── sr501/              # SR501 — GPIO 中断人体红外 
├── app/                    # 应用层 
│   ├── qt_smarthome/       # QT 触摸屏智能家居界面 
│   └── uart_hc05/          # UART 蓝牙通信程序 
├── .gitignore 
└── README.md

```
## 编译

### 驱动模块

```bash
# 设置交叉编译工具链和内核源码路径后
cd drivers/ap3216
make
# insmod ap3216_i2c_drv.ko && insmod ap3216_i2c_client.ko
```

### QT 应用

使用配置好 ARM Kit 的 QT Creator 打开 `app/qt_smarthome/myqtwork.pro` 构建，或通过命令行：

```bash
/path/to/arm-qmake myqtwork.pro && make
```

编译产物传输到开发板

