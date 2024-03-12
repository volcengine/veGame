const getAutoRecycle = (veGameSdkInstance) => {

  let getAutoRecycleDataBtn = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      getAutoRecycleDataBtn = document.createElement('btn');
      $(getAutoRecycleDataBtn)
        .text('获取自动回收时长')
        .addClass('btn btn-primary btn-sm')
        .appendTo('.action-container')
        .on('click', async () => {
          try {
            const res = await veGameSdkInstance.getAutoRecycleTime();
            console.log(res)
            alert(`自动回收时长为${res.duration}秒`)
          } catch (error) {
            console.error(error)
          }
        });
    },
    stopSuccess() {
      $(getAutoRecycleDataBtn).remove();
      getAutoRecycleDataBtn = null;
    },
  };
};

export default getAutoRecycle;
