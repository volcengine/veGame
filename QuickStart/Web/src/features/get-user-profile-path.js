// 获取存档路径
const getUserProfilePath = (veGameSdkInstance) => {
  let getUserProfilePathDropDown = null;

  const getUserProfilePathFunc = async function () {
    var value = $(this).text();
    if (value) {
      try {
        // 实例代码：具体的path可自定义
        const userProfilePath = await veGameSdkInstance.getUserProfilePath();
        alert(`保存用户配置文件的路径为：${JSON.stringify(userProfilePath)}`);
      } catch (error) {
        console.error(error);
      }
    }
  };

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      // 手游才能设置挂机模式
      getUserProfilePathDropDown = document.createElement("div");
      $(getUserProfilePathDropDown)
        .text("获取保存用户配置文件的路径")
        .addClass("btn btn-primary btn-sm")
        .appendTo(".action-container")
        .on("click", getUserProfilePathFunc);
    },
    stopSuccess() {
      $(getUserProfilePathDropDown).remove();
      getUserProfilePathDropDown = null;
    },
  };
};

export default getUserProfilePath;
