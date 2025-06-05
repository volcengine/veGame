import { MODE } from "@volcengine/vegame";
import "./styles/index.css";
import GameController from "./core";

// 手游模式
const mode = MODE.CLOUD_PHONE_GAME;
const File_NAME = "example.txt";
const FILE_SAVE_PATH = "/sdcard/Download";
const RESPONSE_TIMEOUT = 5 * 1000;
const MESSAGE_KEY = "FILE_UPLOAD";

/*
  本例子展示Web SDK和消息通道MC SDK的文件传输功能。
  1. Web SDK 监听消息通道事件
  2. Web SDK 上传文件（key为FILE_UPLOAD）
  3. MC SDK请求 Web SDK 上传的文件信息（轮训）
  4. Web SDK 收到消息后判断
    4.1 请求内容的 key 为 MESSAGE_KEY
    4.2 该文件是否上传完毕
    4.3 消息通道在线
  5. 满足条件，则 Web SDK 发送消息给出的文件名称和路径（key为FILE_UPLOAD），否则是其他消息任务，则不处理（或其他处理方式）
*/

// 动态设置容器样式函数 容器必须有宽高后续sdk初始化才会成功
const setPlayerContainerStyle = (playerContainer: HTMLElement | null) => {
  if (playerContainer) {
    playerContainer.className =
      mode === MODE.CLOUD_PHONE_GAME ? "pc-portrait" : "pc-landscape";
  }
};
setPlayerContainerStyle(document.getElementById("player"));

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

