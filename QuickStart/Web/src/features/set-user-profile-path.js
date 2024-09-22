// 设置存档
const setUserProfilePath = (veGameSdkInstance) => {
  let setUserProfilePathDropDown = null;

  const setUserProfilePathFunc = async function () {
    var value = $(this).text();
    if (value) {
      try {
        // 实例代码：具体的path可自定义
        await veGameSdkInstance.setUserProfilePath(["/a/b/", "/c/d/"]);
        alert(`设置保存用户配置文件的路径成功`);
      } catch (error) {
        console.error(error);
      }
    }
  };

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      // 手游才能设置挂机模式
      setUserProfilePathDropDown = document.createElement("div");
      $(setUserProfilePathDropDown)
        .text("设置保存用户配置文件的路径")
        .addClass("btn btn-primary btn-sm")
        .appendTo(".action-container")
        .on("click", setUserProfilePathFunc);
    },
    stopSuccess() {
      $(setUserProfilePathDropDown).remove();
      setUserProfilePathDropDown = null;
    },
  };
};

export default setUserProfilePath;
