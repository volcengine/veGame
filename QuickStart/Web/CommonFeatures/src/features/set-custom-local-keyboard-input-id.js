const setCustomLocalKeyboardInputId = (veGameSdkInstance) => {
  let setCustomLocalKeyboardInputIdBtn = null;
  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用

  const setCustomLocalKeyboardInputId = async () => {
    try {
      await veGameSdkInstance.setCustomLocalKeyboardInputId(
        "customLocalKeyboardInputId"
      );
      console.log(
        "已设置自定义本地键盘 Input 框ID为 customLocalKeyboardInputId"
      );
    } catch (e) {
      console.error(e);
    }
  };

  return {
    startSuccess() {
      // demo 的 pc 场景才展示 设置自定义本地键盘 Input 框
      if (veGameSdkInstance.isPC) {
        setCustomLocalKeyboardInputIdBtn = document.createElement("btn");
        $(setCustomLocalKeyboardInputIdBtn)
          .text("设置自定义本地键盘 Input 框")
          .addClass("btn btn-primary btn-sm")
          .appendTo(".action-container")
          .on("click", setCustomLocalKeyboardInputId);
      }
    },
    stopSuccess() {
      $(setCustomLocalKeyboardInputIdBtn).remove();
      setCustomLocalKeyboardInputIdBtn = null;
    },
  };
};

export default setCustomLocalKeyboardInputId;
