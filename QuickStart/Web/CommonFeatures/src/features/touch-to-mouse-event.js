import { ACTION, MOUSE_BUTTON } from "../constant";

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

export default touchToMouseMoveEvent;
