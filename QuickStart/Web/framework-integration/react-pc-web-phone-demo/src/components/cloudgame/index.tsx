import React, { useEffect, useRef, useState } from "react";
import { MODE } from "@volcengine/vegame";

import GameController from "./core";
import "./styles/index.css";

export const CloudGameView = () => {
  const gameController = useRef<GameController | null>(null);
  const [showPanel, setShowPanel] = useState(true);

  useEffect(() => {
    // 初始化 sdk
    gameController.current = new GameController({
      initConfig: {
        mode: MODE.CLOUD_PHONE_GAME, // 手游
        isPC: true, // pc端
        domId: "player", // 容器id
      },
      startConfig: {
        // 游戏的横竖屏，mode为CLOUD_PHONE_GAME时传portrait，mode为CLOUD_GAME时传landscape
        gameRotation: "portrait",
      },
    });
  }, []);

  // 控制面板切换功能，与sdk无关
  const handleTogglePanel = () => {
    const controlPanel = document.getElementById("control-panel");
    controlPanel?.classList.toggle("hidden");
    setShowPanel(!showPanel);
  };

  const startGame = () => {
    gameController.current?.start();
  };

  const stopGame = () => {
    gameController.current?.stop();
  };

  return (
    <div className="container">
      <div id="player" className="pc-portrait"></div>

      <button
        id="toggle-panel"
        className="toggle-btn"
        onClick={handleTogglePanel}
      >
        {showPanel ? "隐藏控制面板" : "显示控制面板"}
      </button>

      <div className="control-panel" id="control-panel">
        <button id="start-btn" onClick={startGame}>
          开始游戏
        </button>
        <button id="stop-btn" onClick={stopGame}>
          停止游戏
        </button>
      </div>
    </div>
  );
};
