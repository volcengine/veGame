const reconnectTime = (veGameSdkInstance) => {
  let syncBtn = null;
  let reconnectTimeDataDropDown = null;

  const setReconnectTime = async function () {
    var value = $(this).text();
    console.log("选中的值:", value);
    if (value) {
      try {
        // 单位 毫秒
        await veGameSdkInstance.setReconnectTime(
          Number(value.replace("s", "")) * 1000
        );
        alert(`已设置重连时间为${value}`);
      } catch (error) {
        console.error(error);
      }
    }
  };

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      reconnectTimeDataDropDown = document.createElement("div");
      $(reconnectTimeDataDropDown)
        .addClass("dropdown")
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            设置重连时间
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
            <li><a class="dropdown-item" href="#">0s</a></li>
            <li><a class="dropdown-item" href="#">10s</a></li>
            <li><a class="dropdown-item" href="#">20s</a></li>
            <li><a class="dropdown-item" href="#">100s</a></li>
          </ul>`
        )
        .appendTo(".action-container")
        .on("click", ".dropdown-item", setReconnectTime);
    },
    stopSuccess() {
      $(syncBtn).remove();
      syncBtn = null;

      $(reconnectTimeDataDropDown).remove();
      reconnectTimeDataDropDown = null;
    },
  };
};

export default reconnectTime;
