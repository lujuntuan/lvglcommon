# lvglcommon

这是一个基于lvgl封装的用于MCU的ui开发的中间件

## 特性:

- 基于SDL2定制的渲染平台
- 支持QNX的WFD渲染框架
- MCU仪表绘制demo
- 非侵入式的工程设计
- 支持CMake、Makefile（待实现）

## 依赖：

- C Compiler
- CMake大于3.7

## 如何编译：

```cmake
cmake -B build
cmake --build build --target install
```

## 如何使用：

运行

```cmake
lv_demo_ejanus
```

Warning: ejanus切图不可商业盗用

## 版权所有:

Juntuan.Lu, 2020-2030, All rights reserved.
