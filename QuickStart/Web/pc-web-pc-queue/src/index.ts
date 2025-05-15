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
    /*
    设置游戏玩家排队功能：
      0：不设置排队功能
      1~99：队列优先级，数值越小优先级越高
    */
    queuePriority: 1, // 队列优先级，数字越小优先级越高
  },
});

// 获取/设置自动回收时长：游戏启动后才可以调用实例方法
const startSuccessCallback = (gameController) => {
  // 展示操作的UI
  document.getElementById("control-panel-detail")?.classList.remove("hidden");

  // 监听排队进度
  gameController.sdkInstance.on("queue-update", ({ queueList, done }) => {
    /*
    队列信息
    {
      queueList: IQueueInfo[], // 队列信息
      done: boolean, // 是否排队成功
    }
    interface IQueueInfo{
      configurationCode: string; // 套餐信息
      userPosition: number; // 用户排队位置
      total: number; // 队列总长度
    }
    */
    if (done) {
      console.log("排队完成");
    } else {
      console.log("当前排队信息：", JSON.stringify(queueList));
    }
  });

  // 排队完成
  gameController.sdkInstance.on("queue-success", ({ remainTime }) => {
    console.log(`排队完成，申请服务所需时间：${remainTime}秒`);
  });

  // 取消排队
  document
    .getElementById("cancel-queue-btn")
    ?.addEventListener("click", async () => {
      try {
        const res = await gameController.sdkInstance.cancelQueue();
        console.log(`取消排队${res ? "成功" : "失败"}`);
      } catch (e) {
        console.log("取消排队失败", e);
      }
    });
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
