const updateVideoScale = (veGameSdkInstance) => {
  let updateVideoScaleDataBtn = null;

  const updateVideoScale = async () => {
    try {
      await veGameSdkInstance.updateVideoScale();
      console.log("更新画面放缩比成功");
    } catch (error) {
      console.error(error);
    }
  };
  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      updateVideoScaleDataBtn = document.createElement("btn");
      $(updateVideoScaleDataBtn)
        .text("更新画面放缩比")
        .addClass("btn btn-primary btn-sm")
        .appendTo(".action-container")
        .on("click", updateVideoScale);
    },
    stopSuccess() {
      $(updateVideoScaleDataBtn).remove();
      updateVideoScaleDataBtn = null;
    },
  };
};

export default updateVideoScale;
