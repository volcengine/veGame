import VeGameSDK, { MODE } from '@volcengine/vegame'
import type { CloudCoreConfig } from '@volcengine/vegame'
import type { CoreStartConfig } from '@volcengine/vegame'

interface Props {
  initConfig?: Partial<CloudCoreConfig> & Pick<CloudCoreConfig, 'mode'>
  startConfig?: Partial<CoreStartConfig>
}

class GameController {
  sdkInstance: VeGameSDK
  startConfig?: Partial<CloudCoreConfig>
  initConfig?: Partial<CoreStartConfig> & Pick<CloudCoreConfig, 'mode'>
  starting: boolean
  stopping: boolean
  constructor({ startConfig, initConfig }: Props) {
    console.log('import.meta.env', import.meta.env)
    this.sdkInstance = new VeGameSDK({
      userId: 'vegame_github_quick_start',
      accountId: import.meta.env.VEGAME_ACCOUNT_ID,
      domId: 'player',
      mode: initConfig?.mode ?? MODE.CLOUD_PHONE_GAME,
      isPC: true,
      isDebug: true,
      ...initConfig,
    })
    this.startConfig = startConfig
    this.initConfig = initConfig
    this.initEventListeners()
    this.starting = false
    this.stopping = false
  }
  private initEventListeners() {
    // 监听错误事件
    this.sdkInstance.on('error', (error) => {
      console.error('SDK Error:', error)
    })
  }

  public async start() {
    console.log('start')
    try {
      if (this.starting || this.stopping) {
        console.log('start or stop is running')
        return
      }
      this.starting = true
      await this.sdkInstance.start({
        ...this.startConfig,
        accountId: import.meta.env.VEGAME_ACCOUNT_ID,
        gameId: import.meta.env.VEGAME_GAME_ID,
        token: {
          CurrentTime: import.meta.env.VEGAME_TOKEN_CURRENT_TIME,
          ExpiredTime: import.meta.env.VEGAME_TOKEN_EXPIRED_TIME,
          SessionToken: import.meta.env.VEGAME_TOKEN_SESSION_TOKEN,
          AccessKeyID: import.meta.env.VEGAME_TOKEN_ACCESS_KEY_ID,
          SecretAccessKey: import.meta.env.VEGAME_TOKEN_SECRET_ACCESS_KEY,
        },
        roundId: 'vegame_github_quick_start_round_id',
      })
    } catch (err) {
      // 详细错误信息可查看：https://www.volcengine.com/docs/6512/75597#%E5%90%AF%E5%8A%A8
      console.error('Start Error:', err)
    } finally {
      this.starting = false
    }
  }

  public async stop() {
    try {
      if (this.starting || this.stopping) {
        console.log('start or stop is running')
        return
      }
      this.stopping = true
      await this.sdkInstance.stop()
    } catch (err) {
      // 详细错误信息可查看：https://www.volcengine.com/docs/6512/75597#%E9%94%99%E8%AF%AF%E7%A0%81
      console.error('Stop Error:', err)
    } finally {
      this.stopping = false
    }
  }
}

export default GameController
