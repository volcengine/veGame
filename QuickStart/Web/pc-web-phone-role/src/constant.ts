// 定义游戏类型枚举
export enum GameType {
  SingleControlSingleView = 0, // 单人控制单人观看，默认
  SingleControlMultiViewNoTransfer = 1, // 单人控制多人观看，不可转移游戏控制权
  SingleControlMultiViewTransfer = 2, // 单人控制多人观看，可转移游戏控制权
}

// 定义用户角色枚举
export enum UserRole {
  Viewer = 0, // 观看者，默认
  Operator = 1, // 操作者
}
