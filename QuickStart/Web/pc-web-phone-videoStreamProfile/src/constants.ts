// 手游清晰度档位对照表：https://www.volcengine.com/docs/6512/1256324#%E6%A1%A3%E4%BD%8D-id-%E5%AF%B9%E7%85%A7%E8%A1%A8
// 此处仅展示几个常用的档位
export enum AndroidVideoStreamProfile {
  Smooth = 2, // 流畅
  SD = 6, // 标清，默认档位
  HD = 14, // 高清
  UltraHD = 16, // 超清
}

export const androidVideoStreamProfileOptions = [
  {
    label: "720*1280:4000kbps:30fps",
    value: 1,
  },
  {
    label: "720*1280:4000kbps:60fps(流畅)",
    value: AndroidVideoStreamProfile.Smooth,
  },
  {
    label: "720*1280:5000kbps:30fps",
    value: 3,
  },
  {
    label: "720*1280:5000kbps:60fps",
    value: 4,
  },
  {
    label: "720*1280:6000kbps:30fps",
    value: 5,
  },
  {
    label: "720*1280:6000kbps:60fps(标清，默认档位)",
    value: AndroidVideoStreamProfile.SD,
  },
  {
    label: "720*1280:8000kbps:30fps",
    value: 7,
  },
  {
    label: "720*1280:8000kbps:60fps",
    value: 8,
  },
  {
    label: "1080*1920:4000kbps:30fps",
    value: 9,
  },
  {
    label: "1080*1920:4000kbps:60fps",
    value: 10,
  },
  {
    label: "1080*1920:5000kbps:30fps",
    value: 11,
  },
  {
    label: "1080*1920:5000kbps:60fps",
    value: 12,
  },
  {
    label: "1080*1920:6000kbps:30fps",
    value: 13,
  },
  {
    label: "1080*1920:6000kbps:60fps(高清)",
    value: AndroidVideoStreamProfile.HD,
  },
  {
    label: "1080*1920:8000kbps:30fps",
    value: 15,
  },
  {
    label: "1080*1920:8000kbps:60fps(超清)",
    value: AndroidVideoStreamProfile.UltraHD,
  },
  {
    label: "540*960:4000kbps:30fps",
    value: 17,
  },
  {
    label: "450*800:4000kbps:30fps",
    value: 18,
  },
  {
    label: "360*640:3000kbps:30fps",
    value: 19,
  },
  {
    label: "1080*1920:10000kbps:30fps",
    value: 20,
  },
  {
    label: "1080*1920:10000kbps:60fps",
    value: 21,
  },
  {
    label: "1440*2560:12000kbps:30fps",
    value: 22,
  },
  {
    label: "1440*2560:12000kbps:60fps",
    value: 23,
  },
  {
    label: "720*1280:2000kbps:30fps",
    value: 24,
  },
  {
    label: "720*1280:1000kbps:30fps",
    value: 25,
  },
];

// 端游清晰度档位对照表：https://www.volcengine.com/docs/6512/1316450#%E6%A1%A3%E4%BD%8D-id-%E5%AF%B9%E7%85%A7%E8%A1%A8
export enum PCVideoStreamProfile {
  Default = 19,
}
export const pcVideoStreamProfileOptions = [
  {
    label: "640*360:30:3Mbps",
    value: 1,
  },
  {
    label: "848*480:30:4Mbps",
    value: 2,
  },
  {
    label: "960*540:30:4Mbps",
    value: 3,
  },
  {
    label: "1024*576:30:4Mbps",
    value: 4,
  },
  {
    label: "1024*576:60:4Mbps",
    value: 5,
  },
  {
    label: "1280*720:30:4Mbps",
    value: 6,
  },
  {
    label: "1280*720:60:4Mbps",
    value: 7,
  },
  {
    label: "1280*720:30:5Mbps",
    value: 8,
  },
  {
    label: "1280*720:60:5Mbps",
    value: 9,
  },
  {
    label: "1280*720:30:6Mbps",
    value: 10,
  },
  {
    label: "1280*720:60:6Mbps",
    value: 11,
  },
  {
    label: "1280*720:30:8Mbps",
    value: 12,
  },
  {
    label: "1280*720:60:8Mbps",
    value: 13,
  },
  {
    label: "1920*1080:30:4Mbps",
    value: 14,
  },
  {
    label: "1920*1080:60:4Mbps",
    value: 15,
  },
  {
    label: "1920*1080:30:5Mbps",
    value: 16,
  },
  {
    label: "1920*1080:60:5Mbps",
    value: 17,
  },
  {
    label: "1920*1080:30:6Mbps",
    value: 18,
  },
  {
    label: "1920*1080:60:6Mbps(默认档位)",
    value: PCVideoStreamProfile.Default,
  },
  {
    label: "1920*1080:30:8Mbps",
    value: 20,
  },
  {
    label: "1920*1080:60:8Mbps",
    value: 21,
  },
  {
    label: "1920*1080:30:10Mbps",
    value: 22,
  },
  {
    label: "1920*1080:60:10Mbps",
    value: 23,
  },
  {
    label: "1920*1080:30:12Mbps",
    value: 24,
  },
  {
    label: "1920*1080:60:12Mbps",
    value: 25,
  },
  {
    label: "2560*1440:30:8Mbps",
    value: 26,
  },
  {
    label: "2560*1440:60:8Mbps",
    value: 27,
  },
  {
    label: "2560*1440:30:10Mbps",
    value: 28,
  },
  {
    label: "2560*1440:60:10Mbps",
    value: 29,
  },
  {
    label: "2560*1440:30:12Mbps",
    value: 30,
  },
  {
    label: "2560*1440:60:12Mbps",
    value: 31,
  },
  {
    label: "2560*1440:30:14Mbps",
    value: 32,
  },
  {
    label: "2560*1440:60:14Mbps",
    value: 33,
  },
  {
    label: "2560*1440:30:16Mbps",
    value: 34,
  },
  {
    label: "2560*1440:60:16Mbps",
    value: 35,
  },
  {
    label: "2560*1440:30:18Mbps",
    value: 36,
  },
  {
    label: "2560*1440:60:18Mbps",
    value: 37,
  },
  {
    label: "2560*1440:30:20Mbps",
    value: 38,
  },
  {
    label: "2560*1440:60:20Mbps",
    value: 39,
  },
  {
    label: "3840*2160:30:20Mbps",
    value: 40,
  },
  {
    label: "3840*2160:60:20Mbps",
    value: 41,
  },
  {
    label: "3840*2160:30:25Mbps",
    value: 42,
  },
  {
    label: "3840*2160:60:25Mbps",
    value: 43,
  },
  {
    label: "3840*2160:30:30Mbps",
    value: 44,
  },
  {
    label: "3840*2160:60:30Mbps",
    value: 45,
  },
  {
    label: "3840*2160:30:35Mbps",
    value: 46,
  },
  {
    label: "3840*2160:60:35Mbps",
    value: 47,
  },
  {
    label: "3840*2160:30:40Mbps",
    value: 48,
  },
  {
    label: "3840*2160:60:40Mbps",
    value: 49,
  },
];
