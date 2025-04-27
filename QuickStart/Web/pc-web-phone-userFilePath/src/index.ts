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
    // 保存用户游戏配置文件的路径列表
    userProfilePathList: ["C:\\Users\\Administrator\\Desktop\\test"],
  },
});

// 获取/设置用户游戏配置文件的路径：游戏启动后才可以调用实例方法
const startSuccessCallback = async (gameController) => {
  // 展示操作的UI
  document.getElementById("control-panel-detail")?.classList.remove("hidden");

  const userProfilePathInput = document.getElementById(
    "user-profile-path"
  ) as HTMLInputElement;

  // 绑定设置用户游戏配置文件的路径按钮事件
  const setButton = document.getElementById("set-user-profile-path");
  if (setButton) {
    setButton.addEventListener("click", async () => {
      const userProfilePath = userProfilePathInput?.value?.trim().split("\n");
      // 具体接口逻辑可见：https://www.volcengine.com/docs/6512/75597#%E8%AE%BE%E7%BD%AE%E4%BF%9D%E5%AD%98%E7%94%A8%E6%88%B7%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E7%9A%84%E8%B7%AF%E5%BE%84
      await gameController.sdkInstance.setUserProfilePath(userProfilePath);
      console.log(`用户游戏配置文件的路径已设置为: ${userProfilePath}`);
    });
  }

  // 绑定获取用户游戏配置文件的路径按钮事件
  const getButton = document.getElementById("get-user-profile-path");
  if (getButton) {
    getButton.addEventListener("click", async () => {
      const res = await gameController.sdkInstance.getUserProfilePath();
      const formattedPaths = res.join("\n");
      if (userProfilePathInput) {
        userProfilePathInput.value = formattedPaths;
      }
      console.log(`用户游戏配置文件的路径为: ${formattedPaths}`, res);
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
