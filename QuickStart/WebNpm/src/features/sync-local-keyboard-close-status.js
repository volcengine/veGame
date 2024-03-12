const syncLocalKeyboardCloseStatus = (veGameSdkInstance) => {

  let syncLocalKeyboardCloseStatusDataBtn = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      // demo 的 pc 场景才展示 同步本地键盘隐藏/关闭状态到云端游戏
      if(veGameSdkInstance.isPC){
        syncLocalKeyboardCloseStatusDataBtn = document.createElement('btn');
        $(syncLocalKeyboardCloseStatusDataBtn)
          .text('同步本地键盘隐藏/关闭状态到云端游戏')
          .addClass('btn btn-primary btn-sm')
          .appendTo('.action-container')
          .on('click', async () => {
            try {
              await veGameSdkInstance.syncLocalKeyboardCloseStatus();
              alert('同步本地键盘隐藏/关闭状态到云端游戏成功')
            } catch (error) {
              console.error(error)
            }
          });
      }
    },
    stopSuccess() {
      $(syncLocalKeyboardCloseStatusDataBtn).remove();
      syncLocalKeyboardCloseStatusDataBtn = null;
    },
  };
};

export default syncLocalKeyboardCloseStatus;
