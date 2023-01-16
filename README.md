# Mineglyph

**VERSION/版本** 1.0.0 <br/>

**UPDATE TIME/更新时间** 2023/1/16 UTC+8 <br/>

## English

A tool to convert Outline Fonts into Minecraft Unicode Bitmap Font. <br/>

### Why Mineglyph?

In the new version of Minecraft, we can directly add outline fonts to the resource pack to replace the original bitmap fonts for a better look and feel, but the older versions earlier than Minecraft 1.13 did not support outline fonts, so we need to convert outline fonts to bitmap fonts. This tool can help you quickly convert an outline font to a bitmap font. <br/>

### Features

**SIMPLE**: Convert a font to a resource pack need a single command only. <br/>

**FAST**: Each font conversion takes only 30 seconds. (Configuration: Each character has a resolution of 64 pixels, and images have a resolution of 1024 pixels. High resolution will increase the time) <br/>

**LIGHT WEIGHT**: It's just an executable file less than 1MB. <br/>

**EMBEDDABLE**: Under our license, you can use this program in your commercial or non-commercial projects. <br/>

### Download & Use

You can download the executable files in the Releases of this repository. <br/>

**NOTICE: This requires your system to be Windows 7 x64 or above.**<br/>

You can save the file anywhere and start using it. <br/>

### Usage

#### Help

Run the following command to obtain help:

```powershell
mineglyph
```

or

```powershell
mineglyph help
```

#### Version Info

```powershell
mineglyph version
```

#### Copyright Info

```powershell
mineglyph copyright
```

#### Convert Font

```powershell
mineglyph cv [-i, -in] {INFILE} [-o, -out] {OUTPATH} <[-n, -name] {NAME}> <[-r, -resolution] {RESOLUTION}> <-withpack>
```

##### OPTIONS

**Note: Unicode characters have not been tested and should not appear in any options!** <br/>
<br/>
`-i, -in {INFILE}` <br/>
Specifies the font file to input. TTF, OTF is supported. <br/>
**{INFILE}** Font file path (Absolute path is preferred) <br/>
<br/>
`-o, -out {OUTPATH}` <br/>
Specifies the directory for the output data folder <br/>
**Note: The program will create a folder in this directory with the name specified by -n. If there is no -n, it will be automatically intercepted from the input file path. Therefore, the actual data output directory should be *{OUTPATH}\\{NAME}*.** <br/>
**{OUTPATH}** The directory of the output data folder <br/>
<br/>
`-n, -name {NAME}` <br/>
Specify a name *(optional)* <br/>
This name will be used as the name of the output data folder and the name of the MINECRAFT Resource pack. <br/>
**{NAME}** Name, and must conform to the folder and file naming conventions. <br/>
<br/>
`-r, -resolution {RESOLUTION}` <br/>
Specifies the resolution *(optional)* for a single character <br/>
Each Unicode bitmap font image is 16x16 characters, so the image RESOLUTION is **{RESOLUTION} \* 16** <br/>
**{RESOLUTION}** Single character resolution, which should be in the range 16 to 256, must be a multiple of 16 in pixels, default is 64. <br/>
<br/>
`-e, -encode {ENCODING}` <br/>
Specifies the encoding of the font file *(optional)* <br/>
**{ENCODING}** Encoding, optional **unicode**, **big5**, **gb2312**, the default is unicode. <br/>
<br/>
`-withpack` <br/>
Whether to automatically pack as MINECRAFT Resource Pack after conversion is complete *(optional)* <br/>
Use this option to automatically package as a MINECRAFT resource pack after the conversion is complete. <br/>

##### Example
Convert **D:\\Example.ttf** to a bitmap Font and store it in the directory **D:\\Font\\ExampleTTF\\**
```posershell
mineglyph cv -i D:\Example.ttf -o D:\Font -n ExampleTTF
```
<br/>
Convert **D:\\Example2.ttf** to a bitmap Font with a resolution of 128 pixels and store it in the directory **D:\\Font\\ExampleTTF2\\**
```posershell
mineglyph cv -i D:\Example2.ttf -o D:\Font -n ExampleTTF2 -r 128
```
<br/>
Convert **D:\\ExampleBIG5.ttf** encoded by Big5 into a bitmap Font and store it in the directory **D:\\Font\\ExampleFont\\**
```posershell
mineglyph cv -i D:\ExampleBIG5.ttf -o D:\Font -n ExampleFont -e big5
```
<br/>
Convert **D:\\Example3.ttf** to a bitmap Font with resolution 128 and store it in the directory **D:\\Font\\ExampleTTF3\\** and package it as MINECRAFT Resource Pack
```posershell
mineglyph cv -i D:\Example3.ttf -o D:\Font -n ExampleTTF3 -r 128 -withpack
```
<br/>
### License

