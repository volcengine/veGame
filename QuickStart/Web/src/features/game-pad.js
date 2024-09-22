// 虚拟手柄 参考文档： https://www.volcengine.com/docs/6512/1263810
import { veGamePadList } from "../constant.js";
import { MODE } from "@volcengine/vegame";

const gamePad = (veGameSdkInstance) => {
  let gamePadDropDown = null;

  let pad = null;
  const gamePadFunc = async function () {
    var value = $(this).text();
    if (value) {
      try {
        switch (value) {
          case "关闭触屏操作透传至游戏":
            pad.enableTouch(false);
            alert(`${value}成功`);
            break;
          case "关闭震动":
            pad.enableVibrate(false);
            alert(`${value}成功`);
            break;
          case "导出虚拟手柄配置":
            const config = await pad.exportGamePadConfig();
            alert(`导出虚拟手柄配置，${JSON.stringify(config)}`);
            break;
          default:
            pad[veGamePadList.find((item) => item.label === value).value]?.();
            alert(`${value}成功`);
            break;
        }
      } catch (error) {
        console.error(error);
      }
    }
  };

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      console.log("gamePad", veGameSdkInstance);
      // 引入提供的 umd 文件，全局变量名为 veGamegamePad
      pad = new veGamePadSDK(veGameSdkInstance);

      // 从操控状态进入编辑状态时（调用 startEdit 方法），触发此事件回调。
      pad.on("start-edit", () => {
        console.log("start-edit");
      });
      // 手柄在编辑状态下，用户单击恢复默认时，触发此事件回调，此时手柄恢复至进入编辑之前的 UI 配置。
      pad.on("restore-default", () => {
        console.log("restore-default");
      });
      pad.on("cancel-edit", () => {
        console.log("cancel-edit");
      });
      // 在编辑状态下，用户单击保存时，触发此事件回调，此时手柄更新为新的 UI 配置。
      pad.on("save-config", (params) => {
        const { config } = params;
        console.log("save-config", config);
      });

      // 游戏过程中，游戏内部的震动场景会触发此事件。
      pad.on("gamepad-vibrate", (params) => {
        const { index, l, r } = params;
        console.log("index", index, "l", l, "r", r);
      });

      if (veGameSdkInstance.mode === MODE.CLOUD_PHONE_GAME) {
        gamePadDropDown = document.createElement("div");
        $(gamePadDropDown)
          .addClass("dropdown")
          .html(
            `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            虚拟手柄
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
          ${veGamePadList
            .map(
              (item) =>
                `<li><a class="dropdown-item" href="#">${item.label}</a></li>`
            )
            .join("")}
          </ul>`
          )
          .appendTo(".action-container")
          .on("click", ".dropdown-item", gamePadFunc);
      }
    },
    stopSuccess() {
      $(gamePadDropDown).remove();
      gamePadDropDown = null;
      pad.release();
      pad = null;
    },
  };
};

export default gamePad;
