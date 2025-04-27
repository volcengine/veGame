import { MODE } from "@volcengine/vegame";
import "./libs/veGamePadSDK-Web_1.5.0.js";
import "./styles/index.css";
import GameController from "./core.js";
import { veGamePadList } from "./constant.js";

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
    isPC: false, // 表示是h5端
    domId: "player", // 容器id
  },
  startConfig: {
    // 游戏的横竖屏，mode为CLOUD_PHONE_GAME时传portrait，mode为CLOUD_GAME时传landscape
    gameRotation: mode === MODE.CLOUD_PHONE_GAME ? "portrait" : "landscape",
  },
});

// 游戏启动后才可以调用实例方法
const startSuccessCallback = (gameController) => {
  // 展示操作的UI
  document.getElementById("control-panel-detail")?.classList.remove("hidden");

  // veGamePadSDK 是从引入的脚本文件中导出的全局变量，为避免找不到名称的错误，添加类型断言
  const pad = new (window as any).veGamePadSDK(gameController.sdkInstance);
  // 从操控状态进入编辑状态时（调用 startEdit 方法），触发此事件回调。
  pad.on("start-edit", () => {
    console.log("start-edit");
  });
  // 手柄在编辑状态下，用户单击恢复默认时，触发此事件回调，此时手柄恢复至进入编辑之前的 UI 配置。
  pad.on("restore-default", () => {
    console.log("restore-default");
  });
  pad.on("cancel-edit", () => {
    console.log("cancel-edit");
  });
  // 在编辑状态下，用户单击保存时，触发此事件回调，此时手柄更新为新的 UI 配置。
  pad.on("save-config", (params) => {
    const { config } = params;
    console.log("save-config", config);
  });
  // 游戏过程中，游戏内部的震动场景会触发此事件。
  pad.on("gamepad-vibrate", (params) => {
    const { index, l, r } = params;
    console.log("index", index, "l", l, "r", r);
  });

  // 编辑: 此时用户可以对键位进行自定义（增删按键，调整位置等）
  const startModifyGamePadLayoutBtn = document.getElementById(
    "start-modify-game-pad-layout"
  );
  startModifyGamePadLayoutBtn?.addEventListener("click", () => {
    pad.startModifyGamePadLayout();
    console.log("进入编辑状态");
  });

  // 开启触屏操作透传至游戏
  const enableTouchBtn = document.getElementById("enable-touch");
  enableTouchBtn?.addEventListener("click", () => {
    pad.enableTouch(true);
    console.log("开启触屏操作透传至游戏成功");
  });

  // 关闭触屏操作透传至游戏
  const disableTouchBtn = document.getElementById("disable-touch");
  disableTouchBtn?.addEventListener("click", () => {
    pad.enableTouch(false);
    console.log("关闭触屏操作透传至游戏成功");
  });

  // 刷新虚拟手柄配置
  const refreshGamePadLayoutWithConfigBtn = document.getElementById(
    "refresh-game-pad-layout-with-config"
  );
  refreshGamePadLayoutWithConfigBtn?.addEventListener("click", () => {
    pad.refreshGamePadLayoutWithConfig();
    console.log("刷新虚拟手柄配置成功");
  });

  // 绑定其他pad方法
  const gamePadActionsSelect = document.getElementById(
    "game-pad-actions"
  ) as HTMLSelectElement;
  if (gamePadActionsSelect) {
    gamePadActionsSelect.addEventListener("change", async (event) => {
      const target = event.target as HTMLSelectElement;
      const method = target.value;
      try {
        switch (method) {
          case "show":
            pad.show();
            // 显示虚拟手柄成功后，可进入编辑状态
            document
              .getElementById("after-show-pad-operations")
              ?.classList.remove("hidden");
            console.log("显示虚拟手柄成功");
            break;
          case "disableVibrate":
            pad.enableVibrate(false);
            console.log("关闭震动成功");
            break;
          case "exportGamePadConfig":
            // eslint-disable-next-line no-case-declarations
            let config;
            // 为避免 "Unexpected lexical declaration in case block" 错误，使用块级作用域包裹变量声明
            try {
              config = await pad.exportGamePadConfig();
            } catch (error) {
              console.error("导出虚拟手柄配置时出错:", error);
            }
            console.log(`导出虚拟手柄配置，${JSON.stringify(config)}`);
            break;
          default:
            pad[method]?.();
            console.log(`${method}成功`);
            break;
        }
      } catch (error) {
        console.error(error);
      }
      console.log("value", method);
      pad[method]?.();
    });
  }
};

const stopSuccessCallback = () => {
  // 隐藏操作的UI
  document.getElementById("control-panel-detail")?.classList.add("hidden");
};

// 绑定UI事件
document
  .getElementById("start-btn")
  ?.addEventListener("click", () =>
    gameController.start(() => startSuccessCallback(gameController))
  );
document
  .getElementById("stop-btn")
  ?.addEventListener("click", () =>
    gameController.stop(() => stopSuccessCallback())
  );

// 动态填充 select 元素
const gamePadActionsSelect = document.getElementById(
  "game-pad-actions"
) as HTMLSelectElement;
if (gamePadActionsSelect) {
  veGamePadList.forEach((item) => {
    const option = document.createElement("option");
    option.text = item.label;
    option.value = item.value;
    gamePadActionsSelect.add(option);
  });
}

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
