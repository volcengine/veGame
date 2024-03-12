const imeCompositionVisible = (veGameSdkInstance) => {

  let imeCompositionVisibleDataBtn = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      if(veGameSdkInstance.isPC){
        imeCompositionVisibleDataBtn = document.createElement('btn');
        $(imeCompositionVisibleDataBtn)
          .text('本地输入法显示中文合成过程')
          .addClass('btn btn-primary btn-sm')
          .appendTo('.action-container')
          .on('click', async () => {
            try {
              await veGameSdkInstance.setIMECompositionVisible(true, 'background-color: transparent;');
              alert('本地输入法显示中文合成过程成功')
            } catch (error) {
              console.error(error)
            }
          });
      }
    },
    stopSuccess() {
      $(imeCompositionVisibleDataBtn).remove();
      imeCompositionVisibleDataBtn = null;
    },
  };
};

export default imeCompositionVisible;
