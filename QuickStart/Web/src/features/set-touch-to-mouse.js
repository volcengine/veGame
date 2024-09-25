// 单触点转鼠标消息
const setTouchToMouse = (veGameSdkInstance) => {
  let setTouchToMouseDropDown = null;

  const setTouchToMouseFunc = async function () {
    var value = $(this).text();
    if (value) {
      try {
        await veGameSdkInstance.Mouse.setTouchToMouse(value === "开启");
        alert(`单触点转鼠标消息${value}成功`);
      } catch (error) {
        console.error(error);
      }
    }
  };

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      // 手游才能设置挂机模式
      setTouchToMouseDropDown = document.createElement("div");
      $(setTouchToMouseDropDown)
        .addClass("dropdown")
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            单触点转鼠标消息
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
            <li><a class="dropdown-item" href="#">开启</a></li>
            <li><a class="dropdown-item" href="#">关闭</a></li>
          </ul>`
        )
        .appendTo(".action-container")
        .on("click", ".dropdown-item", setTouchToMouseFunc);
    },
    stopSuccess() {
      $(setTouchToMouseDropDown).remove();
      setTouchToMouseDropDown = null;
    },
  };
};

export default setTouchToMouse;
