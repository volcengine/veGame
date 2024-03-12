const idleTime = (veGameSdkInstance) => {
  let syncBtn = null;
  let idleDataDropdown = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      idleDataDropdown = document.createElement('div');
      $(idleDataDropdown)
        .addClass('dropdown')
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            设置保活时间
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
              const res = await veGameSdkInstance.setIdleTime(Number(value.replace('s','')))
              console.log('res', res)
              alert('设置保活时间成功')
            } catch (error) {
              console.error(error)
            }
          }
        });
    },
    stopSuccess() {
      $(syncBtn).remove();
      syncBtn = null;

      $(idleDataDropdown).remove();
      idleDataDropdown = null;
    },
  };
};

export default idleTime;
