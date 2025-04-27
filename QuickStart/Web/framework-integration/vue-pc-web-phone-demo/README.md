# Vue + Vite

以 PC 玩手游为例，我们为您提供了 Vue+Vite 框架下的 PC 端玩手游的示例代码

# 运行说明

1. 安装依赖

```bash
npm install
```


2. 配置环境变量：
   修改 `.env` 文件，填入您的配置信息

3. 根据您的实际场景调整`index.ts`中的初始化参数`initConfig`，默认是 PC 玩手游

```
| 实际场景       | isPC（是否是PC端） | mode（手游/端游）   |
| pc玩端游       | true             | MODE.CLOUD_PC        |
| pc玩手游       | true             | MODE.CLOUD_PHONE_GAME|
| h5玩端游       | false            | MODE.CLOUD_PC        |
| h5玩手游       | false            | MODE.CLOUD_PHONE_GAME|
```

4. 运行

```bash
npm run dev
```

5. 打开浏览器访问
