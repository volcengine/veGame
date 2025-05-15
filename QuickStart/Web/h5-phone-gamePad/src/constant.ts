enum veGamePad {
  Show = "show",
  Release = "release",
  StartModifyGamePadLayout = "startModifyGamePadLayout",
  ExportGamePadConfig = "exportGamePadConfig",
  RefreshGamePadLayoutWithConfig = "refreshGamePadLayoutWithConfig",
  EnableTouch = "enableTouch",
  DisableTouch = "disableTouch",
  EnableVibrate = "enableVibrate",
  DisableVibrate = "disableVibrate",
}

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
    label: "导出虚拟手柄配置",
    value: veGamePad.ExportGamePadConfig,
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
