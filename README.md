# 项目名称

## 简介

该项目是一个专业实习项目，主要功能包括使用Qt读取文本和图片，同时使用AES算法生成License认证，对软件的使用权限进行限制，TCP通信，贪吃蛇小游戏。

## 功能

1. **文本读取**：通过Qt界面读取和显示文本文件内容。
2. **图片读取**：通过Qt界面读取和显示图片文件内容。
3. **AES加密**：使用AES算法生成License认证，控制软件的使用权限。
4. **制作认证文件**：使用AES算法，通过用户手动输入Mac地址进行加密。
5. **TCP通信**：使用TCP/IP协议和服务器进行通信，通过服务器限制登录设备数量。
6. **下载服务器数据**
7. **贪吃蛇小游戏**

## 项目结构

- `config/`：配置文件目录
- `create_license/`：生成AES秘钥及License文件的代码
- `ico/`：图标文件目录
- `image/`：图片文件目录
- `user_files/`：用户文件目录
- `hello`: 读取图片和文字进行展示
- `tcp_client`: 客户端示例demo，本项目客户端功能已集成到各自的模块中
- `tcp_server` : 服务器
- `snake`: 贪吃蛇小游戏
- `checkDateValidity`: 时间验证接口
- `Bin`: 动态库
- `Lib`: 静态库

## 环境依赖

- Qt 5.9.9
- Visual Studio 2019

## 安装和运行

1. 克隆项目代码：

   ```bash
   git clone https://github.com/yanghualv7/hello_qt.git
   cd hello_qt
   ```

2. 使用 **VS** 打开`sln`文件，配置项目。

3. 编译并运行项目。

## 使用说明

1. 打开软件后，可以选择“读取文本”或“读取图片”按钮来加载相应的文件。
2. 在create_license文件夹下，通过AES加密算法生成License文件，并将其保存在当前的License目录下。
3. 软件会根据生成的License文件来限制用户的使用权限。
4. 动态库创建方式可直接参考微软[官方教程](https://learn.microsoft.com/zh-cn/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-170)。
5. 在使用时需将动态库放入生成的**exe**同级目录下
6. 例如在本项目中是指将 **checkDateValidity.dll**放入**Release**文件夹中。

## 贡献

欢迎任何形式的贡献，您可以通过提交issue或pull request的方式来贡献代码。

## 参考

本项目中的AES算法基于[Pollockovski/Aurora-Encryption](https://github.com/Pollockovski/Aurora-Encryption)项目进行修改和扩展。

本项目中的TCP代码基于[CSDN博客](https://blog.csdn.net/qq_44722098/article/details/126001350?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522171807169016800186581519%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=171807169016800186581519&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_ecpm_v1~rank_v31_ecpm-1-126001350-null-null.nonecase&utm_term=socket&spm=1018.2226.3001.4450)修改。

## 许可证

该项目使用MIT许可证，详情请参见`LICENSE`。
