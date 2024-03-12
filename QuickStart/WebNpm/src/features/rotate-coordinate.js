const rotateCoordinate = (veGameSdkInstance) => {
  let syncBtn = null;
  let rotateCoordinateDataDropdown = null;

  // 返回的 startSuccess 和 stopSuccess 方法会分别在成功启动云手机和成功停止云游戏时调用
  return {
    startSuccess() {
      // 剪贴板数据展示的 dom 节点
      rotateCoordinateDataDropdown = document.createElement('div');
      $(rotateCoordinateDataDropdown)
        .addClass('dropdown')
        .html(
          `<button class="btn btn-sm btn-primary dropdown-toggle" type="button" id="dropdownMenuButton" data-bs-toggle="dropdown">
            坐标旋转
          </button>
          <ul class="dropdown-menu" aria-labelledby="dropdownMenuButton">
            <li><a class="dropdown-item" href="#">0度</a></li>
            <li><a class="dropdown-item" href="#">90度</a></li>
            <li><a class="dropdown-item" href="#">180度</a></li>
            <li><a class="dropdown-item" href="#">270度</a></li>
          </ul>`,
        )
        .appendTo('.action-container')
        .on('click', '.dropdown-item', async function() {
          var value = $(this).text();
          console.log('选中的值:', value);
          if(value){
            try {
              const degree = Number(value.replace('度',''))
              const res = await veGameSdkInstance.rotateCoordinate(degree)
              $('.player-container').css('transform', `rotate(${degree}deg)`);
              console.log('res', res)
              alert('坐标旋转成功')
            } catch (error) {
              console.error(error)
            }
          }
        });
    },
    stopSuccess() {
      $(syncBtn).remove();
      syncBtn = null;

      $(rotateCoordinateDataDropdown).remove();
      rotateCoordinateDataDropdown = null;
    },
  };
};

export default rotateCoordinate;
