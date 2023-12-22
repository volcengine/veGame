import veGameSDK from '@volcengine/vegame';
import { initConfig, startConfig } from './config';
import { initTosFileChannel } from './samples/file-channel';

const startBtn = document.getElementById('start-btn');
const stopBtn = document.getElementById('stop-btn');

function init() {
  const veGameInstance = new veGameSDK(initConfig);
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
      alert('游戏超时退出');
    }
  });

  // 在页面 unmount 时检查云游戏是否处在运行状态，如果是正在运行，则调用 stop
  // 这样做的好处是：1. 释放 webrtc 链接，减少浏览器内存占用 2. 释放云游戏并发
  window.addEventListener('beforeunload', () => {
    /**
     * 这里采用 beforeunload 事件里调用 stop
     * 如果业务在 beforeunload 有业务逻辑，会调用 event.preventDefault
     * 则需要再 unload 事件里调用 stop
     */
    const connectionState = veGameInstance.getConnectionState();
    // connectionState 是 CONNECTED 代表云游戏处在运行状态，此时需要 stop
    if (connectionState === 'CONNECTED') {
      veGameInstance?.stop();
      veGameInstance.destroy();
    }
  });

  initTosFileChannel(veGameInstance);

  return veGameInstance;
}

function bindEventListener(veGameInstance) {
  startBtn.addEventListener('click', async () => {
    console.log('start btn click', veGameInstance);
    try {
      await veGameInstance?.start(startConfig);
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
      // veGameInstance.destroy() 会销毁运行期间创建的 dom 以及绑定的事件
      // 如果需要多次 start/stop 的话，请**不要**调用 destroy，否则建议在 stop 之后调用 destroy
      // veGameInstance.destroy();
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

(async () => {
  // 在启动云游戏之前，先检测用户的浏览器是否支持 rtc， 如果不支持，提示用户更换浏览器
  const isSupportRtc = await veGameSDK.isRtcSupported();
  if (!isSupportRtc) {
    console.log('当前浏览器不支持 WebRTC，请更换浏览器');
    return;
  }
  const veGameInstance = init();
  bindEventListener(veGameInstance);
})();
