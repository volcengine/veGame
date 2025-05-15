# Vue 框架集成方案

## 1. 创建 Vue 组件

在您 Vue 项目的 `src/components` 目录下新增目录 `cloudgame`，将 `pc-web-phone/src` 目录下的 `core.ts` 和 `env.d.ts` 复制到 cloudgame 目录下。 在 `views` 目录下创建 `GameController.vue`

```vue
<template>
  <div class="container">
    <div id="player" class="pc-portrait"></div>

    <button id="toggle-panel" class="toggle-btn" @click="handleTogglePanel">
      {{ showPanel ? '隐藏控制面板' : '显示控制面板' }}
    </button>

    <div class="control-panel" id="control-panel">
      <button id="start-btn" @click="startGame">
        开始游戏
      </button>
      <button id="stop-btn" @click="stopGame">
        停止游戏
      </button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { MODE } from '@volcengine/vegame'
import GameController from '@/components/cloudgame/core'

const showPanel = ref(true)
const gameController = ref<GameController | null>(null)

onMounted(() => {
  // 初始化 sdk
  gameController.value = new GameController({
    initConfig: {
      mode: MODE.CLOUD_PHONE_GAME, // 手游
      isPC: true, // pc端
      domId: 'player', // 容器id
    },
    startConfig: {
      // 游戏的横竖屏，mode为CLOUD_PHONE_GAME时传portrait，mode为CLOUD_GAME时传landscape
      gameRotation: 'portrait',
    },
  })
})

// 控制面板切换功能，与sdk无关
const handleTogglePanel = () => {
  const controlPanel = document.getElementById('control-panel')
  controlPanel?.classList.toggle('hidden')
  showPanel.value = !showPanel.value
}

const startGame = () => {
  gameController.value?.start()
}

const stopGame = () => {
  gameController.value?.stop()
}
</script>

<style scoped>
@import '@/assets/game-controller.css';
</style> 
```

## 2. 环境变量调整

在实际的项目使用中，我们推荐直接在代码`core.ts`修改，这样就不需要复制 `.env` 文件了。另外，对于`token`参数，可以直接 [通过 openapi 接口](https://www.volcengine.com/docs/6512/75588) 获取，则`core.ts`文件调整为：

```
import VeGameSDK, {
  CloudCoreConfig,
  CoreStartConfig,
  MODE,
} from "@volcengine/vegame";

interface Props {
  initConfig?: Partial<CloudCoreConfig> & Pick<CloudCoreConfig, "mode">;
  startConfig?: Partial<CoreStartConfig>;
}

export default class GameController {
  sdkInstance: VeGameSDK;
  startConfig?: Partial<CloudCoreConfig>;
  initConfig?: Partial<CoreStartConfig> & Pick<CloudCoreConfig, "mode">;
  starting: boolean;
  stopping: boolean;

  constructor({ startConfig, initConfig }: Props) {
    this.sdkInstance = new VeGameSDK({
      userId: "vegame_github_quick_start",
      accountId: import.meta.env.VEGAME_ACCOUNT_ID,
      domId: "player",
      mode: initConfig?.mode ?? MODE.CLOUD_PHONE_GAME,
      isPC: true,
      isDebug: true,
      ...initConfig,
    });
    this.startConfig = startConfig;
    this.initConfig = initConfig;
    this.initEventListeners();
    this.starting = false;
    this.stopping = false;
  }

  private initEventListeners() {
    // 监听错误事件
    this.sdkInstance.on("error", (error) => {
      console.error("SDK Error:", error);
    });
  }

  public async start() {
    try {
      if (this.starting || this.stopping) {
        console.log("start or stop is running");
        return;
      }
      this.starting = true;
      // getSTSToken 是获取临时token的方法，具体参考 https://www.volcengine.com/docs/6512/75588 调用openAPI即可
      const {
        ak: AccessKeyID,
        sk: SecretAccessKey,
        token: SessionToken,
        create_at: CurrentTime,
        expire_at: ExpiredTime,
      } = getSTSToken();
      await this.sdkInstance.start({
        ...this.startConfig,
        accountId: 'your account id',
        gameId: 'your game id',
        token: {
          CurrentTime,
          ExpiredTime,
          SessionToken,
          AccessKeyID,
          SecretAccessKey,
        },
        roundId: "vegame_github_quick_start_round_id",
      });
    } catch (err) {
      // 详细错误信息可查看：https://www.volcengine.com/docs/6512/75597#%E5%90%AF%E5%8A%A8
      console.error("Start Error:", err);
    } finally {
      this.starting = false;
    }
  }

  public async stop() {
    try {
      if (this.starting || this.stopping) {
        console.log("start or stop is running");
        return;
      }
      this.stopping = true;
      await this.sdkInstance.stop();
    } catch (err) {
      // 详细错误信息可查看：https://www.volcengine.com/docs/6512/75597#%E9%94%99%E8%AF%AF%E7%A0%81
      console.error("Stop Error:", err);
    } finally {
      this.stopping = false;
    }
  }
}

```

但是，如果您想继续通过 .env 文件设置 SDK 初始化和启动参数，则可以将.env 文件复制到 Vue 项目的根目录下，然后修改`vite.config.ts`文件，将`envPrefix`修改为`VEGAME_`。

## 3. 以入口文件为例，展示该组件

```tsx
// App.vue
<script setup lang="ts">
import { RouterLink, RouterView } from 'vue-router'
import GameController from '@/views/GameController.vue'
</script>

<template>
  <GameController />
</template>

<style scoped>
</style>
```

## 4. 快速体验

以 PC 玩手游为例，我们为您提供了 Vue+Vite 的示例代码 `react-pc-web-phone-demo`，您可以进到该文件夹中按照 README 提示直接运行体验。
