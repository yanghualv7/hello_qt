# 项目名称

## 简介

该项目是一个专业实习项目，主要功能包括使用Qt读取文本和图片，同时使用AES算法生成License认证，对软件的使用权限进行限制。

## 功能

1. **文本读取**：通过Qt界面读取和显示文本文件内容。
2. **图片读取**：通过Qt界面读取和显示图片文件内容。
3. **AES加密**：使用AES算法生成License认证，控制软件的使用权限。

## 项目结构

- `config/`：配置文件目录
- `create_license/`：生成AES秘钥及License文件的代码
- `ico/`：图标文件目录
- `image/`：图片文件目录
- `include/`：头文件目录
- `source/`：源代码目录
- `user_files/`：用户文件目录
- `.gitignore`：Git忽略文件配置
- `hello.qrc`：Qt资源文件
- `hello.rc`：资源文件
- `hello.sln`：解决方案文件
- `hello.ui`：Qt UI文件
- `hello.vcxproj`：Visual Studio项目文件
- `hello.vcxproj.filters`：Visual Studio项目过滤文件
- `resource.h`：资源头文件

## 环境依赖

- Qt 5.9.9
- Visual Studio 2019

## 安装和运行

1. 克隆项目代码：

   ```bash
   git clone https://github.com/yanghualv7/hello_qt.git
   cd hello_qt
   ```

2. 打开Qt Creator并加载`hello.pro`文件，配置项目。

3. 编译并运行项目。

## 使用说明

1. 打开软件后，可以选择“读取文本”或“读取图片”按钮来加载相应的文件。
2. 在create_license文件夹下，通过AES加密算法生成License文件，并将其保存在当前的License目录下。
3. 软件会根据生成的License文件来限制用户的使用权限。

## 贡献

欢迎任何形式的贡献，您可以通过提交issue或pull request的方式来贡献代码。

## 参考

本项目中的AES算法基于[Pollockovski/Aurora-Encryption](https://github.com/Pollockovski/Aurora-Encryption)项目进行修改和扩展。

## 许可证

该项目使用MIT许可证，详情请参见`LICENSE`文件。

---
