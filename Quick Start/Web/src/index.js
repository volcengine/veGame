/*
 * @Author: chenzhongsheng
 * @Date: 2023-01-05 18:31:22
 * @Description: Coding something
 */
const startBtn = document.getElementById('start-btn');
const stopBtn = document.getElementById('stop-btn');
const playerEl = document.getElementById('player');

const isPC =
  !/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(
    navigator.userAgent
  );

function init() {
  console.log(window.initConfig)
  // 引入 lib 下的 火山引擎云游戏 SDK 之后，会在 window 上挂一个全局变量，veGameSDK
  const veGameInstance = new window.veGameSDK({
    ...(window.initConfig || {}),
    domId: 'player',
    isDebug: true,
    isPC,
  });
  console.log('veGameSDK version', veGameInstance.getVersion());

  veGameInstance.on('message-channel-connected', (message) => {
    console.log('message-channel-connected', message);
  });

  veGameInstance.on('error', (error) => {
    console.log('error', error);
  });

  veGameInstance.on('message', (message) => {
    console.log('message', message);
  });

  veGameInstance.on('message-received', ({ msg }) => {
    console.log('message-received', msg);
    const { command } = msg;
    if (command === 8) {
      veGameInstance?.destory();
      alert('游戏超时退出');
    }
  });

  initTosFileChannel(veGameInstance);

  return veGameInstance;
}

function bindEventListener(veGameInstance) {
  startBtn.addEventListener('click', async () => {
    console.log('start btn click', veGameInstance);
    try {
      await veGameInstance?.start({
        ...(window.startConfig || {}),
        width: playerEl.clientWidth,
        height: playerEl.clientHeight,
      });
      toggleDom();
    } catch (err) {
      console.log(err);
      alert(err.message);
    }
  });

  stopBtn.addEventListener('click', async () => {
    console.log('stop btn click', veGameInstance);
    try {
      await veGameInstance?.stop();
      veGameInstance.destroy();
      toggleDom();
    } catch (err) {
      console.log(err);
      alert(err.message);
    }
  });
}

function toggleDom() {
  toggleHidden(startBtn);
  toggleHidden(stopBtn);
}

function toggleHidden(el) {
  el.classList.toggle('hidden');
}

(() => {
  const veGameInstance = init();
  bindEventListener(veGameInstance);
})();
