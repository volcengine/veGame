package com.volcengine.service.vegame.impl;

import com.alibaba.fastjson.JSON;
import com.volcengine.error.SdkError;
import com.volcengine.helper.Utils;
import com.volcengine.model.request.ListGameReq;
import com.volcengine.model.response.ListGameResp;
import com.volcengine.service.BaseServiceImpl;
import com.volcengine.service.vegame.IVeGameService;
import com.volcengine.service.vegame.VeGameConfig;
import com.volcengine.model.response.RawResponse;

public class VeGameServiceImpl extends BaseServiceImpl implements IVeGameService {
    private VeGameServiceImpl() {
        super(VeGameConfig.serviceInfo, VeGameConfig.apiInfoList);
    }
    public static VeGameServiceImpl getInstance() {
        return new VeGameServiceImpl();
    }

    @Override
    public ListGameResp ListGame(ListGameReq req) throws Exception{
        RawResponse resp = query(VeGameConfig.ListGame, Utils.mapToPairList(Utils.paramsToMap(req)));
        if (resp.getCode() != SdkError.SUCCESS.getNumber()) {
            throw resp.getException();
        }

        return JSON.parseObject(resp.getData(), ListGameResp.class);
    }
}
