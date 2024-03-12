/**
 * 画面旋转功能
 * 具体详见：https://www.volcengine.com/docs/6394/1182635
 */

const updateVideoScale = (veGameSdkInstance) => {

  let updateVideoScaleDataBtn = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      // demo 的 pc 场景才展示 更新画面放缩比
      updateVideoScaleDataBtn = document.createElement('btn');
      $(updateVideoScaleDataBtn)
        .text('更新画面放缩比')
        .addClass('btn btn-primary btn-sm')
        .appendTo('.action-container')
        .on('click', async () => {
          try {
            await veGameSdkInstance.updateVideoScale();
            alert('更新画面放缩比成功')
          } catch (error) {
            console.error(error)
          }
        });
    },
    stopSuccess() {
      $(updateVideoScaleDataBtn).remove();
      updateVideoScaleDataBtn = null;
    },
  };
};

export default updateVideoScale;
