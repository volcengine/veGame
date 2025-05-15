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