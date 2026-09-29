# Linux-V4L2-Camera-Capture

基于 Linux V4L2 (Video for Linux Two) 框架的 USB 免驱摄像头视频采集应用。

## 📌 项目亮点 (Features)
- **纯应用层实现**：基于 C 语言及标准 Linux 系统调用 (`open`, `ioctl`, `mmap`) 实现设备交互，无需编写内核驱动。
- **高效内存管理**：采用 `mmap` 内存映射技术，结合 V4L2 缓冲区队列机制 (`VIDIOC_QBUF` / `VIDIOC_DQBUF`)，实现零拷贝视频帧获取。
- **鲁棒的容错机制**：规范处理 `ioctl` 返回值，针对设备断开、内存映射失败等异常情况提供完善的错误捕获与资源释放（`munmap` / `close`）。
- **标准 YUV 数据流**：支持获取 YUYV (YUV422) 格式视频流，并内置格式校验逻辑。

## 🛠️ 技术栈 (Tech Stack)
- **Language**: C
- **OS**: Linux (Ubuntu / Embedded ARM)
- **Kernel Framework**: V4L2
- **Build Tool**: Makefile / GCC

## 🚀 快速开始 (Getting Started)
### 编译
```bash
gcc -o v4l2_capture v4l2_capture.c

运行
确保你的 USB 摄像头已连接到 Linux 环境，并具有 /dev/video0 权限。
./v4l2_capture
