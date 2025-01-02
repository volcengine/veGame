const Clarity = {
  Smooth: 2,
  SD: 6,
  UltraHD: 14,
  HD: 16,
};

export const clarityList = [
  {
    label: "流畅",
    value: Clarity.Smooth,
  },
  {
    label: "标清",
    value: Clarity.SD,
  },
  {
    label: "超清",
    value: Clarity.UltraHD,
  },
  {
    label: "高清",
    value: Clarity.HD,
  },
];

const VeGameMode = {
  Normal: 0,
  HangUp: 1,
};

export const veGameModeList = [
  {
    label: "正常模式",
    value: VeGameMode.Normal,
  },
  {
    label: "挂机模式",
    value: VeGameMode.HangUp,
  },
];

const VeGameRole = {
  Observer: 0,
  Operator: 1,
};

export const veGameRoleList = [
  {
    label: "观看者",
    value: VeGameRole.Observer,
  },
  {
    label: "操作者",
    value: VeGameRole.Operator,
  },
];

const veGameVirtualInputSuite = {
  Show: "show",
  Release: "release",
  StartEdit: "startEdit",
  ExportKeysConfig: "exportKeysConfig",
  EnableTouch: "enableTouch",
  DisableTouch: "disableTouch",
};

export const veGameVirtualInputSuiteList = [
  {
    label: "显示虚拟键鼠配置",
    value: veGameVirtualInputSuite.Show,
  },
  {
    label: "销毁虚拟键鼠配置",
    value: veGameVirtualInputSuite.Release,
  },
  {
    label: "开启编辑虚拟键鼠配置",
    value: veGameVirtualInputSuite.StartEdit,
  },
  {
    label: "导出虚拟键鼠配置",
    value: veGameVirtualInputSuite.ExportKeysConfig,
  },
  {
    label: "开启触屏操作透传至游戏",
    value: veGameVirtualInputSuite.EnableTouch,
  },
  {
    label: "关闭触屏操作透传至游戏",
    value: veGameVirtualInputSuite.DisableTouch,
  },
];

const veGamePad = {
  Show: "show",
  Release: "release",
  StartModifyGamePadLayout: "startModifyGamePadLayout",
  ExportGamePadConfig: "exportGamePadConfig",
  RefreshGamePadLayoutWithConfig: "refreshGamePadLayoutWithConfig",
  EnableTouch: "enableTouch",
  DisableTouch: "disableTouch",
  EnableVibrate: "enableVibrate",
  DisableVibrate: "disableVibrate",
};

export const veGamePadList = [
  {
    label: "显示虚拟手柄配置",
    value: veGamePad.Show,
  },
  {
    label: "销毁虚拟手柄配置",
    value: veGamePad.Release,
  },
  {
    label: "开启编辑虚拟手柄配置",
    value: veGamePad.StartModifyGamePadLayout,
  },
  {
    label: "导出虚拟手柄配置",
    value: veGamePad.ExportGamePadConfig,
  },
  {
    label: "刷新虚拟手柄配置",
    value: veGamePad.RefreshGamePadLayoutWithConfig,
  },
  {
    label: "开启触屏操作透传至游戏",
    value: veGamePad.EnableTouch,
  },
  {
    label: "关闭触屏操作透传至游戏",
    value: veGamePad.DisableTouch,
  },
  {
    label: "开启震动",
    value: veGamePad.EnableVibrate,
  },
  {
    label: "关闭震动",
    value: veGamePad.DisableVibrate,
  },
];

// 触控类型
export const ACTION = {
  DOWN: 0,
  UP: 1,
  MOVE: 2,
  TOUCH_START: 0,
  TOUCH_END: 1,
  TOUCH_MOVE: 2,
  WHEEL: 8,
  PC_TOUCH_UP: 0,
  PC_TOUCH_DOWN: 1,
  PC_TOUCH_MOVE: 2,
};

// 鼠标按键
export const MOUSE_BUTTON = {
  LEFT: 0,
  CENTER: 1,
  RIGHT: 2,
};
