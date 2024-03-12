import { veGameModeList } from '../constant.js'

const sessionMode = (veGameSdkInstance) => {
  let syncBtn = null;
  let sessionModeDataDropDown = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      // 剪贴板数据展示的 dom 节点
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
        .on('click', '.dropdown-item', async function() {
          var value = $(this).text();
          if(value){
            try {
              console.log('传入的参数', veGameModeList.find(item=>item.label === value).value)
              const res = await veGameSdkInstance.sessionMode(veGameModeList.find(item=>item.label === value).value)
              console.log('res', res)
              alert(`切换模式成功，当前为${value}`)
            } catch (error) {
              console.error(error)
            }
          }
        });
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
