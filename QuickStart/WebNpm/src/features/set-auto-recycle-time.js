const autoRecycle = (veGameSdkInstance) => {
  let syncBtn = null;
  let autoRecycleDataDropdown = null;

  const setAutoRecycleTime = async function() {
    var value = $(this).text();
    if(value){
      try {
        await veGameSdkInstance.setAutoRecycleTime(Number(value.replace('s','')))
        alert(`已设置自动回收时长为${value}`)
      } catch (error) {
        console.error(error)
      }
    }
  }

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云游戏和成功停止云游戏时调用
  return {
    startSuccess() {
      autoRecycleDataDropdown = document.createElement('div');
      $(autoRecycleDataDropdown)
        .addClass('dropdown')
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            设置自动回收时长
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
            <li><a class="dropdown-item" href="#">100s</a></li>
            <li><a class="dropdown-item" href="#">200s</a></li>
            <li><a class="dropdown-item" href="#">300s</a></li>
          </ul>`,
        )
        .appendTo('.action-container')
        .on('click', '.dropdown-item', setAutoRecycleTime);
    },
    stopSuccess() {
      $(syncBtn).remove();
      syncBtn = null;

      $(autoRecycleDataDropdown).remove();
      autoRecycleDataDropdown = null;
    },
  };
};

export default autoRecycle;
