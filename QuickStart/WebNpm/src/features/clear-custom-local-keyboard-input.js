const clearCustomLocalKeyboardInput = (veGameSdkInstance) => {

  let clearCustomLocalKeyboardInputDataBtn = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      // demo 的 pc 场景才展示本地键盘
      if(veGameSdkInstance.isPC){
        clearCustomLocalKeyboardInputDataBtn = document.createElement('btn');
        $(clearCustomLocalKeyboardInputDataBtn)
          .text('清除自定义本地键盘 Input 框的状态')
          .addClass('btn btn-primary btn-sm')
          .appendTo('.action-container')
          .on('click', async () => {
            try {
              await veGameSdkInstance.clearCustomLocalKeyboardInput();
              alert('清除自定义本地键盘 Input 框的状态成功')
            } catch (error) {
              console.error(error)
            }
          });
      }
    },
    stopSuccess() {
      $(clearCustomLocalKeyboardInputDataBtn).remove();
      clearCustomLocalKeyboardInputDataBtn = null;
    },
  };
};

export default clearCustomLocalKeyboardInput;
