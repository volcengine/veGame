const switchBackground = (veGameSdkInstance) => {
  let syncBtn = null;
  let switchBackgroundDataDropDown = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      // 剪贴板数据展示的 dom 节点
      switchBackgroundDataDropDown = document.createElement('div');
      $(switchBackgroundDataDropDown)
        .addClass('dropdown')
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            客户端前后台切换
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
            <li><a class="dropdown-item" href="#">切后台</a></li>
            <li><a class="dropdown-item" href="#">切前台</a></li>
          </ul>`,
        )
        .appendTo('.action-container')
        .on('click', '.dropdown-item', async function() {
          var value = $(this).text();
          console.log('选中的值:', value);
          if(value){
            try {
              await veGameSdkInstance.switchBackground(value==='切后台')
              alert('客户端前后台切换成功')
            } catch (error) {
              console.error(error)
            }
          }
        });
    },
    stopSuccess() {
      $(syncBtn).remove();
      syncBtn = null;

      $(switchBackgroundDataDropDown).remove();
      switchBackgroundDataDropDown = null;
    },
  };
};

export default switchBackground;
