// 本地输入法
const imeCompositionVisible = (veGameSdkInstance) => {
  let imeCompositionVisibleDataBtn = null;

  const showImeComposition = async () => {
    try {
      /*
        本地输入法显示中文合成过程
        参数说明：
        @param visible: 是否显示中文合成过程
        @param style: 中文合成过程的样式
      */
      await veGameSdkInstance.setIMECompositionVisible(
        true,
        "background-color: transparent;"
      );
      console.log("本地输入法显示中文合成过程成功");
    } catch (error) {
      console.error(error);
    }
  };

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      if (veGameSdkInstance.isPC) {
        imeCompositionVisibleDataBtn = document.createElement("btn");
        $(imeCompositionVisibleDataBtn)
          .text("本地输入法显示中文合成过程")
          .addClass("btn btn-primary btn-sm")
          .appendTo(".action-container")
          .on("click", showImeComposition);
      }
    },
    stopSuccess() {
      $(imeCompositionVisibleDataBtn).remove();
      imeCompositionVisibleDataBtn = null;
    },
  };
};

export default imeCompositionVisible;