See **LICENSE** <br/>

#### Overview

(c) 2023 Hakdaw. All rights reserved. <br/>

Under this license, you may use the binary Mineglyph for personal use, commercial or non-commercial project production. Hakdaw grants users the right to use, distribute and redistribute the binary Mineglyph, provided that your project indicates that Mineglyph is not your work. Note that Mineglyph is used in your project and the copyright of Mineglyph belongs to Hakdaw. <br/>

Hakdaw distributes secondary Mineglyph files under this license, which means that only Minrglyph binaries are applicable to this license. <br/>

No one is allowed to decomcompile, modify , erase copyright information, or pretend to be their own Mineglyph binary. <br/>

Once you have been granted this license, you have the right to update the Mineglyph binary for the life of the Mineglyph. <br/>

We cannot guarantee the normal operation of Mineglyph in your environment and any consequences arising therefrom shall be borne by the authorized party and have nothing to do with Hakdaw. <br/>

If you are using the Mineglyph binary for an open source project, you must bring this license and state that the Mineglyph is used and the copyright of Mineglyph belongs to Hakdaw. If you are using the Mineglyph binary for a closed source project, you only need to state that the Mineglyph is used and the copyright of Mineglyph belongs to Hakdaw. <br/>

Mineglyph is not allowed to use Mineglyph to convert fonts that are not authorized to you. The copyright of the converted bitmap fonts belongs to the authors of the original fonts. <br/>

Any person using the Mineglyph binary shall be deemed to have granted this license. <br/>

The license may be renewed, and when the license is renewed, the person who has been granted this license will be revoked and a new license will automatically be granted. <br/>

### Q&A

**Q: Why does the stroke thickness vary when the resource pack is imported into Minecraft?** <br/>
A: Please try using high resolution conversion. <br/>
<br/>
**Q: Is the advanced version available?** <br/>
A: No. An exception will be displayed after an earlier version is imported. Only versions 1.13 and later are supported. <br/>
<br/>
**Q: Why does the import of resource pack have no effect?** <br/>
A: It may conflict with other mods, resource packs, or the current version may not be compatible. You can submit issue feedback. <br/>
<br/>
**Q: Failed to convert?** <br/>
A: Please submit issue feedback with details. <br/>
<br/>
**Q: I found a BUG or other problem** <br/>
A: Please submit the issue and describe your problem in detail, including the details. <br/>
<br/>

<br/>

## 中文

一个将矢量字体转为我的世界Unicode位图字体的工具。<br/>

### 为什么使用Mineglyph？

在新版本我的世界里，我们可以直接在资源包里添加矢量字体来代替原版的位图字体，以获得更好的观感。但我的世界1.13以前的老版本并不支持矢量字体，因此我们需要将矢量字体转换为位图字体，这个工具可以帮助你快捷地将一个矢量字体转换为位图字体。<br/>

### 特色

**简单**：一行命令可直接完成从字体转换为资源包的所有操作。<br/>

**快速**：转换一个字体仅需不到30秒！（测试设置：单字分辨率64像素，图片分辨率1024像素。高分辨率下耗时会有所增加）<br/>

**轻量**：只有一个1MB不到的可执行文件。<br/>

**可嵌入**：在我们的许可证下，你可以在你的商用或非商用项目中使用本程序。<br/>

### 下载和使用

你可以在本仓库的Releases中下载本程序的可执行文件。<br/>

**注意：这要求你的系统必须是Windows 7 x64及以上。**<br/>

你可以将这个文件保存到任意位置，就可以开始使用了。<br/>

### 用法

#### 帮助

执行以下命令获取帮助

```powershell
mineglyph
```

或

```powershell
mineglyph help
```

#### 版本信息

```powershell
mineglyph version
```

#### 版权信息

```powershell
mineglyph copyright
```

#### 转换字体

```powershell
mineglyph cv [-i, -in] {INFILE} [-o, -out] {OUTPATH} <[-n, -name] {NAME}> <[-r, -resolution] {RESOLUTION}> <-withpack>
```

##### 参数

