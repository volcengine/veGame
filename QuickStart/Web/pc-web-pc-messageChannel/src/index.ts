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
  },
});

const startSuccessCallback = (gameController) => {
  // 展示操作的UI
  document.getElementById("control-panel-detail")?.classList.remove("hidden");

  const destUserIds = new Map();
  // 运行过程中，云端实例发送到客户端的自定义消息事件回调。
  gameController.sdkInstance.on("message", (msg) => {
    /*
    userId: string; // 发送消息的用户id
    mid: number; // 消息唯一标识 ID
    text: string; // 文本消息内容
    binary: ArrayBuffer; // 二进制消息内容（仅端游支持 ArrayBuffer 数据类型发送）
    time: number; // 消息发送时的时间戳
    key: string; // 自定义消息的 key
    */
    const { userId, mid, text, binary, time, key } = msg;
    console.log(userId, mid, text, binary, time, key);
    // 处理消息的逻辑
  });

  // 通过消息通道收发消息时，云端实例在线的回调
  gameController.sdkInstance.on(
    "message-channel-online",
    async (destUserId) => {
      destUserIds.set(destUserId, true);
      console.log("------------------当前通道连接成功:", destUserId);
      // 客户端发送消息到云端实例，需要在消息通道建立后调用，即需要收到 'message-channel-online' 事件回调。
      const message = {
        // 消息内容（仅端游支持 ArrayBuffer 数据类型发送），比如您需要传递的登录参数等信息
        message: "message",
        // 发送的消息否需要应答
        needAck: true,
        // 应答消息的超时时间（毫秒）
        timeout: 5 * 1000,
        // 云端游戏初始化 veMessageChannelClient 时填入的目标用户 ID（如果 Message Channel 连接通道超过1条，必须指定 destUserId）
        destUserId,
        // 自定义消息的 key
        key: "test-key",
      };
      const res = await gameController.sdkInstance.sendMessage(message);
      console.log("发送消息结果：", res);
    }
  );
  // 通过消息通道收发消息时，云端实例离线的回调（发送消息可能失败）
  gameController.sdkInstance.on("message-channel-offline", (destUserId) => {
    destUserIds.set(destUserId, false);
    // 处理消息通道离线的逻辑
    console.log("------------------当前通道断开连接:", destUserId);
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
