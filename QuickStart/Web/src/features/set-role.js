// 切换角色
import { veGameRoleList } from "../constant.js";
import { MODE } from "@volcengine/vegame";

const setRole = (veGameSdkInstance) => {
  let setRoleDropDown = null;

  const setRoleFunc = async function () {
    var value = $(this).text();
    if (value) {
      try {
        await veGameSdkInstance.changeRole({
          userId: "xxx",
          role: veGameRoleList.find((item) => item.label === value).value,
        });
        alert(`切换角色成功，当前为${value}`);
      } catch (error) {
        console.error(error);
      }
    }
  };

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      // 手游才能设置挂机模式
      if (veGameSdkInstance.mode === MODE.CLOUD_PHONE_GAME) {
        setRoleDropDown = document.createElement("div");
        $(setRoleDropDown)
          .addClass("dropdown")
          .html(
            `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            切换角色
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
          ${veGameRoleList
            .map(
              (item) =>
                `<li><a class="dropdown-item" href="#">${item.label}</a></li>`
            )
            .join("")}
          </ul>`
          )
          .appendTo(".action-container")
          .on("click", ".dropdown-item", setRoleFunc);
      }
    },
    stopSuccess() {
      $(setRoleDropDown).remove();
      setRoleDropDown = null;
    },
  };
};

export default setRole;
