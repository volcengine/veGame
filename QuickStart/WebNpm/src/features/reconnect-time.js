const reconnectTime = (veGameSdkInstance) => {
  let syncBtn = null;
  let reconnectTimeDataDropDown = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      // 剪贴板数据展示的 dom 节点
      reconnectTimeDataDropDown = document.createElement('div');
      $(reconnectTimeDataDropDown)
        .addClass('dropdown')
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            设置重连时间
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
            <li><a class="dropdown-item" href="#">0s</a></li>
            <li><a class="dropdown-item" href="#">10s</a></li>
            <li><a class="dropdown-item" href="#">20s</a></li>
            <li><a class="dropdown-item" href="#">100s</a></li>
          </ul>`,
        )
        .appendTo('.action-container')
        .on('click', '.dropdown-item', async function() {
          var value = $(this).text();
          console.log('选中的值:', value);
          if(value){
            try {
              await veGameSdkInstance.setReconnectTime(Number(value.replace('s','')))
              alert('设置重连时间成功')
            } catch (error) {
              console.error(error)
            }
          }
        });
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
