import { MODE } from "@volcengine/vegame";
import "./styles/index.css";
import GameController from "./core.js";
import { ACTION, MOUSE_BUTTON } from "./constant.js";

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

// 鼠标左键点击
const mouseLeftClick = async (x, y, veGameSdkInstance) => {
  console.log("[MouseLeft Click]:", x, y);
  await veGameSdkInstance.sendMouseMessage({
    button: MOUSE_BUTTON.LEFT,
    action: ACTION.DOWN,
    x,
    y,
  });
  await veGameSdkInstance.sendMouseMessage({
    button: MOUSE_BUTTON.LEFT,
    action: ACTION.UP,
    x,
    y,
  });
};

// 移动端触摸事件转换为鼠标事件:点击/拖拽/滑动
const touchToMouseMoveEvent = (veGameSdkInstance) => {
  let touchstartTs = 0; // 记录touchstart的时间戳
  const clickInterval = 1000; // 如果间隔大于1s，则认为是鼠标滑动，否则认为是点击
  // 在触摸移动事件中，检查从触摸开始到当前的时间间隔, 如果时间间隔大于 dragInterval，则将状态设置为拖拽，并发送鼠标按下的消息。
  // 如果时间间隔小于 dragInterval，则继续处理为滑动。
  const dragInterval = 200;
  let inTouchMove = false;
  let inDrag = false;

  veGameSdkInstance.on("on-touch-event", async (touchList) => {
    if (touchList.length !== 1) return;

    const { action, x, y, movementX, movementY } = touchList.pop();
    if (action === ACTION.TOUCH_START) {
      touchstartTs = Date.now(); // 记录手指触屏的时间
    }
    if (action === ACTION.TOUCH_END) {
      inTouchMove = false;
      // 拖拽过程中，手指抬起离屏
      if (inDrag) {
        console.log("[Drag End]:", x, y);
        inDrag = false;
        veGameSdkInstance.sendMouseMessage({
          button: MOUSE_BUTTON.LEFT,
          action: ACTION.UP,
          x,
          y,
        });
        return;
      }
      if (Date.now() - touchstartTs < clickInterval) {
        mouseLeftClick(x, y, veGameSdkInstance); // 如果触屏和离屏的时间小于某个阈值，则视为一次点击
        return;
      }
      // 其他情况，滑动结束
    }
    if (action === ACTION.TOUCH_MOVE) {
      console.log("[Touch Move]:", x, y);
      if (!inTouchMove && Date.now() - touchstartTs > dragInterval) {
        inDrag = true;
        // 如果是拖拽效果，鼠标左键先按下
        console.log("[Drag start]:", x, y);
        veGameSdkInstance.sendMouseMessage({
          button: MOUSE_BUTTON.LEFT,
          action: ACTION.DOWN,
          x,
          y,
        });
      }
      // 鼠标滑动
      console.log("[Mouse Move]:", x, y);
      veGameSdkInstance.sendMouseMessage({
        action: ACTION.MOVE,
        x,
        y,
        movementX,
        movementY,
      });
      inTouchMove = true;
    }
  });
};

// 游戏启动后才可以调用实例方法
const startSuccessCallback = (gameController) => {
  // 展示操作的UI
  document.getElementById("control-panel-detail")?.classList.remove("hidden");
  touchToMouseMoveEvent(gameController.sdkInstance);
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
