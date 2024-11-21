const rotateCoordinate = (veGameSdkInstance) => {
  let syncBtn = null;
  let rotateCoordinateDataDropdown = null;

  const rotateCoordinate = async function () {
    var value = $(this).text();
    if (value) {
      try {
        const degree = Number(value.replace("度", ""));
        const res = await veGameSdkInstance.rotateCoordinate(degree);
        $(".player-container").css("transform", `rotate(${degree}deg)`);
        console.log("res", res);
        console.log(`坐标已旋转${value}`);
      } catch (error) {
        console.error(error);
      }
    }
  };

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      rotateCoordinateDataDropdown = document.createElement("div");
      $(rotateCoordinateDataDropdown)
        .addClass("dropdown")
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            坐标旋转
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
            <li><a class="dropdown-item" href="#">0度</a></li>
            <li><a class="dropdown-item" href="#">90度</a></li>
            <li><a class="dropdown-item" href="#">180度</a></li>
            <li><a class="dropdown-item" href="#">270度</a></li>
          </ul>`
        )
        .appendTo(".action-container")
        .on("click", ".dropdown-item", rotateCoordinate);
    },
    stopSuccess() {
      $(syncBtn).remove();
      syncBtn = null;

      $(rotateCoordinateDataDropdown).remove();
      rotateCoordinateDataDropdown = null;
    },
  };
};

export default rotateCoordinate;
