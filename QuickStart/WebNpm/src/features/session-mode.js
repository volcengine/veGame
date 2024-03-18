import { veGameModeList } from '../constant.js'
import { MODE } from '@volcengine/vegame';

const sessionMode = (veGameSdkInstance) => {
  let syncBtn = null;
  let sessionModeDataDropDown = null;

  const setSessionModeFn = async function() {
    var value = $(this).text();
    if(value){
      try {
        await veGameSdkInstance.sessionMode(veGameModeList.find(item=>item.label === value).value)
        alert(`切换模式成功，当前为${value}`)
      } catch (error) {
        console.error(error)
      }
    }
  }

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      // 手游才能设置挂机模式
      if(veGameSdkInstance.mode === MODE.CLOUD_PHONE_GAME){
        sessionModeDataDropDown = document.createElement('div');
      $(sessionModeDataDropDown)
        .addClass('dropdown')
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            切换模式
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
            <li><a class="dropdown-item" href="#">正常模式</a></li>
            <li><a class="dropdown-item" href="#">挂机模式</a></li>
          </ul>`,
        )
        .appendTo('.action-container')
        .on('click', '.dropdown-item', setSessionModeFn)
      }
    },
    stopSuccess() {
      $(syncBtn).remove();
      syncBtn = null;

      $(sessionModeDataDropDown).remove();
      sessionModeDataDropDown = null;
    },
  };
};

export default sessionMode;
