// 参考文档：https://www.volcengine.com/docs/6512/1330725#exportkeysconfig
import { veGameVirtualInputSuiteList } from "../constant.js";
import { MODE } from "@volcengine/vegame";

const virtualInputSuite = (veGameSdkInstance) => {
  let virtualInputSuiteDropDown = null;

  let vis = null;
  const virtualInputSuiteFunc = async function () {
    var value = $(this).text();
    if (value) {
      try {
        switch (value) {
          case "关闭触屏操作透传至游戏":
            vis.enableTouch(false);
            console.log(`${value}成功`);
            break;
          case "导出虚拟键鼠配置":
            const config = await vis.exportKeysConfig();
            console.log(`导出虚拟键鼠配置成功，${JSON.stringify(config)}`);
            break;
          default:
            vis[
              veGameVirtualInputSuiteList.find((item) => item.label === value)
                .value
            ]?.();
            console.log(`${value}成功`);
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
      console.log("virtualInputSuite", veGameSdkInstance);
      // 引入提供的 umd 文件，全局变量名为 veGameVirtualInputSuite
      vis = new veGameVirtualInputSuite(veGameSdkInstance);

      // 从操控状态进入编辑状态时（调用 startEdit 方法），触发此事件回调。
      vis.on("start-edit", () => {
        console.log("start-edit");
      });
      // 虚拟键鼠在编辑状态下，用户单击恢复默认时，触发此事件回调，此时虚拟键鼠恢复至进入编辑之前的 UI 配置。
      vis.on("restore-default", () => {
        console.log("restore-default");
      });
      // 在编辑状态下，用户单击保存时，触发此事件回调，此时虚拟键鼠更新为新的 UI 配置。
      vis.on("save-config", (params) => {
        const { config } = params;
        console.log("save-config", config);
      });
      // 虚拟键鼠从在编辑状态下退出至操作状态时，触发此事件。（用户点击恢复默认或保存都会触发）
      vis.on("exit-edit", () => {
        console.log("exit-edit");
      });

      if (veGameSdkInstance.mode === MODE.CLOUD_PHONE_GAME) {
        virtualInputSuiteDropDown = document.createElement("div");
        $(virtualInputSuiteDropDown)
          .addClass("dropdown")
          .html(
            `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            虚拟键鼠
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
          ${veGameVirtualInputSuiteList
            .map(
              (item) =>
                `<li><a class="dropdown-item" href="#">${item.label}</a></li>`
            )
            .join("")}
          </ul>`
          )
          .appendTo(".action-container")
          .on("click", ".dropdown-item", virtualInputSuiteFunc);
      }
    },
    stopSuccess() {
      $(virtualInputSuiteDropDown).remove();
      virtualInputSuiteDropDown = null;
      vis.release();
      vis = null;
    },
  };
};

export default virtualInputSuite;
