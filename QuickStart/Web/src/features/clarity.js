import { clarityList } from "../constant.js";

const clarity = (veGameSdkInstance) => {
  let syncBtn = null;
  let clarityDataDropdown = null;

  const clarityData = async function () {
    var value = $(this).text();
    if (value) {
      const currentClarityValue = clarityList.find(
        (item) => item.label === value
      );
      try {
        const res = await veGameSdkInstance.switchVideoStreamProfile(
          currentClarityValue.value
        );
        console.log("res", res);
        console.log(`清晰度已切换为${value}`);
      } catch (error) {
        console.error(error);
      }
    }
  };

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      clarityDataDropdown = document.createElement("div");
      $(clarityDataDropdown)
        .addClass("dropdown")
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            切换清晰度
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
            <li><a class="dropdown-item" href="#">流畅</a></li>
            <li><a class="dropdown-item" href="#">标清</a></li>
            <li><a class="dropdown-item" href="#">高清</a></li>
            <li><a class="dropdown-item" href="#">超清</a></li>
          </ul>`
        )
        .appendTo(".action-container")
        .on("click", ".dropdown-item", clarityData);
    },
    stopSuccess() {
      $(syncBtn).remove();
      syncBtn = null;

      $(clarityDataDropdown).remove();
      clarityDataDropdown = null;
    },
  };
};

export default clarity;
