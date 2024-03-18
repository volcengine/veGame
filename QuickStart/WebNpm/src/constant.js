const Clarity = {
    Smooth: 2,
    SD: 6,
    UltraHD: 14,
    HD: 16
};

export const clarityList = [
    {
        label: '流畅',
        value: Clarity.Smooth,
    },
    {
        label: '标清',
        value: Clarity.SD,
    },
    {
        label: '超清',
        value: Clarity.UltraHD,
    },
    {
        label: '高清',
        value: Clarity.HD,
    }
]

const VeGameMode = {
    Normal: 0,
    HangUp: 1,
};

export const veGameModeList = [
    {
        label: '正常模式',
        value: VeGameMode.Normal,
    },
    {
        label: '挂机模式',
        value: VeGameMode.HangUp,
    }
]