**注意：包含中文在内的Unicode字符未经测试，不应出现在任何参数内！** <br/>
<br/>
`-i, -in {INFILE}` <br/>
指定输入的字体文件，支持TTF，OTF。<br/>
**{INFILE}** 字体文件的路径（最好使用绝对路径）<br/>
<br/>
`-o, -out {OUTPATH}` <br/>
指定输出数据文件夹的目录 <br/>
**注意：程序会在此目录下创建 -n 所指定名字的文件夹，如果无 -n ，将会自动从输入文件路径中截取。因此，真实的数据输出目录应该为 *{OUTPATH}\\{NAME}*。** <br/>
**{OUTPATH}** 输出数据文件夹的目录 <br/>
<br/>
`-n, -name {NAME}` <br/>
指定一个名称*（可选）* <br/>
这个名称将被用来做输出数据文件夹的名称和我的世界资源包的名称。<br/>
**{NAME}** 名称，须符合文件夹及文件命名规范 <br/>
<br/>
`-r, -resolution {RESOLUTION}` <br/>
指定单个字符的分辨率*（可选）* <br/>
每张Unicode位图字体图片都为16x16字符，因此图片分辨率为**{RESOLUTION} \* 16** <br/>
**{RESOLUTION}** 单个字符分辨率，应在范围16至256之间，必须为16的倍数，单位为像素，默认为64。<br/>
<br/>
``-e, -encode {ENCODING}` <br/>
指定字体文件的编码*（可选）* <br/>
**{ENCODING}** 编码，可选**unicode**, **big5**, **gb2312**，默认为unicode。<br/>
<br/>
`-withpack` <br/>
是否在转换完成后自动打包为我的世界资源包*（可选）* <br/>
使用此选项可以在转换完成后自动打包为我的世界资源包。<br/>

##### 示例
将 **D:\\Example.ttf** 转换为位图字体，存储在 **D:\\Font\\ExampleTTF\\** 目录下
```posershell
mineglyph cv -i D:\Example.ttf -o D:\Font -n ExampleTTF
```

将 **D:\\Example2.ttf** 转换为分辨率为128的位图字体，存储在 **D:\\Font\\ExampleTTF2\\** 目录下
```posershell
mineglyph cv -i D:\Example2.ttf -o D:\Font -n ExampleTTF2 -r 128
```

将Big5编码的 **D:\\ExampleBIG5.ttf** 转换为位图字体，存储在 **D:\\Font\\ExampleFont\\** 目录下
```posershell
mineglyph cv -i D:\ExampleBIG5.ttf -o D:\Font -n ExampleFont -e big5
```

将 **D:\\Example3.ttf** 转换为分辨率为128的位图字体，存储在 **D:\\Font\\ExampleTTF3\\** 目录下，并打包为我的世界资源包
```posershell
mineglyph cv -i D:\Example3.ttf -o D:\Font -n ExampleTTF3 -r 128 -withpack
```

### 许可证

参见 **LICENSE** <br/>

#### 概述

(c)2023 Hakdaw. 版权所有。<br/>

你可以在此许可证下使用Mineglyph的二进制文件进行个人使用，商业或非商业的项目制作，Hakdaw授予使用者Mineglyph二进制文件的使用、分发、二次分发权，但前提是你的项目必须注明Mineglyph不是你的作品，注明你的项目使用了Mineglyph且Mineglyph的版权属于Hakdaw。<br/>

Hakdaw依据此许可证分发Mineglyph的二级制文件，这意味着仅有Minrglyph的二进制文件适用于本许可证。<br/>

任何人不得对Mineglyph二进制文件进行反编译、修改、抹除版权信息、装作是自己的作品等。<br/>

你一旦被授予了此许可证，意味着在Mineglyph生命周期内，你享有使用Mineglyph二进制文件的更新的权利。<br/>

我们无法保证在你的环境下Mineglyph的正常运行，因此产生的任何后果由被授权人承担，与Hakdaw无关。<br/>

如果你将Mineglyph二进制文件用于开源项目，则必须携带本许可证，并注明使用了Mineglyph且Mineglyph的版权属于Hakdaw，如果用于闭源项目，则只需注明使用了Mineglyph且Mineglyph的版权属于Hakdaw。<br/>

转换字体时，需遵守原字体许可证，你不得使用Mineglyph转换未授权给你的字体，转换的位图字体版权归原字体作者所有。<br/>

任何使用Mineglyph二进制文件的人视为被授予了本许可证。<br/>

许可证可能更新，当许可证更新时，被授予了本许可证的人将被撤销授予，同时自动授予新许可证。<br/>

### 常见问题

**Q:为什么资源包导入到我的世界里后笔画出现粗细不一？**<br/>
A:请尝试使用高分辨率转换。<br/>
<br/>
**Q:高版本可用吗？**<br/>
A:不可用，高版本导入后会显示异常，仅支持1.13以下版本。<br/>
<br/>
**Q:资源包导入后为什么没效果？**<br/>
A:可能和其他Mod，资源包冲突，也可能当前版本不兼容，可以提交issue反馈。<br/>
<br/>
**Q:转换失败？**<br/>
A:请提交issue反馈，附上详细信息。<br/>
<br/>
**Q:我发现了BUG或其他问题**<br/>
A:请提交issue，并详细描述你的问题，附上详细信息。<br/>