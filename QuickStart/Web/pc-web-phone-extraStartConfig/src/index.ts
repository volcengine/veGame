import { MODE } from "@volcengine/vegame";
import "./styles/index.css";
import GameController from "./core";

// 手游模式
const mode = MODE.CLOUD_PHONE_GAME;

// 动态设置容器样式，容器必须有宽高后续sdk初始化才会成功
const playerContainer = document.getElementById("player");
if (playerContainer) {
  // 手游容器默认是竖屏pc-portrait，端游容器是横屏pc-landscape
  if (mode === MODE.CLOUD_PHONE_GAME) {
    playerContainer.className = "pc-portrait";
  } else {
    playerContainer.className = "pc-landscape";
  }
}

// 初始化 sdk
const gameController = new GameController({
  initConfig: {
    mode,
    isPC: true, // 表示是pc端
    domId: "player", // 容器id
  },
  startConfig: {
    // 游戏的横竖屏，mode为CLOUD_PHONE_GAME时传portrait，mode为CLOUD_GAME时传landscape
    gameRotation: mode === MODE.CLOUD_PHONE_GAME ? "portrait" : "landscape",
    extra: {
      // Pod 内跳转到第三方浏览器：是否开启在云端游戏中点击链接打开游览器的功能, 开启该功能，需要同时联系云游戏相关人员在云游戏管理后台启用该功能
      enable_browser: "true",
      /* Pod 内游戏产生日志上传：
        开启日志上传功能所需要的对象存储信息:
        注：开启日志上传至对象存储功能，需向云游戏技术支持提供火山引擎云游戏业务 ID（可通过云游戏控制台 业务配置 页面获取）
      */
      debug_info: JSON.stringify({
        schema: "btd_tos", // 固定值，必填
        config: {
          ak: "your_ak",
          sk: "your_sk",
          bucket: "your_bucket",
          endpoint: "tos-cn-beijing.volces.com",
          region: "cn-beijing",
        },
        key: "test/adc.zip", // 仅支持zip格式
      }),
      // 云游戏改机参数列表，具体配置可联系云游戏相关人员提供技术支持
      characteristics: JSON.stringify({
        manufacturer: "xxx", // 厂商
        brand: "xxx", // 品牌
      }),
      // 指定运行游戏需要的伴随程序列表, 已上传的伴随程序列表以及伴随程序包名可通过调用服务端 伴随程序管理 接口获取
      userFrameworkApp: JSON.stringify(["com.pkg1", "com.pkg2", "com.pkg3"]),
      // 本地调试开启 debug 模式，可看到Pod打印日志，线上关闭
      isDebug: import.meta.env.VEGAME_DEV,
    },
  },
});

// 绑定UI事件
document
  .getElementById("start-btn")
  ?.addEventListener("click", () => gameController.start());
document
  .getElementById("stop-btn")
  ?.addEventListener("click", () => gameController.stop());

// 控制面板切换功能：与sdk无关
const togglePanel = document.getElementById("toggle-panel");
const controlPanel = document.getElementById("control-panel");

togglePanel?.addEventListener("click", () => {
  const isShowing = controlPanel?.classList.contains("show");
  controlPanel?.classList.toggle("show");
  controlPanel?.classList.toggle("hidden");
  if (togglePanel) {
    togglePanel.textContent = isShowing ? "隐藏控制面板" : "显示控制面板";
  }
});