// 游戏启动成功后绑定回调和事件监听
const startSuccessCallback = (gameController) => {
  // 展示操作的UI
  document.getElementById("control-panel-detail")?.classList.remove("hidden");

  const destUserIds = new Map();
  let channelStatus = false; // 消息通道状态
  let currentDestUserId: string | undefined; // 当前的目标用户 ID
  let uploadInfoProgress = 0; // 上传进度
  let file; // 上传的文件
  let uploading = false; // 是否正在上传
  let downloading = false; // 是否正在下载

  // 运行过程中，云端实例发送到客户端的自定义消息事件回调。
  gameController.sdkInstance.on("message", async (msg) => {
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
    // 处理消息的逻辑 约定key为FILE_UPLOAD时，表示请求上传文件的名称和路径(可以轮训：当收到消息时，判断消息通道是否在线 & File_NAME文件传输完毕)
    if (key === MESSAGE_KEY && uploadInfoProgress === 100 && channelStatus) {
      // 客户端发送消息到云端实例，需要在消息通道建立后调用，即需要收到 'message-channel-online' 事件回调。
      const message = {
        // 消息内容（仅端游支持 ArrayBuffer 数据类型发送），比如您需要传递的登录参数等信息
        message: `${File_NAME}_${FILE_SAVE_PATH}`,
        // 发送的消息否需要应答
        needAck: true,
        // 应答消息的超时时间（毫秒）
        timeout: RESPONSE_TIMEOUT,
        // 云端游戏初始化 veMessageChannelClient 时填入的目标用户 ID（如果 Message Channel 连接通道超过1条，必须指定 destUserId）
        destUserId: currentDestUserId,
        // 自定义消息的 key
        key: MESSAGE_KEY,
      };
      try {
        const res = await gameController.sdkInstance.sendMessage(message);
        console.log("发送消息结果：", res);
      } catch (error) {
        console.log("发送消息失败：", error);
      }
    }
  });

  // 通过消息通道收发消息时，云端实例在线的回调
  gameController.sdkInstance.on(
    "message-channel-online",
    async (destUserId) => {
      destUserIds.set(destUserId, true);
      console.log("------------------当前通道连接成功:", destUserId);
      currentDestUserId = destUserId;
      channelStatus = true;
    }
  );
  // 通过消息通道收发消息时，云端实例离线的回调（发送消息可能失败）
  gameController.sdkInstance.on("message-channel-offline", (destUserId) => {
    destUserIds.set(destUserId, false);
    // 处理消息通道离线的逻辑
    console.log("------------------当前通道断开连接:", destUserId);
    channelStatus = false;
    currentDestUserId = undefined;
  });
  // 选择文件
  const fileInput = document.getElementById("upload-input");
  fileInput?.addEventListener("change", (e) => {
    // 由于 e.target 的类型是 EventTarget，默认没有 files 属性，需要将其类型断言为 HTMLInputElement
    file = (e.target as HTMLInputElement)?.files?.[0];
    console.log(file, e, e.target);
  });
  // 上传文件
  const uploadBtn = document.getElementById("upload-btn");
  uploadBtn?.addEventListener("click", () => {
    if (file) {
      gameController.sdkInstance.startSendFile(file, {
        name: File_NAME,
        folder: FILE_SAVE_PATH,
      });
    } else {
      alert("请选择文件");
    }
  });
  // 暂停上传
  const pauseBtn = document.getElementById("pause-upload-btn");
  pauseBtn?.addEventListener("click", () => {
    if (uploading) {
      gameController.sdkInstance.pauseSendFile();
    } else {
      alert("暂无文件上传中");
    }
  });
  // 停止发送文件
  const stopBtn = document.getElementById("stop-upload-btn");
  stopBtn?.addEventListener("click", () => {
    if (uploading) {
      gameController.sdkInstance.stopSendFile();
    } else {
      alert("暂无文件上传中");
    }
  });

  // 暂停下载
  const pauseDownloadBtn = document.getElementById("pause-download-btn");
  pauseDownloadBtn?.addEventListener("click", () => {
    if (downloading) {
      gameController.sdkInstance.pauseReceiveFile();
    } else {
      alert("暂无文件下载中");
    }
  });
  // 停止下载
  const stopDownloadBtn = document.getElementById("stop-download-btn");
  stopDownloadBtn?.addEventListener("click", () => {
    if (downloading) {
      gameController.sdkInstance.stopReceiveFile();
    } else {
      alert("暂无文件下载中");
    }
  });

  // 监听文件传输事件
  gameController.sdkInstance.on("on-send-file-done", async (data) => {
    uploading = false;
    console.log("上传文件成功", data);
    uploadInfoProgress = 100;
    /* data
    {
        "carries": {
            "result": {},
            "folder": "/sdcard/Download",
            "name": "api-helper.config.ts",
            "url": "",
            "md5": "1bd2184203a6978d0df38d3fae723898"
        }
    }
    */
  });
  gameController.sdkInstance.on("on-send-file-progress", (progress) => {
    uploading = true;
    console.log("上传文件进度", `${progress}%`);
  });
  gameController.sdkInstance.on("on-send-file-info", (data) => {
    /* data
    {
        "message": "当前已有上传任务，新任务已加入队列",
        "options": {}
    }
    */
    console.log("上传文件信息", data);
  });
  gameController.sdkInstance.on("on-send-file-error", (error) => {
    uploading = false;
    console.log("上传文件失败", error, error.message);
  });
  gameController.sdkInstance.on("on-send-file-message-error", (data) => {
    uploading = false;
    console.log("上传文件消息发动失败", data);
    /* data:
    {
        "action": 2,
        "carries": {
            "result": {},
            "folder": "/sdcard/Download",
            "name": "pnpm-lock.yaml",
            "url": "",
            "md5": "4140a95f3efe8884895ed5fd160afd04"
        }
    }
    */
  });
  gameController.sdkInstance.on("on-send-file-request-error", (data) => {
    uploading = false;
    console.log("上传文件请求发送失败", data);
  });
  gameController.sdkInstance.on("on-download-file-done", (data) => {
    downloading = false;
    console.log("下载文件成功", data);
    if (typeof data === "string") {
      window.open(data);
    } else {
      const a = document.createElement("a");
      a.download = "ddd.mp4";
      document.body.append(a);
      a.href = URL.createObjectURL(data);
      a.click();
    }
  });
  gameController.sdkInstance.on("on-download-file-progress", (data) => {
    downloading = true;
    console.log("下载文件进度", data);
  });
  gameController.sdkInstance.on("on-download-file-request-error", (data) => {
    downloading = false;
    console.log("下载文件请求发送失败", data);
  });
  gameController.sdkInstance.on("on-download-file-message-error", (data) => {
    downloading = false;
    console.log("下载文件消息发动失败", data);
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
